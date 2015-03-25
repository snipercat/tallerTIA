#include "mainwindow.h"
#include "ui_mainwindow.h"
 #include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImage_clicked()
{
    this->files = QFileDialog::getOpenFileNames(this,tr("Open Image"),"/home/snipercat/Desktop",tr("Image Files(*.png *.jpg *.bmp)"));

    const char* image = files.at(0).toStdString().data();
    imagenBW = cvLoadImage(image,1);// cargamos la imagen en Blanco y negros
    ui->tabWidget->setEnabled(true);

    cvShowImage( "IMAGEN Original", imagenBW ); // representamos la imagen en la ventana
}

void MainWindow::on_findContours_clicked(){
    int thresholdType = ui->contourThresholdType->currentIndex();
        IplImage *imgThreshold;
        IplImage *imgcontours = cvCreateImage( cvGetSize( imagenBW ), 8, 1 );

      //Obtener imagen umbralizada
        if(thresholdType == 0)
            imgThreshold = umbralNormal(true);
        else{
            imgThreshold = umbralAutomatico ();
            if(imgThreshold == NULL)
                return;
        }
        cvShowImage( "Umbralizada" , imgThreshold );

        CvSeq* contours = findContours(imgThreshold);

        if( contours ){
                cvDrawContours(
                    imgcontours,
                    contours,
                    cvScalarAll(255),
                    cvScalarAll(255),
                    100 );
            }
        cvShowImage( "Contours", imgcontours );
}











IplImage * MainWindow::umbralNormal(bool gris){


    int index = ui->thresholdType->currentIndex();


    double thresholdValue= ui->thresholdValue->value();
    double thresholdMaxValue = ui->thresholdMaxValue->value();

    int thresholdType;

    switch(index)
    {
        case 0:
            thresholdType = CV_THRESH_BINARY;
            break;
        case 1:
            thresholdType = CV_THRESH_BINARY_INV;
            break;
        case 2:
            thresholdType = CV_THRESH_TRUNC;
            break;
        case 3:
            thresholdType = CV_THRESH_TOZERO_INV;
            break;
        case 4:
            thresholdType = CV_THRESH_TOZERO;
            break;
        default:
            thresholdType = CV_THRESH_BINARY;
    }

    IplImage *imgThreshold;

    //Si está seleccionado el checkBox de Blanco y negro, se hace un cambio de espacio de color antes
    // de realizar la Umbralización
    if( gris ){
        IplImage *temp = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvCvtColor(imagenBW, temp, CV_RGB2GRAY );

        imgThreshold = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvThreshold(temp,imgThreshold, thresholdValue, thresholdMaxValue, thresholdType);
    }
    else{
        imgThreshold = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,imagenBW->nChannels);
        cvThreshold(imagenBW,imgThreshold, thresholdValue, thresholdMaxValue, thresholdType);
    }

    //cvShowImage( windowTittle , imgThreshold );
    return imgThreshold;

}

IplImage * MainWindow::umbralAutomatico(){

    int index = ui->thresholdType->currentIndex();
    int indexADP = ui->automaticThresholdType->currentIndex();

    double thresholdMaxValue = ui->thresholdMaxValue->value();

    int thresholdType;
    int ADPType;

    switch(index)
    {
        case 0:
            thresholdType = CV_THRESH_BINARY;
            break;
        case 1:
            thresholdType = CV_THRESH_BINARY_INV;
            break;
        default:
            QMessageBox messageBox;
            messageBox.critical(0,"Error","La Umbralizacion automatica solo puede ser usada con umbralizaciones de tipo binaria");
            messageBox.setFixedSize(500,200);
            return NULL;
            break;
    }

    switch(indexADP)
    {
        case 0:
            ADPType = CV_ADAPTIVE_THRESH_MEAN_C;
            break;
        default:
            ADPType = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
    }

    IplImage *imgThreshold;

    //Solo se puede ralizar con imagenes en blanco y negro
        IplImage *temp = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvCvtColor(imagenBW, temp, CV_RGB2GRAY );

        imgThreshold = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvAdaptiveThreshold(temp, imgThreshold, thresholdMaxValue, ADPType, thresholdType);

    //cvShowImage( windowTittle , imgThreshold );
      return imgThreshold;
}

CvSeq    * MainWindow::findContours( IplImage *imgThreshold ){
    CvSeq* contours = 0;
    CvMemStorage* 	g_storage = NULL;
    g_storage = cvCreateMemStorage(0);
    cvFindContours( imgThreshold, g_storage, &contours );
    return contours;
}
