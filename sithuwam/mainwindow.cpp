#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QImageWriter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_canvas = new Canvas;
    setCentralWidget(m_canvas);
    createActions();
    createMenues();
    setWindowTitle(tr("Sithuwam"));
    resize(500, 500);
}

MainWindow::~MainWindow()
{
    delete m_canvas;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (shouldSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (shouldSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::currentPath());
        if (!fileName.isEmpty())
        {
            m_canvas->openImage(fileName);
        }
    }
}

void MainWindow::save()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QByteArray content = action->data().toByteArray();
    saveFile(content);
}

void MainWindow::penColor()
{
    QColor color = QColorDialog::getColor(m_canvas->getPenColor());
    if (color.isValid())
    {
        m_canvas->setPenColor(color);
    }
}

void MainWindow::penWidth()
{
    bool isOK = false;
    int width = QInputDialog::getInt(this, tr("Sithuwam"),
                                     tr("Select pen width : "), m_canvas->getPenWidth(),
                                     1, 50, 1, &isOK);
    if (isOK)
    {
        m_canvas->setPenWidth(width);
    }
}

void MainWindow::createActions()
{
    m_openAction = new QAction(tr("&Open"), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    connect(m_openAction, SIGNAL(triggered()), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString type = tr("%1...").arg(QString(format)).toUpper();
        QAction* action = new QAction(type, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        m_saveAsOptions.append(action);
    }

    m_printAction = new QAction(tr("&Print..."), this);
    connect(m_printAction, SIGNAL(triggered()), m_canvas, SLOT(print()));

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_penColorAction = new QAction(tr("&Pen Color.."), this);
    connect(m_penColorAction, SIGNAL(triggered()), this, SLOT(penColor()));
    m_penWidthAction = new QAction(tr("&Pen Width.."), this);
    connect(m_penWidthAction, SIGNAL(triggered()), this, SLOT(penWidth()));

    m_clearAction = new QAction(tr("&Clear"), this);
    m_clearAction->setShortcut(tr("Ctrl+L"));
    connect(m_clearAction, SIGNAL(triggered()), m_canvas, SLOT(clearImage()));
}

void MainWindow::createMenues()
{
    m_saveAsMenu = new QMenu(tr("&Save As.."), this);
    foreach(QAction* action, m_saveAsOptions)
    {
        m_saveAsMenu->addAction(action);
    }
    m_fileMenu = new QMenu(tr("&File"), this);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addMenu(m_saveAsMenu);
    m_fileMenu->addAction(m_printAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_optionsMenu = new QMenu(tr("&Options"), this);
    m_optionsMenu->addAction(m_penColorAction);
    m_optionsMenu->addAction(m_penWidthAction);
    m_optionsMenu->addAction(m_clearAction);

    menuBar()->addMenu(m_fileMenu);
    menuBar()->addMenu(m_optionsMenu);
}

bool MainWindow::shouldSave()
{
    if (m_canvas->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Sithuwam"),
                                   tr("The image has been modified.\nDo you want to save the changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            return saveFile("png");
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray& fileFormat)
{
    QString path = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As.."),
                                                    path, tr("%1 Files (*.%2;; All Files(*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
    {
        return false;
    }
    return m_canvas->saveImage(fileName, fileFormat.constData());
}
