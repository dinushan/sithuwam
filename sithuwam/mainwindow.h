#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class Canvas;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();

private:
    void createActions();
    void createMenues();
    bool shouldSave();
    bool saveFile(const QByteArray& fileFormat);

    Canvas* m_canvas;

    QMenu* m_saveAsMenu;
    QMenu* m_fileMenu;
    QMenu* m_optionsMenu;

    QAction* m_openAction;
    QList<QAction*> m_saveAsOptions;
    QAction* m_exitAction;
    QAction* m_penColorAction;
    QAction* m_penWidthAction;
    QAction* m_printAction;
    QAction* m_clearAction;

};

#endif // MAINWINDOW_H
