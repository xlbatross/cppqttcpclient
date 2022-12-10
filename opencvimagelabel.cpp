#include "opencvimagelabel.h"
#include <QImage>
#include <QPixmap>

OpenCVImageLabel::OpenCVImageLabel(QWidget * parent)
    : QLabel(parent)
{

}

OpenCVImageLabel::~OpenCVImageLabel()
{

}

// slots
void OpenCVImageLabel::setOpenCVImage(const cv::Mat &img)
{
    this->img = img.clone();
    cv::cvtColor(this->img, this->img, cv::COLOR_BGR2RGB);
    this->qtImage = QImage((const unsigned char *) (this->img.data), this->img.cols, this->img.rows, QImage::Format_RGB888);

    this->setPixmap(QPixmap::fromImage(this->qtImage));
    this->resize(this->pixmap()->size());
}

void OpenCVImageLabel::setOpenCVImage(const char * const * data, const int height, const int width, const int channels)
{
//    this->img = cv::Mat(height, width, CV_8UC3, (unsigned char *) data);
//    cv::cvtColor(this->img, this->img, cv::COLOR_BGR2RGB);
    this->qtImage = QImage((const unsigned char *) (data[0]), width, height, QImage::Format_BGR888);
    this->setPixmap(QPixmap::fromImage(this->qtImage));
    this->resize(this->pixmap()->size());

}
