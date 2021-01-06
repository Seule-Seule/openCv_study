#ifndef FQIMAGEMAT_HPP
#define FQIMAGEMAT_HPP

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QWidget>
#include <QLabel>

class FqImageMat
{
public:
    FqImageMat();
    FqImageMat(const cv::Mat &inMat);

    // IMREAD_UNCHANGED            = -1, //!< If set, return the loaded image as is (with alpha channel, otherwise it gets cropped). Ignore EXIF orientation.
    // IMREAD_GRAYSCALE            = 0,  //!< If set, always convert image to the single channel grayscale image (codec internal conversion).
    // IMREAD_COLOR                = 1,  RGB
    FqImageMat( const cv::String& filename, int flags = cv::IMREAD_COLOR);
    ~FqImageMat();

    static QImage  cvMatToQImage( const cv::Mat &inMat );
    static cv::Mat QImageTocvMat(QImage image);

    bool empty();
    bool setMat(cv::Mat& inMat);
    bool setImage(QImage& inImage);

    static void setLabelPixmap( QLabel * _qLanel, const  cv::Mat & _cvMat, int _pWidth = 0, int _pHeight = 0);
    static void setLabelPixmap( QLabel * _qLanel, const QImage  &inImage, int _pWidth = 0, int _pHeight = 0);
    static void setLabelPixmap( QLabel * _qLanel, FqImageMat &inFqImageMat, int _pWidth = 0, int _pHeight = 0);

    static void show(const cv::String& winname, const cv::Mat &inMat);
    static void show(const cv::String& winname, const QImage  &inImage);
    static void show(const cv::String& winname, FqImageMat &inFqImageMat);

    static void write(const cv::String& filename, const cv::Mat &inMat);
    static void write(const cv::String& filename, const QImage  &inImage);
    static void write(const cv::String& filename, FqImageMat &inFqImageMat);
public:

    cv::Mat mat;
    QImage image;

};


#endif // FQIMAGEMAT_HPP
