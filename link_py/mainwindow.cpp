#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QColorDialog>
#include <windows.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    //setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    //setWindowFlag(Qt::Window);

    regionGrow = false;
    waterShed = false;
    thereIsPicture = false;
    thereIsSeed = false;

    path = QCoreApplication::applicationDirPath()+"/../..";

    connect(ui->rgButton, SIGNAL(clicked()), this, SLOT(regionGrowing()));
    connect(ui->lpeButton, SIGNAL(clicked()), this, SLOT(waterShedSeg()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->action_import_file, SIGNAL(triggered()), this, SLOT(importdcm()));
    connect(ui->action_import_dir, SIGNAL(triggered()), this, SLOT(importdir()));
    connect(ui->selectFileDir, SIGNAL(valueChanged(int)), this, SLOT(goThroughFile()));
    connect(ui->action_sauver, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionChange_Background_Color, SIGNAL(triggered()), this, SLOT(changeBGColor()));
    connect(ui->actionChange_Text_Color, SIGNAL(triggered()), this, SLOT(changeTextColor()));

    ui->picture->setScaledContents( true );
    //ui->picture->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    QPixmap pixmap(path+"/Ressources/accueil.jpg");
    ui->picture->setPixmap(pixmap);

    QFont bigFont("Calibri", 16, false);
    QFont smallFont("Calibri", 11, false);
    appColor = Qt::gray;
    textColor = Qt::black;
    this->setFont(bigFont);
    ui->filepath->setFont(smallFont);
    ui->headerView->setFont(smallFont);

    ui->selectFileDir->setEnabled(false);

    ui->x_pos->setStyleSheet("QLabel { background-color : white; color : black;}");
    ui->y_pos->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->filename->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->filepath->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->filenumber->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->totalfiles->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->errors->setStyleSheet("QLabel { color : red; }");

    ui->currentfile->setStyleSheet("QLabel { text-decoration : underline;}");
    ui->seed->setStyleSheet("QLabel { text-decoration : underline;}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importdir(){
    dirpath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "C:/", QFileDialog::ShowDirsOnly);
    QDir dir = QDir(dirpath);
    if(dir.exists()){
        ui->picture->setPixmap(QPixmap(path+"/Ressources/chargement.jpg"));
        files = dir.entryList(QStringList() << "*.dcm", QDir::Files);

        ui->selectFileDir->setRange(0, files.length()-1);
        ui->selectFileDir->setTickInterval(1);
        ui->selectFileDir->setEnabled(true);
        ui->filenumber->setNum(1);
        ui->totalfiles->setText("/ "+QString::number(files.length()));

        callInit(dirpath);

        int v = ui->selectFileDir->value();

        filename = files[0];
        filepath = dirpath+"/"+filename;

        ui->filepath->setText(filepath);
        ui->filename->setText(filename);

        QPixmap pixmap(path+"/Out/initial"+QString::number(v)+".jpg");
        picWidth = pixmap.width();
        picHeight = pixmap.height();
        ui->picture->setPixmap(pixmap);
        displayHeader();

        thereIsPicture = true;
        regionGrow = false;
        waterShed = false;
        thereIsSeed = false;
    }
}

void MainWindow::importdcm(){
    filepath = QFileDialog::getOpenFileName(this,tr("Select picture"),"C:/",tr("Dicom Files (*.dcm);;Images (*.dcm *.jpg *.png)"));
    if(QFile(filepath).exists()){
        QFileInfo f(filepath);
        filename = f.baseName();
        ui->filepath->setText(filepath);
        ui->filename->setText(filename);
        ui->selectFileDir->setEnabled(false);
        ui->filenumber->clear();
        ui->totalfiles->clear();

        callInit(filepath);

        displayHeader();

        thereIsPicture = true;
        regionGrow = false;
        waterShed = false;
        thereIsSeed = false;

        QPixmap pixmap(path+"/Out/initial0.jpg");
        picWidth = pixmap.width();
        picHeight = pixmap.height();
        std::cout<<picWidth<<picHeight<<std::endl;
        ui->picture->setPixmap(pixmap);
    }
}

void MainWindow::goThroughFile(){
    int value = ui->selectFileDir->value();
    ui->filenumber->setNum(value+1);
    QPixmap pixmap(path+"/Out/initial"+QString::number(value)+".jpg");
    picWidth = pixmap.width();
    picHeight = pixmap.height();
    ui->picture->setPixmap(pixmap);
    filename = files[value];
    filepath = dirpath+"/"+filename;
    ui->filename->setText(filename);
    ui->filepath->setText(filepath);
    //displayHeader();
}

