#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QDir>
#include <QRegExp>
#include <windows.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    regionGrow = false;
    waterShed = false;
    thereIsPicture = false;
    thereIsSeed = false;

    path = QCoreApplication::applicationDirPath()+"/../..";

    connect(ui->rgButton, SIGNAL(clicked()), this, SLOT(regionGrowing()));
    connect(ui->lpeButton, SIGNAL(clicked()), this, SLOT(waterShedSeg()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->actionImporter_fichier_dcm, SIGNAL(triggered()), this, SLOT(importdcm()));

    QPixmap pixmap(path+"/Ressources/accueil.jpg");
    ui->picture->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::regionGrowing(){
    ui->errors->clear();
    if(!regionGrow && thereIsPicture && thereIsSeed){
        QString cmd_qt = QString("python "+
                                 path
                                 +"/regionGrow.py "+
                                 ui->x_pos->text()+" "+
                                 ui->y_pos->text()+" "+
                                 filepath+" "+
                                 path);
        const char* cmd = cmd_qt.toLocal8Bit().constData();
        system(cmd);
        ui->picture->clear();
        QPixmap pixmap(path+"/Ressources/chargement.jpg");
        ui->picture->setPixmap(pixmap);
        regionGrow = true;
    }

    if(regionGrow){
        QPixmap pixmap("../Out/regionGrow.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }

    if(!thereIsSeed){
        ui->errors->setText("Erreur : Vous devez indiquer une valeur de seed\nCliquez sur l'image");
    }
}

void MainWindow::reset(){
    QPixmap pixmap_accueil(path+"/Ressources/accueil.jpg");
    ui->picture->setPixmap(pixmap_accueil);
    regionGrow = false;

    if(thereIsPicture){
        QPixmap pixmap("../Out/initial.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }
}

void MainWindow::importdcm(){
    filepath = QFileDialog::getOpenFileName(this,tr("Select picture"),"C:/",tr("Dicom Files (*.dcm)"));
    QFileInfo f(filepath);
    filename = f.baseName();
    ui->filepath->setText(filepath);
    ui->filename->setText(filename);

    QString cmd_qt = QString("python "+
                             path+
                             "/init.py "+
                             filepath+" "+
                             path+"/Out/initial.jpg");
    const char* cmd = cmd_qt.toLocal8Bit().constData();
    system(cmd);

    QPixmap pixmap(path+"/Out/initial.jpg");
    ui->picture->setPixmap(pixmap);
    thereIsPicture = true;
}

void MainWindow::waterShedSeg(){
    if(!waterShed && thereIsPicture){
        QString cmd_qt = QString("python "+
                                 path
                                 +"/waterShed.py "+
                                 filepath+" "+
                                 path);
        const char* cmd = cmd_qt.toLocal8Bit().constData();
        system(cmd);
        ui->picture->clear();
        QPixmap pixmap(path+"/Ressources/chargement.jpg");
        ui->picture->setPixmap(pixmap);
        waterShed = true;
    }

    if(waterShed){
        QPixmap pixmap("../Out/waterShed.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton){
        QPoint point = ui->picture->mapFromParent(event->pos());
        if(point.x()<=512 && point.x()>=0 && point.y()<=512 && point.y()>=0){
            ui->x_pos->setNum(point.x());
            ui->y_pos->setNum(point.y());
            regionGrow = false;
            thereIsSeed = true;
        }
        else{
            ui->x_pos->setText(QString(""));
            ui->y_pos->setText(QString(""));
            thereIsSeed = false;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    QDir dir(path+"/Out/");
    dir.setNameFilters(QStringList() << "*.jpg");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
    QWidget::closeEvent(event);
}
