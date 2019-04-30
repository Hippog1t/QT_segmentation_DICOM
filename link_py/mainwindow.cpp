#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QProcess>
#include <windows.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    regionGrow = false;
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(reset()));

    QString path = QString("C:/Users/Alexandre/Documents/Projet2I/FinalCode");
    QString cmd_qt = QString("python "+
                             path+
                             "/init.py "+
                             path+
                             "/DATA_dcm/IM-0001-0180-0001.dcm");
    const char* cmd = cmd_qt.toLocal8Bit().constData();
    system(cmd);

    QPixmap pixmap(path+"/Out/initial.jpg");
    ui->picture->setPixmap(pixmap);
    ui->picture->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start(){
    if(!regionGrow){
        ui->picture->clear();
        ui->picture->setText(QString("CHARGEMENT..."));
        QString cmd_qt = QString("python C:/Users/Alexandre/Documents/Projet2I/FinalCode/regionGrow.py "+
                                 ui->x_pos->text()+" "+
                                 ui->y_pos->text());
        const char* cmd = cmd_qt.toLocal8Bit().constData();
        system(cmd);
        regionGrow = true;
    }

    QPixmap pixmap("../Out/regionGrow_dcm.jpg");
    ui->picture->setPixmap(pixmap);
    ui->picture->show();
}

void MainWindow::reset(){
    ui->picture->clear();
    regionGrow = false;
    QPixmap pixmap("../Out/initial.jpg");
    ui->picture->setPixmap(pixmap);
    ui->picture->show();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton){
        QPoint point = ui->picture->mapFromParent(event->pos());
        if(point.x()<=512 && point.x()>=0 && point.y()<=512 && point.y()>=0){
            ui->x_pos->setNum(point.x());
            ui->y_pos->setNum(point.y());
            regionGrow = false;
        }
        else{
            ui->x_pos->setText(QString(""));
            ui->y_pos->setText(QString(""));
        }
    }
}
