#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QColorDialog>
#include <windows.h>
#include <iostream>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    regionGrow = false;
    waterShed = false;
    Threshold = false;
    thereIsPicture = false;
    thereIsSeed = false;
    thereIsValue = false;
    isCT = false;
    isMR = false;

    //Location of the .exe file
    path = QCoreApplication::applicationDirPath();

    //Window Parameters
    this->setWindowIcon(QIcon(QPixmap(path+"/Ressources/icone.jpg")));
    this->setWindowTitle(QString("Medical Image Segmentation - ESME 2019"));

    //Links the buttons to the functions
    connect(ui->rgButton, SIGNAL(clicked()), this, SLOT(regionGrowing()));
    connect(ui->lpeButton, SIGNAL(clicked()), this, SLOT(waterShedSeg()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->action_import_file, SIGNAL(triggered()), this, SLOT(importdcm()));
    connect(ui->action_import_dir, SIGNAL(triggered()), this, SLOT(importdir()));
    connect(ui->selectFileDir, SIGNAL(valueChanged(int)), this, SLOT(goThroughFile()));
    connect(ui->action_sauver, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->action_DarkMode, SIGNAL(triggered()), this, SLOT(setDarkTheme()));
    connect(ui->action_LightMode, SIGNAL(triggered()), this, SLOT(setLightTheme()));
    connect(ui->actionHSV, SIGNAL(triggered()), this, SLOT(applyhsv()));
    connect(ui->actionRGB, SIGNAL(triggered()), this, SLOT(applyrgb()));
    connect(ui->actionSpectral, SIGNAL(triggered()), this, SLOT(applyspectral()));
    connect(ui->actionBlue_and_red, SIGNAL(triggered()), this, SLOT(applyblueandred()));
    connect(ui->actionRGB2, SIGNAL(triggered()), this, SLOT(applyRGB2()));
    connect(ui->actionThresholds, SIGNAL(triggered()), this, SLOT(applythresholds()));
    connect(ui->actionGreyscale, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->actionPrism, SIGNAL(triggered()), this, SLOT(applyprism()));
    connect(ui->demo, SIGNAL(clicked()), this, SLOT(demonstration()));
    connect(ui->thresholdButton, SIGNAL(clicked()), this, SLOT(Thresholding()));
    connect(ui->enterValue, SIGNAL(textChanged(QString)), this, SLOT(changeofValue()));

    //This will resize the pictures to fit the pictureLabel
    ui->picture->setScaledContents( true );

    QPixmap pixmap(path+"/Ressources/accueil.jpg");
    ui->picture->setPixmap(pixmap);

    ui->selectFileDir->setEnabled(false);
    ui->enterValue->setEnabled(false);
    ui->actionChange_Language->setEnabled(false);

    //setLightTheme();
    setDarkTheme();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importdir(){
    dirpath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "C:/", QFileDialog::ShowDirsOnly);
    if(QDir(dirpath).exists()){
        //files is a QStringList containing the names of the files in the directory selected
        files.clear();
        callInit(dirpath);

        //callInit created a directory.txt, to be copied in the QStringList files
        QFile dir(QString(path+"/Out/directory.txt"));
        dir.open(QIODevice::ReadOnly);
        QTextStream in(&dir);

        QString line = in.readLine();
        while(!line.isNull()){
            files.append(QString(line));
            line = in.readLine();
        }
        dir.close();

        //Parameters of the slider (size, value of increment...)
        ui->selectFileDir->setRange(0, files.length()-1);
        ui->selectFileDir->setTickInterval(1);
        ui->selectFileDir->setEnabled(true);
        ui->filenumber->setNum(1);
        ui->totalfiles->setText("/ "+QString::number(files.length()));

        ui->selectFileDir->setValue(0);
        int v = ui->selectFileDir->value();

        filename = files[0];
        filepath = dirpath+"/"+filename;

        ui->filepath->setText(filepath);
        ui->filename->setText(filename);

        QPixmap pixmap(path+"/Out/initial"+QString::number(v)+".jpg");
        picWidth = pixmap.width();
        picHeight = pixmap.height();
        ui->picture->setPixmap(pixmap);
        //To see the dicom header in a table
        displayHeader();

        //Now that there's a picture, we can allow the segmentations
        thereIsPicture = true;
        regionGrow = false;
        waterShed = false;
        thereIsSeed = false;
        Threshold = false;
        ui->enterValue->setEnabled(true);

        //To check if the picture is a scanner or a MRI
        CTorMR();
    }
}

