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
    void save();
    void setDarkTheme();
    void setLightTheme();
    void applyhsv();
    void applyrgb();
    void applyspectral();
    void applyblueandred();
    void applythresholds();
    void applyprism();
    void demonstration();
    void Thresholding();
    void changeofValue();

protected :
    void mouseReleaseEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);
    void callRegionGrow();
    void callWaterShed();
    void callInit(QString pictures);
    void callExtractHeader(QString picture);
    void displayHeader();
    void applyColormap(QString colormap);
    void callThreshold(QString threshold);
    void CTorMR();

private:
    Ui::MainWindow *ui;
    int picWidth;
    int picHeight;
    bool regionGrow;
    bool waterShed;
    bool thereIsPicture;
    bool thereIsSeed;
    bool thereIsValue;
    bool Threshold;
    bool isCT;
    bool isMR;
    QString path;
    QStringList files;
    QString dirpath;
    QString filename;
    QString filepath;
};

#endif // MAINWINDOW_H
