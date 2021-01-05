#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QVector>

#include "FqImageMat.hpp"

using namespace cv;

QImage cvMatToQImage( const cv::Mat &inMat )
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
//         static QVector<QRgb>  sColorTable( 256 );

//         only create our color table the first time
//         注意：下面这种写法是错的，参考后文的说明
//         如果将这个函数定义成一个类方法 那可以在图像没有改变时加快转换速度 现在这里是个函数 就不在加快转换速度
//         且原函数在这个地方有bug sColorTable 定义时确定了大小， 默认初始化过了 第一次转换会出错 不能用 isEmpty()  判断是否第一次转换
//              可以考虑初始化表值后 由值判断是否第一次转换
//         if ( sColorTable.isEmpty() )
//        {
//           for ( int i = 0; i < 256; ++i )
//           {
//              sColorTable[i] = qRgb( i, i, i );
//           }
//        }

        QVector<QRgb>  sColorTable( 256 );

        for ( int i = 0; i < 256; ++i )
        {
           sColorTable[i] = qRgb( i, i, i );
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

void setLabelPixmap(QLabel * _qLanel, Mat & _cvMat, int _pWidth = 0, int _pHeight = 0)
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


void testMask(int type, Mat & _inMat, Mat& _outMat)
{
    if (type == 0){
        // Make mask by oneself.
        // mask
        /*
         * 0  -1  0
         * -1  5  -1
         * 0  -1  0
         *
         */

        double t = getTickCount();
        int cols = (_inMat.cols -1) * _inMat.channels();
        int offsetX = _inMat.channels();
        int rows = _inMat.rows;

        for (int row = 1; row < rows -1; row++){
            const uchar * previousPtr = _inMat.ptr<uchar>(row-1);
            const uchar * currentPtr  = _inMat.ptr<uchar>(row);
            const uchar * nextPtr     = _inMat.ptr<uchar>(row+1);

            uchar* output = _outMat.ptr<uchar>(row);
            for (int col = offsetX; col< cols; col++){
                // saturate_cast<uchar>(var)  can sure var number is 0 to 255;
                output[col] = saturate_cast<uchar>(5 * currentPtr[col] - (currentPtr[col - offsetX] + currentPtr[col + offsetX] + previousPtr[col] + nextPtr[col]));
            }
        }

        double timeConSume = (getTickCount() - t) / getTickFrequency();
        qDebug() << "Myself mask run time is " << timeConSume ;
    }

    else if (type == 1) {
// Use openCv mask API.
double t = getTickCount();
Mat kernel = (Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
filter2D(_inMat, _outMat, _inMat.depth(), kernel);
double timeConSume = (getTickCount() - t) / getTickFrequency();
qDebug() << "System API mask run time is " << timeConSume ;
    }
}

void testPixel( Mat & _inMat, Mat& _outMat)
{
//    int width = _inMat.cols;
//    int height = _inMat.rows;
//    int channels = _inMat.channels();

//    // Myself
//    for (int row = 0; row < height; row++){

//        for (int col = 0; col < width; col++){
//            if (channels == 1){
//                int pixelV = _inMat.at<uchar>(row, col);
//                _outMat.at<uchar>(row, col) = 255 - pixelV;
//            }
//            else if (channels == 3){
//                int b = _inMat.at<Vec3b>(row, col)[0];
//                int g = _inMat.at<Vec3b>(row, col)[1];
//                int r = _inMat.at<Vec3b>(row, col)[2];

//                _outMat.at<Vec3b>(row, col)[0] = 255 - b;
//                _outMat.at<Vec3b>(row, col)[1] = 255 - g;
//                _outMat.at<Vec3b>(row, col)[2] = 255 - r;
//            }
//        }
//    }

    // System API.
    bitwise_not(_inMat, _outMat);

}

void testBrightnessContrast(Mat& _inMat, Mat & _outMat)
{
    float a = 1.5;
    float bate = 30;
    int width = _inMat.cols;
    int height = _inMat.rows;

    _inMat.convertTo(_inMat, CV_32F);
    for (int row = 0; row < height; row++){
        for(int col =0; col < width; col++){
            if (_inMat.channels() == 1){
                float pixV = _inMat.at<float>(row,col);
                _outMat.at<uchar>(row, col) = saturate_cast<uchar>(pixV*a +bate);
            }
            else if (_inMat.channels() == 3){
                float b =  _inMat.at<Vec3f>(row, col)[0];
                float g =  _inMat.at<Vec3f>(row, col)[1];
                float r =  _inMat.at<Vec3f>(row, col)[2];

                _outMat.at<Vec3b>(row,col)[0] = saturate_cast<uchar>(b *a +bate);
                _outMat.at<Vec3b>(row,col)[1] = saturate_cast<uchar>(g *a +bate);
                _outMat.at<Vec3b>(row,col)[2] = saturate_cast<uchar>(r *a +bate);
            }
        }
    }
}

void testChannels(Mat& _inMat)
{
    //把一个三通道图像，转换成3个单通道图像
    Mat BImg, GImg, RImg;
    std::vector<Mat> channels;
    Mat aChannels[3];

    //利用数组分离
    split(_inMat, aChannels);
    //利用vector对象分离
    split(_inMat, channels);

//    BImg = aChannels[0];
//    GImg = aChannels[1];
//    RImg = aChannels[2];

    BImg = channels[0];
    GImg = channels[1];
    RImg = channels[2];

    imshow("蓝色分量", BImg);
    imshow("绿色分量", GImg);
    imshow("红色分量", RImg);

    Mat BIamgT = Mat::zeros(BImg.size(), BImg.type());
    Mat GIamgT = Mat::zeros(BImg.size(), BImg.type());
    Mat RIamgT = Mat::zeros(BImg.size(), BImg.type());

//   当每个通道存在时，才会显示为彩色 通道分离后的单通道只显示灰色
//    std::vector<Mat> aChannelss(3);
//    aChannelss[0] = BImg;
//    aChannelss[1] = GIamgT;
//    aChannelss[2] = RIamgT;
    //merge功能：将一些数组合并成一个多通道数组
    Mat dst;
    merge(channels, dst);
    imshow("重新组合通道", dst);
}

void testGraphicsText(Mat& _bgImage)
{

    // 绘制线
    Point lp1 = Point(10,10);
    Point lp2 = Point(_bgImage.cols - 10, _bgImage.rows - 10);
    Scalar lcolor = Scalar(0,0,255);
    line(_bgImage, lp1, lp2, lcolor, 2,LINE_AA, 0);

    // 绘制矩形
    Point rectanglep1 = Point(100,100);
    Point rectanglep2 = Point(_bgImage.cols - 100, _bgImage.rows - 100);
    Scalar rectangleColor = Scalar(255,0,0);
    rectangle(_bgImage, rectanglep1, rectanglep2, rectangleColor, 2, LINE_AA, 0);

    // 绘制椭圆
    Point ellipseConter = Point(_bgImage.cols/2, _bgImage.rows/2);
    Scalar ellipseColor = Scalar(0,255,0);
    Size ellipseSize = Size((_bgImage.cols - 200)/2, (_bgImage.rows - 200)/4);
    ellipse(_bgImage, ellipseConter, ellipseSize, 90, 0, 360, ellipseColor, 2, LINE_AA, 0);

    // 绘制圆
    Point circleConter = Point(_bgImage.cols/2, _bgImage.rows/2);
    Scalar circleColor = Scalar(0,255,255);
    int  circleRadius = (_bgImage.cols - 200)/2;
    circle(_bgImage, circleConter, circleRadius, circleColor, 2, LINE_AA, 0);

    // 绘制多边形填充
    Point root_points[1][6];
    root_points[0][0] = Point(100 + _bgImage.cols/2, _bgImage.rows/2);
    root_points[0][1] = Point(100 + _bgImage.cols/2 + 100, _bgImage.rows/2 - 100);
    root_points[0][2] = Point(100 + _bgImage.cols/2 + 200, _bgImage.rows/2 );
    root_points[0][3] = Point(100 + _bgImage.cols/2, _bgImage.rows/2 + 200);
    root_points[0][4] = Point(100 + _bgImage.cols/2 - 200, _bgImage.rows/2);
    root_points[0][5] = Point(100 + _bgImage.cols/2 - 100, _bgImage.rows/2 - 100);
    Scalar polylinesColor = Scalar(0,0,0);
    const Point* ppt[1] = { root_points[0] };
    int npt[] = { 6 };
    polylines(_bgImage, ppt, npt, 1, 1, polylinesColor, 2, LINE_AA, 0);
    fillPoly(_bgImage, ppt, npt, 1, Scalar(255, 255, 255), LINE_AA, 0);

    // 绘制随机直线
    RNG rng(123456);
    Mat rngLineMat = Mat::ones(_bgImage.size(), _bgImage.type());
    for (int index = 0; index < 1000; index ++){
        Point rngLinep1 = Point(rng.uniform(0, _bgImage.cols),rng.uniform(0, _bgImage.rows));
        Point rngLinep2 = Point(rng.uniform(0, _bgImage.cols),rng.uniform(0, _bgImage.rows));
        Scalar rngLinecolor = Scalar(rng.uniform(0, 255),rng.uniform(0, 255),rng.uniform(0, 255));
        line(rngLineMat, rngLinep1, rngLinep2, rngLinecolor, 2,LINE_AA, 0);
    }

    // 绘制文字
    std::string textString("Hello OpenCv");
    Point textPoint = Point( _bgImage.cols/2, _bgImage.rows/2 - 200);
    Scalar putTextColor = Scalar(255,0 ,255);
    putText(_bgImage, textString, textPoint, FONT_HERSHEY_COMPLEX, 5, putTextColor, 2, LINE_AA);

    imshow("绘制图形", _bgImage);
    imshow("绘制随机直线", rngLineMat);
}

void  testBlur(Mat& _inMat)
{
    // 均值模糊
    Mat blurMat;
    blur(_inMat, blurMat, Size(11, 11), Point(-1, -1) );
    imshow("均值模糊", blurMat);

    // 高斯模糊
    Mat gaussianBlurMat;
    GaussianBlur(_inMat, gaussianBlurMat, Size(11, 11), 11, 11);
    imshow("高斯模糊", blurMat);

    // 中值模糊  多用于消除椒盐噪声
    Mat medianBlurMat;
    medianBlur(_inMat, medianBlurMat, 11);
    imshow("中值模糊", medianBlurMat);

    // 高斯双边滤波  弥补高斯算法 边缘差异大时产生的误差 可以较好的保留图像轮廓
    Mat bilateralFilterMat;
    bilateralFilter(_inMat, bilateralFilterMat, 11, 100, 3);
    imshow("高斯双边滤波", medianBlurMat);

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    FqImageMat imageMat(cv::String("D:/Data/Code_Workspace/imagesTestEx/test2.jpg"), IMREAD_UNCHANGED);

    if (imageMat.empty()){
        qDebug() << "imageMat is empty!" ;
        return ;
    }


    FqImageMat::show("test1", imageMat.getImage());
    FqImageMat::show("test2", imageMat.getMat());
    FqImageMat::show("test3", imageMat);

    ui->label->setPixmap(QPixmap::fromImage(imageMat.getImage()));

    // cvtColor
//    Mat outStr ;
//    cvtColor(str, outStr, CV_RGB2HLS);

//    QImage outStrQImage = cvMatToQImage(outStr);
//    ui->label2->setPixmap(QPixmap::fromImage(outStrQImage));

    // imwrite
//    imwrite("D:/Data/Code_Workspace/imagesTestEx/QtHLS.png", outStr);


    //setLabelPixmap(ui->label, str, this->width()/2, this->height());

    // mask  for Mat.ptr()
//    Mat maskOutMat = Mat::zeros(str.size(), str.type());
//    testMask(1, str, maskOutMat);
//    setLabelPixmap(ui->label2, maskOutMat,  this->width()/2, this->height());

    // get Pixel with three or one channels
   //  Mat pixelMat = Mat::zeros(str.size(), str.type());

    // 注意通道数匹配 否则导致处理完的图像看起来像被压缩过
//    Mat pixelMat2 = Mat::zeros(str.size(), CV_8UC1);
//    cvtColor(str, pixelMat, CV_RGB2GRAY, 1);
//    testPixel(str, pixelMat2);  // three channels
//    //testPixel(pixelMat, pixelMat2);  // one channels
//    setLabelPixmap(ui->label2, pixelMat2,  this->width()/2, this->height());

    // 图像混合  G(x)=(1-a)F(x)+aQ(x)
//    Mat str1 = imread("D:/Data/Code_Workspace/imagesTestEx/CMD.jpg", IMREAD_UNCHANGED);
//    Mat str2 = imread("D:/Data/Code_Workspace/imagesTestEx/git.jpg", IMREAD_UNCHANGED);
//    if (str1.empty()){
//        qDebug() << "str1 is empty!" ;
//        return ;
//    }
//    if (str2.empty()){
//        qDebug() << "str2 is empty!" ;
//        return ;
//    }
//    Mat outStr;
//    setLabelPixmap(ui->label, str1,  this->width()/2, this->height());
//    setLabelPixmap(ui->label2, str2,  this->width()/2, this->height());
//    double alpha = 0.5;
//    // 要保证需要混合的图像尺寸和类型一致
//    if (str1.size() == str2.size() && str1.type() == str2.type()){
//        //addWeighted(str1, alpha, str2, 1-alpha, 0.0,outStr);
//        // add(str1, str2, outStr);
//        multiply(str1, str2, outStr);
//        imshow("test", outStr);
//    }

    // 图像对比度和亮度操作  G(x) = aF(x) + b // a影响对比度 b影响亮度
//    Mat testMat;
//    //testMat = Mat::zeros(str.size(), str.type());
//    testMat = Mat::zeros(str.size(), CV_8UC1);
//    cvtColor(str, str, CV_RGB2GRAY);
//    testBrightnessContrast(str,testMat);
//    setLabelPixmap(ui->label2, testMat,  this->width()/2, this->height());

    //分离颜色通道，多通道融合
    //testChannels(str);

    // openCv绘制图形有文字
 //   testGraphicsText(str);

    // openCv  图像模糊
    //testBlur(str);

}


MainWindow::~MainWindow()
{
    delete ui;
}

