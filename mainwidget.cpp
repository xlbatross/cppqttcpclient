#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , client(new WTCPClient)
//    , client(new LTCPClient)
    , receiveThread(new ReceiveThread(this->client))
    , myRoomMemberCount(-1)
{
    ui->setupUi(this);

    this->timer = new QTimer(this);
    this->label = new OpenCVImageLabel(this);

    if (this->client->connectServer())
    {
        qDebug() << "connected";
        connect(this->timer, SIGNAL(timeout()), this, SLOT(readCapture()));
        connect(this->receiveThread, SIGNAL(disconnectServerSignal()), this, SLOT(disconnectServer()));
        connect(this->receiveThread, SIGNAL(resImageSignal(ResImage*)), this, SLOT(responseImage(ResImage*)), Qt::BlockingQueuedConnection);
        connect(this->receiveThread, SIGNAL(resRoomListSignal(ResRoomList*)), this, SLOT(responseRoomList(ResRoomList*)));
        connect(this->receiveThread, SIGNAL(resMakeRoomSignal(ResMakeRoom*)), this, SLOT(responseMakeRoom(ResMakeRoom*)));
        connect(this->receiveThread, SIGNAL(resEnterRoomSignal(ResEnterRoom*)), this, SLOT(responseEnterRoom(ResEnterRoom*)));
        connect(this->receiveThread, SIGNAL(resJoinRoomSignal(ResJoinRoom*)), this, SLOT(responseJoinRoom(ResJoinRoom*)));
        connect(this->receiveThread, SIGNAL(resDisjoinRoomSignal(ResDisjoinRoom*)), this, SLOT(responseDisjoinRoom(ResDisjoinRoom*)));
        connect(ui->btn_makeRoom, SIGNAL(clicked(bool)), this, SLOT(viewMakeRoomMessageBox()));
        connect(ui->lw_roomList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enterRoom(QListWidgetItem*)));
        connect(ui->btn_backFromStudent, SIGNAL(clicked(bool)), this, SLOT(backClicked()));
        connect(ui->btn_endLecture, SIGNAL(clicked(bool)), this, SLOT(backClicked()));
        connect(ui->btn_refreshList, SIGNAL(clicked(bool)), this, SLOT(refeashRoomList()));
        this->receiveThread->start();
        this->client->sendReqRoomList();
    }
    else
    {
        this->disconnectServer();
    }
}

MainWidget::~MainWidget()
{
    delete timer;
    delete label;
    delete client;
    this->cap.release();
    delete ui;
}

// slots
void MainWidget::readCapture()
{
    if (this->cap.isOpened())
    {
        this->cap.read(img);
        cv::resize(img, img, cv::Size(320, 240));
        this->client->sendReqImage(img);
    }
    else
    {
        this->cap.open(0);
        this->cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
        this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
    }
}

void MainWidget::viewMakeRoomMessageBox()
{
    QMessageBox msgBox;
    int ret;
    msgBox.setText("방을 만드시겠습니까?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    ret = msgBox.exec();
    if (ret == QMessageBox::Ok)
        this->client->sendReqMakeRoom("안녕하세요?");
}

void MainWidget::enterRoom(QListWidgetItem * item)
{
    if (this->ipList.size() > 0)
    {
        int row = ui->lw_roomList->row(item);
        this->client->sendReqEnterRoom(this->ipList[row], this->portList[row]);
    }
}

void MainWidget::backClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    this->myRoomMemberCount = -1;
    this->timer->stop();
    this->client->sendReqLeaveRoom();
    this->client->sendReqRoomList();
}

void MainWidget::refeashRoomList()
{
    this->client->sendReqRoomList();
}

void MainWidget::disconnectServer()
{
    QMessageBox msgBox;
    msgBox.setText("서버와의 접속을 유지할 수 없습니다.");
    msgBox.exec();
    this->deleteLater();
}

void MainWidget::responseImage(ResImage * resImage)
{
    QLabel * current = NULL;

    std::cout << resImage->number() << std::endl;

    switch(resImage->number())
    {
    case 0:
        current = ui->lb_proImage;
        break;
    case 1:
        current = ui->lb_stuImage1;
        break;
    case 2:
        current = ui->lb_stuImage2;
        break;
    case 3:
        current = ui->lb_stuImage3;
        break;
    case 4:
        current = ui->lb_stuImage4;
        break;
    }

    if (current != NULL)
    {
        QImage qtImage((const unsigned char *) (resImage->img().data), resImage->img().cols, resImage->img().rows, QImage::Format_RGB888);
        current->setPixmap(QPixmap::fromImage(qtImage));
    }
}

