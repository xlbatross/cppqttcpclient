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
//        this->timer->start(33);
        this->receiveThread->start();
        this->client->sendReqRoomList();

        connect(ui->btnMakeRoom, SIGNAL(clicked(bool)), this, SLOT(viewMakeRoomMessageBox()));
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
    ui->listWidgetRoomList->clear();
//    if (resRoomList-> == 0)
//    {
//        ui->listWidgetRoomList->addItem("방이 없습니다.");
//    }
//    else
//    {
//        int memoryLoc = 0;
//        for (int i = 0; i < receiveHeader->dataCount(); i++)
//        {
//            QString ip;
//            int port;
//            QString roomName;
//            int roomMemberCount;
//            memoryLoc = 0;
//            ip = QString::fromUtf8(receiveDataList[i], receiveHeader->attr()[4 * i]);
//            memoryLoc += receiveHeader->attr()[4 * i];
//            memcpy(&port, receiveDataList[i] + memoryLoc, receiveHeader->attr()[4 * i + 1]);
//            memoryLoc += receiveHeader->attr()[4 * i + 1];
//            roomName = QString::fromUtf8(receiveDataList[i] + memoryLoc, receiveHeader->attr()[4 * i + 2]);
//            memoryLoc += receiveHeader->attr()[4 * i + 2];
//            memcpy(&roomMemberCount, receiveDataList[i] + memoryLoc, receiveHeader->attr()[4 * i + 3]);
//            memoryLoc += receiveHeader->attr()[4 * i + 3];
//            qDebug() << ip;
//            std::cout << port << std::endl;
//            ui->listWidgetRoomList->addItem(roomName + " (" + QString::number(roomMemberCount) + "/4)");
//            std::cout << roomMemberCount << std::endl;
//            std::cout << std::endl;

//        }
//    }
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





