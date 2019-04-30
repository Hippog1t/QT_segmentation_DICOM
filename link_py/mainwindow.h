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
    void importdcm();

protected :
    void mouseReleaseEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;
    bool regionGrow;
    bool thereIsPicture;
    bool thereIsSeed;
    QString path;
    QString filename;
    QString filepath;
};

#endif // MAINWINDOW_H
