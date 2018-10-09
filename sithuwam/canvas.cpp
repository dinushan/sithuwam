#include "canvas.h"

#include <QPrinter>
#include <QSize>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPrintDialog>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    m_isModified = false;
    m_isDrawing  = false;
    m_penColor = Qt::black;
    m_penWidth = 1;
}

bool Canvas::openImage(const QString& fileName)
{
    QImage image;
    if (!image.load(fileName))
    {
        return false;
    }
    QSize size = image.size().expandedTo(QWidget::size());
    resizeImage(&image, size);
    m_image = image;
    m_isModified = false;
    update();
    return true;
}

bool Canvas::saveImage(const QString& fileName, const char* fileFormat)
{
    QImage image = m_image;
    resizeImage(&image, size());
    if (image.save(fileName, fileFormat))
    {
        m_isModified = false;
        return true;
    }
    return false;
}

void Canvas::setPenColor(const QColor& color)
{
    m_penColor = color;
}

void Canvas::setPenWidth(int width)
{
    m_penWidth = width;
}

void Canvas::clearImage()
{
    m_image.fill(qRgb(255, 255, 255));
    m_isModified = false;
    update();
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->pos();
        m_isDrawing = true;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_isDrawing)
    {
        drawLine(event->pos());
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isDrawing)
    {
        drawLine(event->pos());
        m_isDrawing = false;
    }
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    painter.drawImage(rect, m_image, rect);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
    if (width() > m_image.width() || height() > m_image.height())
    {
        int width = qMax(QWidget::width() + 128, m_image.width());
        int height = qMax(QWidget::height() + 128, m_image.height());
        resizeImage(&m_image, QSize(width, height));
        update();
    }
    QWidget::resizeEvent(event);
}

void Canvas::drawLine(const QPoint& endPoint)
{
    QPainter painter(&m_image);
    painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_lastPoint, endPoint);
    m_isModified = true;
    int rad = (m_penWidth / 2) + 2;
    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    m_lastPoint = endPoint;
}

void Canvas::resizeImage(QImage* image, const QSize& size)
{
    if (image->size() == size)
        return;

    QImage newImage(size, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void Canvas::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = m_image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_image.rect());
        painter.drawImage(0, 0, m_image);
    }
#endif
}
