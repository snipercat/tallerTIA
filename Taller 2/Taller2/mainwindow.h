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

    void on_reduce_clicked();

    void on_extend_clicked();

    void on_ColorSpace_clicked();

    void on_colorFormat_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_umbralizar_clicked();

    void on_experiment1_clicked();

    void on_umbralAutomatico_clicked();

private:
    Ui::MainWindow *ui;
    QStringList files;
    IplImage *imagenBW;


};

#endif // MAINWINDOW_H
