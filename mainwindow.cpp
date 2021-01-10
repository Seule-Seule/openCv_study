#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QVector>

#include "FqImageMat.hpp"

using namespace cv;

void testMask(int type, const Mat & _inMat, Mat& _outMat)
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

void testPixel(const Mat & _inMat, Mat& _outMat)
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

void testDilateErode(Mat& _inMat)
{
    Mat out;
    //获取自定义核
    Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
    //膨胀  减小缝隙 亮点填充暗点
    dilate(_inMat, out, element);
    FqImageMat::show("膨胀",out);

    // 腐蚀 减小毛刺 暗点填充亮点
    erode(_inMat, out, element);
    FqImageMat::show("腐蚀",out);

    //高级形态学处理，调用这个函数就可以了，具体要选择哪种操作，就修改第三个参数就可以了
    // 开运算 先腐蚀，在膨胀 去除毛刺
    morphologyEx(_inMat, out, MORPH_OPEN, element);
    FqImageMat::show("开运算",out);

    // 闭运算 先膨胀，在腐蚀 去除缝隙
    morphologyEx(_inMat, out, MORPH_CLOSE, element);
    FqImageMat::show("闭运算",out);

    // 梯度运算 膨胀的图像-腐蚀的图像 边缘信息
    morphologyEx(_inMat, out, MORPH_GRADIENT, element);
    FqImageMat::show("梯度运算",out);

    // 高帽 原始图像-开运算结果 要消除的毛刺信息
    morphologyEx(_inMat, out, MORPH_TOPHAT, element);
    FqImageMat::show("高帽",out);

    // 黑帽 闭运算结果-原始图像 要消除的缝隙信息
    morphologyEx(_inMat, out, MORPH_BLACKHAT, element);
    FqImageMat::show("黑帽",out);
}

void testmorphologyExLine()
{
    Mat lineMat = imread(cv::String("D:/Data/Code_Workspace/imagesTestEx/lineABCD.png"), IMREAD_COLOR);
    Mat garyMat, binMat, dst;
    // 转换为灰度图像——cvtColor
    cvtColor(~lineMat, garyMat, CV_RGB2GRAY);
    FqImageMat::show("灰度图像",garyMat);

    // 转换为二值图像——adaptiveThreshold
    adaptiveThreshold(garyMat, binMat, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -2);
    FqImageMat::show("二值图像",binMat);

    // 提取水平线
    Mat elementVline = getStructuringElement(MORPH_RECT, Size(lineMat.cols / 32, 1));
    morphologyEx(binMat, dst, MORPH_OPEN, elementVline);
    FqImageMat::show("提取水平线",dst);

    //提取垂直线
    Mat elementHline = getStructuringElement(MORPH_RECT, Size(1, lineMat.rows / 32));
    morphologyEx(binMat, dst, MORPH_OPEN, elementHline);
    FqImageMat::show("提取垂直线",dst);

    // 开运算操作实际应用
    Mat kernel = getStructuringElement(MORPH_RECT,Size(3, 3));
    morphologyEx(binMat,dst,MORPH_OPEN,kernel);
    FqImageMat::show("开运算操作实际应用",~dst);

}