void MainWindow::importdcm(){
    filepath = QFileDialog::getOpenFileName(this,tr("Select picture"),"C:/",tr("Dicom Files (*.dcm)"));
    if(QFile(filepath).exists()){
        //To get the name of the file selected
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
        Threshold = false;
        ui->enterValue->setEnabled(true);

        QPixmap pixmap(path+"/Out/initial0.jpg");
        picWidth = pixmap.width();
        picHeight = pixmap.height();
        ui->picture->setPixmap(pixmap);

        CTorMR();
    }
}

void MainWindow::goThroughFile(){
    //This function handles the movements of the slider
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

    thereIsPicture = true;
    regionGrow = false;
    waterShed = false;
    Threshold = false;
}

void MainWindow::reset(){
    //Cancel the segmentation
    QPixmap pixmap_accueil(path+"/Ressources/accueil.jpg");
    ui->picture->setPixmap(pixmap_accueil);

    if(thereIsPicture){
        if(ui->selectFileDir->isEnabled()){
            int v = ui->selectFileDir->value();
            QPixmap pixmap(path+"/Out/initial"+QString::number(v)+".jpg");
            ui->picture->setPixmap(pixmap);
        }
        else {
            QPixmap pixmap(path+"/Out/initial0.jpg");
            ui->picture->setPixmap(pixmap);
        }
    }
}

