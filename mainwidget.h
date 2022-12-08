#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include "opencvimagelabel.h"
#include "client.h"
#include "dataheader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

    DataHeader * dataHeader;
    Client * client;

    cv::VideoCapture cap;
    cv::Mat img;

    QTimer * timer;
    OpenCVImageLabel * label;

private slots:
    void readCapture();
    void sendImage(cv::Mat const &);

signals:
    void setOpenCVImageSignal(cv::Mat);
    void sendImageSignal(cv::Mat);
};
#endif // MAINWIDGET_H
