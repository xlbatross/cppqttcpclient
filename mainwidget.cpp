#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , client(new Client(this))
{
    ui->setupUi(this);

    this->timer = new QTimer(this);
    this->label = new OpenCVImageLabel(this);

    if (this->client->connectToHost())
    {
        qDebug() << "connected";

        connect(this->timer, SIGNAL(timeout()), this, SLOT(readCapture()));
        connect(this, SIGNAL(setOpenCVImageSignal(cv::Mat)), this->label, SLOT(setOpenCVImage(cv::Mat)));
        connect(this, SIGNAL(sendImageSignal(cv::Mat)), this, SLOT(sendImage(cv::Mat)));

        this->timer->start(30);
    }
    else
    {
        qDebug() << "not connected";
        this->deleteLater();
    }
}

MainWidget::~MainWidget()
{
    this->cap.release();
    delete ui;
}

// slots
void MainWidget::readCapture()
{
    if (this->cap.isOpened())
    {
        this->cap.read(img);
        emit setOpenCVImageSignal(img);
        emit sendImageSignal(img);
    }
    else
    {
        this->cap.open(0);
    }
}

void MainWidget::sendImage(cv::Mat const & image)
{
    this->client->send(image);
}






