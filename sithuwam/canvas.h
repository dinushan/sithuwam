#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    bool openImage(const QString& fileName);
    bool saveImage(const QString& fileName, const char* fileFormat);
    void setPenColor(const QColor& color);
    void setPenWidth(int width);

    bool isModified() const { return m_isModified; }
    QColor getPenColor() const { return m_penColor; }
    int getPenWidth() const    { return m_penWidth; }

signals:

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawLine(const QPoint& endPoint);
    void resizeImage(QImage* image, const QSize& size);

    bool m_isModified;
    bool m_isDrawing;

    QColor m_penColor;
    int m_penWidth;

    QImage m_image;
    QPoint m_lastPoint;

};

#endif // CANVAS_H
