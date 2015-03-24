#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cv.h>
#include <highgui.h>
 #include <QMessageBox>

char* image;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    cvWaitKey(0); // se pulsa tecla para terminar
    //cvReleaseImage(&imagenBW);
    cvDestroyAllWindows(); // destruimos todas las ventanas
}

void MainWindow::on_loadImage_clicked()
{
    this->files = QFileDialog::getOpenFileNames(this,tr("Open Image"),"/home/snipercat/Deskto",tr("Image Files(*.png *.jpg *.bmp)"));

    const char* image = files.at(0).toStdString().data();
    imagenBW = cvLoadImage(image,1);// cargamos la imagen en Blanco y negros
    ui->tabWidget->setEnabled(true);

    cvShowImage( "IMAGEN Original", imagenBW ); // representamos la imagen en la ventana
}

void MainWindow::on_reduce_clicked()
{
    //IplImage *img1 = cvLoadImage(files.at(0).toStdString().data(),0);// cargamos la imagen en Blanco y negros

    IplImage *img2 = cvCreateImage( cvSize(imagenBW->width/2,imagenBW->height/2),imagenBW->depth,imagenBW->nChannels);


    //cvResize(imagenBW,img2,CV_INTER_NN);
    cvResize(imagenBW,img2, CV_INTER_LINEAR);

    cvShowImage( "IMAGEN Reducida", img2 ); // representamos la imagen en la ventana
    cvWaitKey(0); // se pulsa tecla para terminar

    cvReleaseImage(&img2);
    //cvDestroyAllWindows(); // destruimos todas las ventanas
}

void MainWindow::on_extend_clicked()
{
    //IplImage *img1 = cvLoadImage(files.at(0).toStdString().data(),0);// cargamos la imagen en Blanco y negros

    IplImage *img2 = cvCreateImage( cvSize(imagenBW->width*2,imagenBW->height*2),imagenBW->depth,imagenBW->nChannels);
    cvResize(imagenBW,img2,CV_INTER_NN);

    cvShowImage( "IMAGEN Ampliada", img2 ); // representamos la imagen en la ventana
    cvWaitKey(0); // se pulsa tecla para terminar

    cvReleaseImage(&img2);
    //cvDestroyAllWindows(); // destruimos todas las ventanas
}

/**
 * @brief MainWindow::on_ColorSpace_clicked
 * Abrira una ventana con la imagen transformada en el espacio de color seleccionado
 */
void MainWindow::on_ColorSpace_clicked()
{

    const char* ColorFormat; //Titulo de la ventana
    int index = ui->colorFormat->currentIndex(); // Indice del combobox seleccionado
    int transform; //ID de la transformacion que se usara
    int colorChannels; //Canales de la imagen destino (varian segub el espacio de color)

    //Se agregan valores a las variables segun el espacio seleccionado
    switch(index)
    {
        case 0:
            ColorFormat = "Gray";
            transform = CV_RGB2GRAY;
            colorChannels = 1;
            break;
        case 1:
            ColorFormat = "XYZ";
            transform = CV_RGB2XYZ;
            colorChannels = 3;
            break;
        case 2:
            ColorFormat = "HSV";
            transform = CV_RGB2HSV;
            colorChannels = 3;
            break;
        case 3:
            ColorFormat = "HLS";
            transform = CV_RGB2HLS;
            colorChannels = 3;
            break;
        case 4:
            ColorFormat = "Luv";
            transform = CV_RGB2Luv;
            colorChannels = 3;
            break;
        case 5:
            ColorFormat = "YCrCb";
            transform = CV_RGB2YCrCb;
            colorChannels = 3;
            break;
        default:
            ColorFormat = "BGR";
            transform = CV_RGB2BGR;
            colorChannels = 3;
            break;
    }

    //Se crea la imagen y se realiza la transformacion.
    IplImage *imgColor = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,colorChannels);
    cvCvtColor(imagenBW, imgColor, transform );
    cvShowImage( ColorFormat , imgColor );

}


/**
 * @brief MainWindow::on_umbralizar_clicked
 * Abrira una ventana con la imagen Umbralizada con las opciones seleccionadas.
 */
