#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , client(new WTCPClient)
//    , client(new LTCPClient)
    , receiveThread(new ReceiveThread(this->client))
{
    ui->setupUi(this);

    this->timer = new QTimer(this);
    this->label = new OpenCVImageLabel(this);

    if (this->client->connectServer())
    {
        qDebug() << "connected";
//        connect(this->timer, SIGNAL(timeout()), this, SLOT(readCapture()));
//        connect(this->receiveThread, SIGNAL(viewImageSignal(DataHeader*,const char*const*)), this->label, SLOT(setOpenCVImage()), Qt::BlockingQueuedConnection);
//        connect(this, SIGNAL(sendImageSignal(cv::Mat)), this, SLOT(sendImage(cv::Mat)));
        connect(this->receiveThread, SIGNAL(disconnectServerSignal()), this, SLOT(disconnectServer()));
        connect(this->receiveThread, SIGNAL(resRoomListSignal(ResRoomList*)), this, SLOT(responseRoomList(ResRoomList*)));
        connect(this->receiveThread, SIGNAL(resMakeRoomSignal(ResMakeRoom *)), this, SLOT(responseMakeRoom(ResMakeRoom *)));
        connect(ui->btn_makeRoom, SIGNAL(clicked(bool)), this, SLOT(viewMakeRoomMessageBox()));
        connect(ui->lw_roomList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enterRoom(QListWidgetItem*)));

        this->receiveThread->start();
//        this->timer->start(33);
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
void MainWidget::disconnectServer()
{
    QMessageBox msgBox;
    msgBox.setText("서버와의 접속을 유지할 수 없습니다.");
    msgBox.exec();
    this->deleteLater();
}


void MainWidget::readCapture()
{
    if (this->cap.isOpened())
    {
        this->cap.read(img);
        cv::resize(img, img, cv::Size(320, 240));
        emit sendImageSignal(img);
    }
    else
    {
        this->cap.open(0);
        this->cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
        this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
    }
}

void MainWidget::sendImage(cv::Mat const & image)
{
//    this->client->sendReqImage(image);
}

void MainWidget::responseRoomList(ResRoomList * resRoomList)
{
    ipList = resRoomList->ipList();
    portList = resRoomList->portList();
    roomNameList = resRoomList->roomNameList();
    roomMemberCountList = resRoomList->roomMemberCountList();

    ui->lw_roomList->clear();
//    if (ipList.size() == 0)
    if (ipList.size() == 0)
    {
        ui->lw_roomList->addItem("방이 없습니다.");
    }
    else
    {
        for (int i = 0; i < ipList.size(); i++)
//        for (int i = 0; i < resRoomList->ipList().size(); i++)
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

void MainWidget::responseMakeRoom(ResMakeRoom * resMakeRoom)
{
    QMessageBox msgBox;
    if (resMakeRoom->isMake())
        msgBox.setText("방이 생성되었습니다.");
    else
        msgBox.setText("방을 생성하지 못하였습니다.");
    msgBox.exec();

    if (resMakeRoom->isMake())
        this->client->sendReqRoomList();
//        ui->stackedWidget->setCurrentIndex(1);
}

void MainWidget::enterRoom(QListWidgetItem * item)
{
    if (this->ipList.size() > 0)
    {
        int row = ui->lw_roomList->row(item);
        this->client->sendReqEnterRoom(this->ipList[row], this->portList[row]);
    }
}






