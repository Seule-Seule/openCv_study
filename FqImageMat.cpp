#include "FqImageMat.hpp"

#include <QImage>
#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <opencv2/opencv.hpp>

FqImageMat::FqImageMat(const cv::Mat &_inMat)
{
    if (_inMat.empty()){
        qDebug() << "FqImageMat input Mat is empty!" ;
        return;
    }
    mat = _inMat;
    image = cvMatToQImage(_inMat);
}

FqImageMat::FqImageMat()
{

}

FqImageMat::FqImageMat(const cv::String& filename, int flags)
{
    if (filename.empty()){
        qDebug() << "FqImageMat input filename is empty!";
        return;
    }

    mat = cv::imread(filename, flags);
    if (mat.empty()){
        qDebug() << "FqImageMat get image fail!";
        return ;
    }
    image = cvMatToQImage(mat);
}

FqImageMat::~FqImageMat()
{
}

bool FqImageMat::empty()
{
    return mat.empty();
}

bool FqImageMat::setMat(cv::Mat& inMat)
{
    if (inMat.empty()){
        return false;
    }
    mat = inMat;
    image = cvMatToQImage(inMat);

    return true;
}

bool FqImageMat::setImage(QImage& inImage)
{
    if (inImage.isNull()){
        return false;
    }
    image = inImage;
    mat = QImageTocvMat(inImage);
    return true;
}

void FqImageMat::setLabelPixmap( QLabel * _qLanel, const  cv::Mat & _cvMat, int _pWidth, int _pHeight)
{
    if (_pWidth == 0){
        _pWidth = _cvMat.size().width;
    }
    if (_pHeight == 0){
        _pHeight = _cvMat.size().height;
    }
    QImage tempQImage = cvMatToQImage(_cvMat);
    QPixmap pixmap = QPixmap::fromImage(tempQImage);
    pixmap= pixmap.scaled(_pWidth, _pHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _qLanel->setPixmap(pixmap);
}

void FqImageMat::setLabelPixmap( QLabel * _qLanel, const QImage  &inImage, int _pWidth, int _pHeight)
{
    if (_pWidth == 0){
        _pWidth = inImage.width();
    }
    if (_pHeight == 0){
        _pHeight = inImage.height();
    }
    QPixmap pixmap = QPixmap::fromImage(inImage);
    pixmap= pixmap.scaled(_pWidth, _pHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _qLanel->setPixmap(pixmap);

}
void FqImageMat::setLabelPixmap( QLabel * _qLanel, FqImageMat &inFqImageMat, int _pWidth, int _pHeight)
{
    if (_pWidth == 0){
        _pWidth = inFqImageMat.mat.size().width;
    }
    if (_pHeight == 0){
        _pHeight = inFqImageMat.mat.size().height;
    }
    QPixmap pixmap = QPixmap::fromImage(inFqImageMat.image);
    pixmap= pixmap.scaled(_pWidth, _pHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _qLanel->setPixmap(pixmap);

}

void FqImageMat::show(const cv::String& winname, const cv::Mat &inMat)
{
    if (winname.empty() || inMat.empty()){
        qDebug() << "FqImageMat::show winname.empty() || inMat.empty()!";
    }
    cv::namedWindow(winname, cv::WINDOW_NORMAL);
    cv::imshow(winname, inMat);
}

void FqImageMat::show(const cv::String& winname, const QImage  &inImage)
{
    if (winname.empty() || inImage.isNull()){
        qDebug() << "FqImageMat::show winname.empty() || inImage.isNull()!";
    }
    cv::Mat _tempMat = QImageTocvMat(inImage);
    cv::namedWindow(winname, cv::WINDOW_NORMAL);
    cv::imshow(winname, _tempMat);
}

void FqImageMat::show(const cv::String& winname, FqImageMat &inFqImageMat)
{
    if (winname.empty() || inFqImageMat.empty() ){
        qDebug() << "FqImageMat::show winname.empty() || inFqImageMat.empty()!";
    }
    cv::namedWindow(winname, cv::WINDOW_NORMAL);
    cv::imshow(winname, inFqImageMat.mat);
}


void FqImageMat::write(const cv::String& filename, const cv::Mat &inMat)
{
    if (filename.empty() || inMat.empty()){
        qDebug() << "FqImageMat::write filename.empty() || inMat.empty()!";
    }
    cv::imwrite(filename,inMat);
}

void FqImageMat::write(const cv::String& filename, const QImage  &inImage)
{
    if (filename.empty() || inImage.isNull()){
        qDebug() << "FqImageMat::write filename.empty() || inImage.empty()!";
    }
    cv::Mat tempMat = QImageTocvMat(inImage);
    cv::imwrite(filename,tempMat);
}

void FqImageMat::write(const cv::String& filename, FqImageMat &inFqImageMat)
{
    if (filename.empty() || inFqImageMat.empty()){
        qDebug() << "FqImageMat::write filename.empty() || inFqImageMat.empty()!";
    }
    cv::imwrite(filename,inFqImageMat.mat);
}


QImage FqImageMat::cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable( 256, qRgb(0,0,0) );
        // only create our color table the first time.
        if ( sColorTable[255] == qRgb(0,0,0) )
        {
            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );

        return image;
    }

    default:
        qWarning() << "cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

cv::Mat FqImageMat::QImageTocvMat(QImage image)
{
    cv::Mat mat, matTemp;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        matTemp = mat.clone();
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        matTemp = mat.clone();
        cv::cvtColor(matTemp, matTemp, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        matTemp = mat.clone();
        break;

    default:
        qWarning() << "QImageTocvMat() - QImage image type not handled in switch:" << image.format();
        break;
    }
    return matTemp;
}



