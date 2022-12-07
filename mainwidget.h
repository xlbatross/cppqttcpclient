#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include "opencvimagelabel.h"
#include "client.h"

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
    Client * client;
    Ui::MainWidget *ui;
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
