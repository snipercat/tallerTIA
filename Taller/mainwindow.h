#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>
#include <QFileDialog>
#include <cv.h>
#include <highgui.h>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadImage_clicked();

    void on_findContours_clicked();
    IplImage * umbralNormal( bool gris );
    IplImage * umbralAutomatico( );
    CvSeq    * findContours( IplImage *imgThreshold );

private:
    Ui::MainWindow *ui;
    QStringList files;
    IplImage *imagenBW;
};

#endif // MAINWINDOW_H