void MainWindow::on_umbralizar_clicked()
{
    const char* windowTittle;// = ui->colorFormat->currentText().toStdString().c_str();
    int index = ui->thresholdType->currentIndex();


    double thresholdValue= ui->thresholdValue->value();
    double thresholdMaxValue = ui->thresholdMaxValue->value();

    int thresholdType;

    switch(index)
    {
        case 0:
            windowTittle = "CV-THRESH-BINARY";
            thresholdType = CV_THRESH_BINARY;
            break;
        case 1:
            windowTittle = "CV_THRESH_BINARY_INV";
            thresholdType = CV_THRESH_BINARY_INV;
            break;
        case 2:
            windowTittle = "CV_THRESH_TRUNC";
            thresholdType = CV_THRESH_TRUNC;
            break;
        case 3:
            windowTittle = "CV_THRESH_TOZERO_INV";
            thresholdType = CV_THRESH_TOZERO_INV;
            break;
        case 4:
            windowTittle = "CV_THRESH_TOZERO";
            thresholdType = CV_THRESH_TOZERO;
            break;
        default:
            windowTittle = "CV-THRESH-BINARY";
            thresholdType = CV_THRESH_BINARY;
    }

    IplImage *imgThreshold;

    //Si está seleccionado el checkBox de Blanco y negro, se hace un cambio de espacio de color antes
    // de realizar la Umbralización
    if( ui->thresholdBW->isChecked() ){
        IplImage *temp = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvCvtColor(imagenBW, temp, CV_RGB2GRAY );

        imgThreshold = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvThreshold(temp,imgThreshold, thresholdValue, thresholdMaxValue, thresholdType);
    }
    else{
        imgThreshold = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,imagenBW->nChannels);
        cvThreshold(imagenBW,imgThreshold, thresholdValue, thresholdMaxValue, thresholdType);
    }

    cvShowImage( windowTittle , imgThreshold );

}


void MainWindow::on_umbralAutomatico_clicked()
{
    const char* windowTittle;// = ui->colorFormat->currentText().toStdString().c_str();
    int index = ui->thresholdType->currentIndex();
    int indexADP = ui->automaticThresholdType->currentIndex();

    double thresholdMaxValue = ui->thresholdMaxValue->value();

    int thresholdType;
    int ADPType;

    switch(index)
    {
        case 0:
            windowTittle = "CV-THRESH-BINARY";
            thresholdType = CV_THRESH_BINARY;
            break;
        case 1:
            windowTittle = "CV_THRESH_BINARY_INV";
            thresholdType = CV_THRESH_BINARY_INV;
            break;
        default:
            QMessageBox messageBox;
            messageBox.critical(0,"Error","La Umbralizacion automatica solo puede ser usada con umbralizaciones de tipo binaria");
            messageBox.setFixedSize(500,200);
            return;
            break;
    }

    switch(indexADP)
    {
        case 0:
            windowTittle =  "CV_ADAPTIVE_THRESH_MEAN_C";
            ADPType = CV_ADAPTIVE_THRESH_MEAN_C;
            break;
        default:
            windowTittle = "CV_ADAPTIVE_THRESH_GAUSSIAN_C";
            ADPType = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
    }

    IplImage *imgThreshold;

    //Solo se puede ralizar con imagenes en blanco y negro
        IplImage *temp = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvCvtColor(imagenBW, temp, CV_RGB2GRAY );

        imgThreshold = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvAdaptiveThreshold(temp, imgThreshold, thresholdMaxValue, ADPType, thresholdType);

    cvShowImage( windowTittle , imgThreshold );
}



void MainWindow::on_experiment1_clicked()
{
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

    IplImage *imgThresholdBW; // Convierte a gris primero
    IplImage *imgThresholdTH;  // Umbraliza primero

    // Convierte a Gris primero
        IplImage *temp = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvCvtColor(imagenBW, temp, CV_RGB2GRAY );
        imgThresholdBW = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvThreshold(temp,imgThresholdBW, thresholdValue, thresholdMaxValue, thresholdType);
    //Umbraiza primero
        IplImage *temp2 = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,imagenBW->nChannels);
        cvThreshold(imagenBW,temp2, thresholdValue, thresholdMaxValue, thresholdType);

        imgThresholdTH = cvCreateImage( cvSize(imagenBW->width,imagenBW->height),imagenBW->depth,1);
        cvCvtColor(temp2, imgThresholdTH, CV_RGB2GRAY );

    cvShowImage( "Gris Primero" , imgThresholdBW );
    cvShowImage( "Umbral Primero" , imgThresholdTH );
}


