#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include "opencvimagelabel.h"
#include "wtcpclient.h"
//#include "ltcpclient.h"
#include "receivethread.h"
#include "response.h"

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

    // roomList
    std::vector<std::string> ipList;
    std::vector<int> portList;
    std::vector<std::string> roomNameList;
    std::vector<int> roomMemberCountList;

    // roomCount
    int myRoomMemberCount;

private slots:

    void readCapture();
    void viewMakeRoomMessageBox();
    void enterRoom(QListWidgetItem *);
    void backClicked();
    void refeashRoomList();
    void Login();
    void SignUp();

    void disconnectServer();
    void responseImage(ResImage *);
    void responseRoomList(ResRoomList *);
    void responseMakeRoom(ResMakeRoom *);
    void responseEnterRoom(ResEnterRoom *);
    void responseJoinRoom(ResJoinRoom *);
    void responseDisjoinRoom(ResDisjoinRoom *);
    void responseLogin(ResLogin *);

signals:
    void setOpenCVImageSignal(cv::Mat);
};
#endif // MAINWIDGET_H