void MainWidget::responseRoomList(ResRoomList * resRoomList)
{
    ipList = resRoomList->ipList();
    portList = resRoomList->portList();
    roomNameList = resRoomList->roomNameList();
    roomMemberCountList = resRoomList->roomMemberCountList();

    ui->lw_roomList->clear();
    if (ipList.size() == 0)
    {
        ui->lw_roomList->addItem("방이 없습니다.");
    }
    else
    {
        for (int i = 0; i < ipList.size(); i++)
        {
            ui->lw_roomList->addItem(
                QString::fromUtf8(roomNameList.at(i).c_str(), roomNameList.at(i).size())
              + " " + QString::fromUtf8(ipList.at(i).c_str(), ipList.at(i).size())
              + " " + QString::number(portList[i])
            );
//            ui->lw_roomList->addItem(
//                QString::fromUtf8(resRoomList->roomNameList().at(i).c_str(), resRoomList->roomNameList().at(i).size())
//              + " " + QString::fromUtf8(resRoomList->ipList().at(i).c_str(), resRoomList->ipList().at(i).size())
//              + " " + QString::number(resRoomList->portList()[i])
//            );
        }
    }
}

void MainWidget::responseMakeRoom(ResMakeRoom * resMakeRoom)
{
    if (resMakeRoom->isMake())
    {
        ui->tb_chatPro->clear();
        this->myRoomMemberCount = 0;
        ui->lb_stuImage1->setVisible(false);
        ui->lb_stuImage2->setVisible(false);
        ui->lb_stuImage3->setVisible(false);
        ui->lb_stuImage4->setVisible(false);
        ui->stackedWidget->setCurrentIndex(4);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("방을 생성하지 못하였습니다.");
        msgBox.exec();
        this->client->sendReqRoomList();
    }
}

void MainWidget::responseEnterRoom(ResEnterRoom * resEnterRoom)
{
    if (resEnterRoom->isEnter())
    {
        ui->tb_chatStu->clear();
        ui->stackedWidget->setCurrentIndex(3);
        this->timer->start(33);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("방에 참가하지 못했습니다.");
        msgBox.exec();
        this->client->sendReqRoomList();
    }
}

void MainWidget::responseJoinRoom(ResJoinRoom * resJoinRoom)
{
    QString appear = QString::fromUtf8(resJoinRoom->name().c_str(), resJoinRoom->name().size()) + "님이 등장하셨습니다.";
    if (ui->stackedWidget->currentIndex() == 4)
    {
        ui->tb_chatPro->append(appear);
        this->myRoomMemberCount += 1;
        switch(this->myRoomMemberCount)
        {
        case 1:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(false);
            ui->lb_stuImage3->setVisible(false);
            ui->lb_stuImage4->setVisible(false);
            break;
        case 2:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(true);
            ui->lb_stuImage3->setVisible(false);
            ui->lb_stuImage4->setVisible(false);
            break;
        case 3:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(true);
            ui->lb_stuImage3->setVisible(true);
            ui->lb_stuImage4->setVisible(false);
            break;
        case 4:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(true);
            ui->lb_stuImage3->setVisible(true);
            ui->lb_stuImage4->setVisible(false);
            break;
        }
        if (this->myRoomMemberCount > 0)
            this->timer->start(33);
    }
    else if (ui->stackedWidget->currentIndex() == 3)
    {
        ui->tb_chatStu->append(appear);
    }
}

void MainWidget::responseDisjoinRoom(ResDisjoinRoom * resDisjoinRoom)
{
    QString disappear = QString::fromUtf8(resDisjoinRoom->name().c_str(), resDisjoinRoom->name().size()) + "님이 퇴장하셨습니다.";
    if (resDisjoinRoom->isProfessorOut())
    {
        this->client->sendReqRoomList();
        ui->stackedWidget->setCurrentIndex(2);
        this->timer->stop();
        QMessageBox msgBox;
        msgBox.setText("강의가 종료되었습니다.");
        msgBox.exec();
    }
    else if (ui->stackedWidget->currentIndex() == 4)
    {
        ui->tb_chatPro->append(disappear);
        this->myRoomMemberCount -= 1;
        switch(this->myRoomMemberCount)
        {
        case 0:
            ui->lb_stuImage1->setVisible(false);
            ui->lb_stuImage2->setVisible(false);
            ui->lb_stuImage3->setVisible(false);
            ui->lb_stuImage4->setVisible(false);
            break;
        case 1:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(false);
            ui->lb_stuImage3->setVisible(false);
            ui->lb_stuImage4->setVisible(false);
            break;
        case 2:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(true);
            ui->lb_stuImage3->setVisible(false);
            ui->lb_stuImage4->setVisible(false);
            break;
        case 3:
            ui->lb_stuImage1->setVisible(true);
            ui->lb_stuImage2->setVisible(true);
            ui->lb_stuImage3->setVisible(true);
            ui->lb_stuImage4->setVisible(false);
            break;
        }
        if (this->myRoomMemberCount == 0)
            this->timer->stop();
    }
    else if (ui->stackedWidget->currentIndex() == 3)
    {
        ui->tb_chatStu->append(disappear);
    }
}