void MainWindow::reset(){
    QPixmap pixmap_accueil(path+"/Ressources/accueil.jpg");
    ui->picture->setPixmap(pixmap_accueil);
    regionGrow = false;
    waterShed = false;

    if(thereIsPicture){
        if(ui->selectFileDir->isEnabled()){
            int v = ui->selectFileDir->value();
            QPixmap pixmap(path+"/Out/initial"+QString::number(v)+".jpg");
        }
        QPixmap pixmap("../Out/initial0.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }
}

void MainWindow::regionGrowing(){
    ui->errors->clear();
    if(!regionGrow && thereIsPicture && thereIsSeed){
        callRegionGrow();
        ui->picture->clear();
        QPixmap pixmap(path+"/Ressources/chargement.jpg");
        ui->picture->setPixmap(pixmap);
        regionGrow = true;
        waterShed = false;
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

void MainWindow::waterShedSeg(){
    if(!waterShed && thereIsPicture){
        callWaterShed();
        ui->picture->clear();
        QPixmap pixmap(path+"/Ressources/chargement.jpg");
        ui->picture->setPixmap(pixmap);
        waterShed = true;
        regionGrow = false;
    }

    if(waterShed){
        QPixmap pixmap("../Out/waterShed.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton && thereIsPicture){
        QPoint point = ui->picture->mapFromParent(event->pos());
        if(point.x()<=ui->picture->width() && point.x()>=0 && point.y()<=ui->picture->height()+15 && point.y()>=15){
            ui->x_pos->setNum(int(point.x()*picWidth/1024));
            ui->y_pos->setNum(int(point.y())*picHeight/1024-15);
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
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
    QWidget::closeEvent(event);
}

void MainWindow::callInit(QString pictures){
    QString cmd_qt = QString("python "+
                             path+
                             "/init.py "+
                             pictures+" "+
                             path);
    const char* cmd = cmd_qt.toLocal8Bit().constData();
    system(cmd);
}

void MainWindow::callExtractHeader(QString picture){
    QString cmd_qt = QString("python "+
                     path+
                     "/extractHeader.py "+
                     picture+" "+
                     path);
    const char * cmd = cmd_qt.toLocal8Bit().constData();
    system(cmd);
}

void MainWindow::displayHeader(){
    ui->headerView->clear();
    callExtractHeader(filepath);

    QFile header(QString(path+"/Out/dicomheader.txt"));
    header.open(QIODevice::ReadOnly);
    QTextStream in(&header);

    QString line = in.readLine();
    while(!line.isNull()){
        QTreeWidgetItem * item = new QTreeWidgetItem(ui->headerView);
        item->setText(0, line);
        line = in.readLine();
        item->setText(1, line);
        ui->headerView->addTopLevelItem(item);
        line = in.readLine();
    }
    header.close();
}

void MainWindow::callRegionGrow(){
    QString cmd_qt = QString("python "+
                             path
                             +"/regionGrow.py "+
                             ui->x_pos->text()+" "+
                             ui->y_pos->text()+" "+
                             filepath+" "+
                             path);
    const char* cmd = cmd_qt.toLocal8Bit().constData();
    system(cmd);
}

void MainWindow::callWaterShed(){
    QString cmd_qt = QString("python "+
                             path
                             +"/waterShed.py "+
                             filepath+" "+
                             path);
    const char* cmd = cmd_qt.toLocal8Bit().constData();
    system(cmd);
}

void MainWindow::save(){
    QString imagePath = QFileDialog::getSaveFileName(this,tr("Save Picture"), "C:/", tr("JPEG (*.jpg *.jpeg)"));
    ui->picture->grab().save(imagePath);
}


//Note si on effectue les 2 à la suite, le texte perd sa police Calibri 16.
void MainWindow::changeBGColor(){
    appColor = QColorDialog::getColor();
    this->setStyleSheet(QString("color : "+textColor.name()+"; background-color : "+appColor.name()));
}

void MainWindow::changeTextColor(){
    textColor = QColorDialog::getColor();
    this->setStyleSheet(QString("color : "+textColor.name()+"; background-color : "+appColor.name()));
}
