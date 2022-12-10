#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , client(new WTCPClient)
    , receiveThread(new ReceiveThread(this->client))
{
    ui->setupUi(this);

    this->timer = new QTimer(this);
    this->label = new OpenCVImageLabel(this);

    if (this->client->connectServer())
    {
        qDebug() << "connected";

        connect(this->timer, SIGNAL(timeout()), this, SLOT(readCapture()));
        connect(this->receiveThread, SIGNAL(viewImageSignal(const char* const *,long,long,long)), this->label, SLOT(setOpenCVImage(const char* const *,long,long,long)), Qt::BlockingQueuedConnection);
        connect(this, SIGNAL(sendImageSignal(cv::Mat)), this, SLOT(sendImage(cv::Mat)));

        this->timer->start(33);
        this->receiveThread->start();
    }
    else
    {
        qDebug() << "not connected";
        this->deleteLater();
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
    this->client->sendData(image);
}






