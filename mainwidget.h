#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include "opencvimagelabel.h"
//#include "wtcpclient.h"
#include "ltcpclient.h"
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

    void setOpenCVImage(QLabel * label, const cv::Mat & img);

private:
    Ui::MainWidget *ui;

//    WTCPClient * client;
    LTCPClient * client;

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

    // page ########
    int nowPage;

private slots:
    void readCapture();
    void viewMakeRoomMessageBox();
    void enterRoom(QListWidgetItem *);
    void backClicked();
    void refeashRoomList();
    void Login();
    void SignUp();
    void sendChat(); //####
    void gotoSignUp(); //###회원가입페이지로 이동
    void goToLogin(); //###로그인페이지로 이동
    void goToRoomList();//###강의룸리스트로 이동
    void gotoMyRoom();
    void gotoOtherRoom();
    void closeLecture();//###강의 종료

    void disconnectServer();
    void responseImage(ResImage *);
    void responseRoomList(ResRoomList *);
    void responseMakeRoom(ResMakeRoom *);
    void responseEnterRoom(ResEnterRoom *);
    void responseJoinRoom(ResJoinRoom *);
    void responseDisjoinRoom(ResDisjoinRoom *);
    void responseLogin(ResLogin *);
    void responseSignUp(ResSignUp *); //####
    void responseChat(ResChat *); //####
};
#endif // MAINWIDGET_H
