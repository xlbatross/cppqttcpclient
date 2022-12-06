#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    while (!this->cap.isOpened())
        this->cap.open(0);

    this->timer = new QTimer(this);
    this->label = new OpenCVImageLabel(this);

    connect(this->timer, SIGNAL(timeout()), this, SLOT(readCapture()));
    connect(this, SIGNAL(setOpenCVImage(cv::Mat)), this->label, SLOT(setOpenCVImage(cv::Mat)));

    this->timer->start(30);
}

MainWidget::~MainWidget()
{
    this->cap.release();
    delete ui;
}

void MainWidget::readCapture()
{
    if (this->cap.isOpened())
    {
        this->cap.read(img);
        emit setOpenCVImage(img);
    }
    else
    {
        this->cap.open(0);
    }
}