void  testPyrdownPyrup()
{
    Mat srcMat = imread(cv::String("D:/Data/Code_Workspace/imagesTestEx/test2.jpg"), IMREAD_COLOR);
    Mat downMat, downMat2, upMat, upMat2;
    imshow("原图",srcMat );

    // 下采样 原图长宽各减小一半 面积减小1/4
    Mat  matGauss,matGauss2;
    GaussianBlur(srcMat, matGauss,Size(5,5), 0.3 , 0.4);
    pyrDown(matGauss, downMat, Size(matGauss.cols/2 , matGauss.rows/2));
    imshow("下采样",downMat );

    GaussianBlur(downMat, matGauss2,Size(3, 3), 0.5, 0.6);
    pyrDown(matGauss2, downMat2, Size(matGauss2.cols /2 , matGauss2.rows/2));
    imshow("下采样2",downMat2 );

    // 上采样
    pyrUp(downMat2, upMat, Size(downMat2.cols * 2, downMat2.rows * 2));
    imshow("上采样",upMat );

    pyrUp(upMat, upMat2, Size(upMat.cols * 2, upMat.rows * 2));
    imshow("上采样2",upMat2 );

    // Laplacian 金字塔
    Mat laplacianMat, laplacianMat2;
    Mat resetMat, resetMat2;
    // 调整图片尺寸
    matGauss.resize(upMat2.rows);
    if (matGauss.size() == upMat2.size() ){
        subtract(matGauss, upMat2, laplacianMat);
        imshow("Laplacian",laplacianMat );

        // 还原图像
        add(laplacianMat, upMat2, resetMat);
        imshow("resetMat",resetMat );
    }
    matGauss2.resize(upMat.rows);
    if (matGauss2.size() == upMat.size() ){
        subtract(matGauss2, upMat, laplacianMat2);
        imshow("Laplacian2",laplacianMat2 );

        // 还原图像
        add(laplacianMat2, upMat, resetMat2);
        imshow("resetMat2",resetMat2 );
    }

    // DOG
    Mat matGauss3, matGauss4;
    Mat matDOG;
    GaussianBlur(srcMat, matGauss3,Size(5,5), 0.3 , 0.4);
    GaussianBlur(srcMat, matGauss4,Size(5,5), 0.7 , 0.8);
    subtract(matGauss3, matGauss4, matDOG);
    imshow("DOG",laplacianMat2 );

    // 之后可以从matDOG中提取特征点特别是角点 用于分析图象
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    FqImageMat imageMat(cv::String("D:/Data/Code_Workspace/imagesTestEx/Qt.png"), IMREAD_UNCHANGED);

    if (imageMat.empty()){
        qDebug() << "imageMat is empty!" ;
        return ;
    }

    FqImageMat::setLabelPixmap(ui->leftLabel->label, imageMat, this->width()/2, this->height());

    //     //cvtColor
    //        Mat outStr ;
    //        cvtColor(imageMat.mat, outStr, CV_RGB2HLS_FULL);
    //        FqImageMat::setLabelPixmap(ui->rightLabel->label, outStr, this->width()/2, this->height());

    //     //mask  for Mat.ptr()
    //        Mat maskOutMat = Mat::zeros(imageMat.mat.size(), imageMat.mat.type());
    //        testMask(0, imageMat.mat, maskOutMat);
    //        FqImageMat::setLabelPixmap(ui->rightLabel->label, maskOutMat,  this->width()/2, this->height());

    //     //get Pixel with three or one channels
    //        Mat pixelMat = Mat::zeros(imageMat.mat.size(), imageMat.mat.type());

    //        // 注意通道数匹配 否则导致处理完的图像看起来像被压缩过
    //        Mat pixelMat2 = Mat::zeros(imageMat.mat.size(), CV_8UC1);
    //        cvtColor(imageMat.mat, pixelMat, CV_RGB2GRAY, 1);
    //        testPixel(imageMat.mat, pixelMat2);  // three channels
    //        //testPixel(pixelMat, pixelMat2);  // one channels
    //        FqImageMat::setLabelPixmap(ui->rightLabel->label, pixelMat2,  this->width()/2, this->height());

    //     //图像混合  G(x)=(1-a)F(x)+aQ(x)
    //        FqImageMat str1 = FqImageMat("D:/Data/Code_Workspace/imagesTestEx/CMD.jpg", IMREAD_UNCHANGED);
    //        FqImageMat str2 = FqImageMat("D:/Data/Code_Workspace/imagesTestEx/git.jpg", IMREAD_UNCHANGED);
    //        if (str1.empty()){
    //            qDebug() << "str1 is empty!" ;
    //            return ;
    //        }
    //        if (str2.empty()){
    //            qDebug() << "str2 is empty!" ;
    //            return ;
    //        }
    //        Mat outStr;
    //        FqImageMat::setLabelPixmap(ui->leftLabel->label, str1,  this->width()/2, this->height());
    //        FqImageMat::setLabelPixmap(ui->rightLabel->label, str2,  this->width()/2, this->height());
    //        double alpha = 0.5;
    //        // 要保证需要混合的图像尺寸和类型一致
    //        if (str1.mat.size() == str2.mat.size() && str1.mat.type() == str2.mat.type()){
    //            // addWeighted(str1.mat, alpha, str2.mat, 1-alpha, 0.0,outStr);
    //            //add(str1.mat, str2.mat, outStr);
    //             multiply(str1.mat, str2.mat, outStr);
    //            imshow("test", outStr);
    //        }

    //     //图像对比度和亮度操作  G(x) = aF(x) + b // a影响对比度 b影响亮度
    //        Mat testMat;

    //        // 三通道图像操作
    //        testMat = Mat::zeros(imageMat.mat.size(), imageMat.mat.type());
    //        Mat threeChannelMat = imageMat.mat;
    //        testBrightnessContrast(threeChannelMat, testMat);

    //        // 单通道图像操作
    //        // testMat = Mat::zeros(imageMat.mat.size(), CV_8UC1);
    //        //Mat oneChannelMat;
    //        //cvtColor(imageMat.mat, oneChannelMat, CV_RGB2GRAY);
    //        //testBrightnessContrast(oneChannelMat, testMat);
    //        FqImageMat::setLabelPixmap(ui->rightLabel->label, testMat,  this->width()/2, this->height());

    //    //分离颜色通道，多通道融合
    //    testChannels(imageMat.mat);

    //    // openCv绘制图形有文字
    //    testGraphicsText(imageMat.mat);

    //    // openCv  图像模糊
    //    testBlur(imageMat.mat);

    //    // 图像基本形态学操作 膨胀 腐蚀 开运算 闭运算 梯度计算 高帽和黑帽
    //    testDilateErode(imageMat.mat);
    //    testmorphologyExLine();

    // 图像上采样与下采样  插值算法 高斯函数差分(DOG)
    testPyrdownPyrup();

}


MainWindow::~MainWindow()
{
    delete ui;
}

