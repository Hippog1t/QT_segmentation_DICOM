#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots :
    void start();
    void reset();

protected :
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Ui::MainWindow *ui;
    bool regionGrow;
};

#endif // MAINWINDOW_H
