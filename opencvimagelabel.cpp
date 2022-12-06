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
