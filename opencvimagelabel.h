#ifndef OPENCVIMAGELABEL_H
#define OPENCVIMAGELABEL_H

#include <QLabel>
#include <opencv2/opencv.hpp>

class OpenCVImageLabel : public QLabel
{
    Q_OBJECT
public:
    OpenCVImageLabel(QWidget * parent);
    ~OpenCVImageLabel();

private:
    cv::Mat img;
    QImage qtImage;

private slots:
    void setOpenCVImage(cv::Mat const & img);
    void setOpenCVImage(const char * const * data, const long height, const long width, const long channels);

};

#endif // OPENCVIMAGELABEL_H