void MainWindow::regionGrowing(){
    ui->errors->clear();
    if(!regionGrow && thereIsPicture && thereIsSeed){
        callRegionGrow();
        //The region growing segmentation has been done => regionGrow = true
        regionGrow = true;
    }

    if(regionGrow){
        //If the operation has been done once and the seed has not been changed, we can simply reload the picture
        QPixmap pixmap(path+"/Out/regionGrow.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }

    if(!thereIsSeed){
        ui->errors->setText("Error : You have to select a seed\nClick on the picture");
    }
}

void MainWindow::waterShedSeg(){
    if(!waterShed && thereIsPicture){
        callWaterShed();
        //The watershed segmentation has been done => watershed = true
        waterShed = true;
    }

    if(waterShed){
        QPixmap pixmap(path+"/Out/waterShed.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }
}

void MainWindow::Thresholding(){
    ui->errors->clear();
    if(!ui->enterValue->text().isEmpty()){
        thereIsValue = true;
    }

    if(!Threshold && thereIsPicture && thereIsValue){
        callThreshold(ui->enterValue->text());
        QPixmap pixmap(path+"/Ressources/chargement.jpg");
        ui->picture->setPixmap(pixmap);
        //The thresholding segmentation has been done => threshold = true
        Threshold = true;
    }

    if(Threshold){
        QPixmap pixmap(path+"/Out/threshold.jpg");
        ui->picture->setPixmap(pixmap);
        ui->picture->show();
    }

    if(!thereIsValue){
        ui->errors->setText("Error : Select a threshold value");
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton && thereIsPicture){
        //mapFromParent gives the coordinates inside of the widget, mapFromGlobal gives the coordinates inside the window
        QPoint point = ui->picture->mapFromParent(event->pos());

        if(point.x()<=ui->picture->width() && point.x()>=0 && point.y()<=ui->picture->height()+20 && point.y()>=20){
            //There's a constant error of 20px for the y-axis, the +20 is there to correct it
            ui->x_pos->setNum(int(point.x()*picWidth/1024));
            ui->y_pos->setNum(int(point.y())*picHeight/1024-10);
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
    //deletes the content of the Out directory when the window is closed
    QDir dir(path+"/Out/");
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
    QWidget::closeEvent(event);
}

void MainWindow::displayHeader(){
    //Reads the content of the dicomheader.txt file and put it in the table
    ui->headerView->clear();
    callExtractHeader(filepath);

    QFile header(QString(path+"/Out/dicomheader.txt"));
    header.open(QIODevice::ReadWrite);
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

void MainWindow::save(){
    QString imagePath = QFileDialog::getSaveFileName(this,tr("Save Picture"), "C:/", tr("JPEG (*.jpg *.jpeg)"));
    ui->picture->grab().save(imagePath);
}

//Themes of the application
void MainWindow::setDarkTheme(){
    ui->errors->setStyleSheet("QLabel { color : red; }");

    QFile qssFile(path+"/Ressources/QTDark.qss");
    qssFile.open(QFile::ReadOnly);
    QString qss = QLatin1String(qssFile.readAll());
    this->setStyleSheet(qss);
}

void MainWindow::setLightTheme(){
    ui->errors->setStyleSheet("QLabel { color : red; }");

    this->setStyleSheet("");
}

//Colormaps
void MainWindow::applyhsv(){
    applyColormap("hsv");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

void MainWindow::applyrgb(){
    applyColormap("gist_rainbow");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

void MainWindow::applyspectral(){
    applyColormap("nipy_spectral");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

void MainWindow::applyblueandred(){
    applyColormap("seismic");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

void MainWindow::applyprism(){
    applyColormap("prism");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

void MainWindow::applythresholds(){
    applyColormap("tab10");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

void MainWindow::applyRGB2(){
    applyColormap("gist_ncar");
    QPixmap pixmap(path+"/Out/lsd.jpg");
    ui->picture->setPixmap(pixmap);
}

//Loads a demo dcm file
void MainWindow::demonstration(){
    filepath = path+"/Ressources/demo.dcm";
    ui->filepath->setText(filepath);
    ui->filename->setText("demo.dcm");
    callInit(filepath);
    displayHeader();

    thereIsPicture = true;
    regionGrow = false;
    waterShed = false;
    thereIsSeed = false;
    thereIsValue = false;
    Threshold = false;
    ui->enterValue->setEnabled(true);

    QPixmap pixmap(path+"/Out/initial0.jpg");
    picWidth = pixmap.width();
    picHeight = pixmap.height();
    ui->picture->setPixmap(pixmap);

    CTorMR();
}

void MainWindow::CTorMR(){
    //Uses the dicom header to know if the picture is a scanner or a MRI
    QTreeWidgetItem* a = ui->headerView->findItems(QString("Modality"), Qt::MatchFlag::MatchExactly, 0)[0];
    QString modality = a->text(1);
    if(modality == "CT"){
        ui->valueLimits->setText("Choose a value between -1000 and +3000");
        isCT = true;
    }

    if(modality =="MR"){
        ui->valueLimits->setText("Choose a value between 0 and +6000");
        isMR = true;
    }
}

void MainWindow::changeofValue(){
    //Makes sure that the threshold is relevant with the type of picture
    thereIsValue = true;
    Threshold = false;
    if(isCT){
        if(ui->enterValue->text().toInt()> 3000){
            ui->enterValue->clear();
            ui->enterValue->setText("3000");
        }
        if(ui->enterValue->text().toInt()< -1000){
            ui->enterValue->clear();
            ui->enterValue->setText("-1000");
        }
    }
    if(isMR){
        if(ui->enterValue->text().toInt()> 6000){
            ui->enterValue->clear();
            ui->enterValue->setText("6000");
        }
        if(ui->enterValue->text().toInt()<0){
            ui->enterValue->clear();
            ui->enterValue->setText("0");
        }
    }
}

//Calls of the python scripts
void MainWindow::callInit(QString pictures){
    //Converts to jpg and saves the picture to Out
    QString program = "python";
    QStringList args;
    args << path+"/init.py" << pictures << path;
    QProcess test;
    test.start(program, args);
    test.waitForFinished();
}

void MainWindow::callExtractHeader(QString picture){
    //Creates dicomheader.txt and directory.txt if a directory is selected
    QString program = "python";
    QStringList args;
    args << path+"/extractHeader.py" << picture << path;
    QProcess test;
    test.start(program, args);
    test.waitForFinished();
}

void MainWindow::callRegionGrow(){
    QString program = "python";
    QStringList args;
    args << path+"/regionGrow.py" << ui->x_pos->text() << ui->y_pos->text() << filepath << path;
    QProcess test;
    test.start(program, args);
    test.waitForFinished();
}

void MainWindow::callWaterShed(){
    QString program = "python";
    QStringList args;
    args << path+"/waterShed.py" << filepath << path;
    QProcess test;
    test.start(program, args);
    test.waitForFinished();
}

void MainWindow::applyColormap(QString colormap){
    QString program = "python";
    QStringList args;
    args << path+"/LinearSegmentationDistribution.py" << filepath << path << colormap;
    QProcess test;
    test.start(program, args);
    test.waitForFinished();
}

void MainWindow::callThreshold(QString threshold){
    QString program = "python";
    QStringList args;
    args << path+"/threshold.py" << filepath << path << threshold;
    QProcess test;
    test.start(program, args);
    test.waitForFinished();
}
