#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include "opencvimagelabel.h"
#include "wtcpclient.h"
//#include "ltcpclient.h"
#include "receivethread.h"
#include "resheader.h"

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

    WTCPClient * client;
//    LTCPClient * client;

    ReceiveThread * receiveThread;

    cv::VideoCapture cap;
    cv::Mat img;

    QTimer * timer;
    OpenCVImageLabel * label;

private slots:
    void disconnectServer();
    void readCapture();
    void sendImage(cv::Mat const &);
    void responseRoomList(ResRoomList *);
    void viewMakeRoomMessageBox();
    void responseMakeRoom(ResMakeRoom *);

signals:
    void setOpenCVImageSignal(cv::Mat);
    void sendImageSignal(cv::Mat);
};
#endif // MAINWIDGET_H
