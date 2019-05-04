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
    void regionGrowing();
    void waterShedSeg();
    void reset();
    void importdcm();
    void importdir();
    void goThroughFile();

protected :
    void mouseReleaseEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);
    void callRegionGrow();
    void callWaterShed();
    void callInit(QString pictures);
    void callExtractHeader(QString picture);
    void displayHeader();

private:
    Ui::MainWindow *ui;
    bool regionGrow;
    bool waterShed;
    bool thereIsPicture;
    bool thereIsSeed;
    QString path;
    QStringList files;
    QString dirpath;
    QString filename;
    QString filepath;
};

#endif // MAINWINDOW_H
