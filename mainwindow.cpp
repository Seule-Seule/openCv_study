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
    //��һ����ͨ��ͼ��ת����3����ͨ��ͼ��
    Mat BImg, GImg, RImg;
    std::vector<Mat> channels;
    Mat aChannels[3];

    //�����������
    split(_inMat, aChannels);
    //����vector�������
    split(_inMat, channels);

    //    BImg = aChannels[0];
    //    GImg = aChannels[1];
    //    RImg = aChannels[2];

    BImg = channels[0];
    GImg = channels[1];
    RImg = channels[2];

    imshow("��ɫ����", BImg);
    imshow("��ɫ����", GImg);
    imshow("��ɫ����", RImg);

    Mat BIamgT = Mat::zeros(BImg.size(), BImg.type());
    Mat GIamgT = Mat::zeros(BImg.size(), BImg.type());
    Mat RIamgT = Mat::zeros(BImg.size(), BImg.type());

    //   ��ÿ��ͨ������ʱ���Ż���ʾΪ��ɫ ͨ�������ĵ�ͨ��ֻ��ʾ��ɫ
    //    std::vector<Mat> aChannelss(3);
    //    aChannelss[0] = BImg;
    //    aChannelss[1] = GIamgT;
    //    aChannelss[2] = RIamgT;
    //merge���ܣ���һЩ����ϲ���һ����ͨ������
    Mat dst;
    merge(channels, dst);
    imshow("�������ͨ��", dst);
}

void testGraphicsText(Mat& _bgImage)
{

    // ������
    Point lp1 = Point(10,10);
    Point lp2 = Point(_bgImage.cols - 10, _bgImage.rows - 10);
    Scalar lcolor = Scalar(0,0,255);
    line(_bgImage, lp1, lp2, lcolor, 2,LINE_AA, 0);

    // ���ƾ���
    Point rectanglep1 = Point(100,100);
    Point rectanglep2 = Point(_bgImage.cols - 100, _bgImage.rows - 100);
    Scalar rectangleColor = Scalar(255,0,0);
    rectangle(_bgImage, rectanglep1, rectanglep2, rectangleColor, 2, LINE_AA, 0);

    // ������Բ
    Point ellipseConter = Point(_bgImage.cols/2, _bgImage.rows/2);
    Scalar ellipseColor = Scalar(0,255,0);
    Size ellipseSize = Size((_bgImage.cols - 200)/2, (_bgImage.rows - 200)/4);
    ellipse(_bgImage, ellipseConter, ellipseSize, 90, 0, 360, ellipseColor, 2, LINE_AA, 0);

    // ����Բ
    Point circleConter = Point(_bgImage.cols/2, _bgImage.rows/2);
    Scalar circleColor = Scalar(0,255,255);
    int  circleRadius = (_bgImage.cols - 200)/2;
    circle(_bgImage, circleConter, circleRadius, circleColor, 2, LINE_AA, 0);

    // ���ƶ�������
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

    // �������ֱ��
    RNG rng(123456);
    Mat rngLineMat = Mat::ones(_bgImage.size(), _bgImage.type());
    for (int index = 0; index < 1000; index ++){
        Point rngLinep1 = Point(rng.uniform(0, _bgImage.cols),rng.uniform(0, _bgImage.rows));
        Point rngLinep2 = Point(rng.uniform(0, _bgImage.cols),rng.uniform(0, _bgImage.rows));
        Scalar rngLinecolor = Scalar(rng.uniform(0, 255),rng.uniform(0, 255),rng.uniform(0, 255));
        line(rngLineMat, rngLinep1, rngLinep2, rngLinecolor, 2,LINE_AA, 0);
    }

    // ��������
    std::string textString("Hello OpenCv");
    Point textPoint = Point( _bgImage.cols/2, _bgImage.rows/2 - 200);
    Scalar putTextColor = Scalar(255,0 ,255);
    putText(_bgImage, textString, textPoint, FONT_HERSHEY_COMPLEX, 5, putTextColor, 2, LINE_AA);

    imshow("����ͼ��", _bgImage);
    imshow("�������ֱ��", rngLineMat);
}

void  testBlur(Mat& _inMat)
{
    // ��ֵģ��
    Mat blurMat;
    blur(_inMat, blurMat, Size(11, 11), Point(-1, -1) );
    imshow("��ֵģ��", blurMat);

    // ��˹ģ��
    Mat gaussianBlurMat;
    GaussianBlur(_inMat, gaussianBlurMat, Size(11, 11), 11, 11);
    imshow("��˹ģ��", blurMat);

    // ��ֵģ��  ������������������
    Mat medianBlurMat;
    medianBlur(_inMat, medianBlurMat, 11);
    imshow("��ֵģ��", medianBlurMat);

    // ��˹˫���˲�  �ֲ���˹�㷨 ��Ե�����ʱ��������� ���ԽϺõı���ͼ������
    Mat bilateralFilterMat;
    bilateralFilter(_inMat, bilateralFilterMat, 11, 100, 3);
    imshow("��˹˫���˲�", medianBlurMat);

}

void testDilateErode(Mat& _inMat)
{
    Mat out;
    //��ȡ�Զ����
    Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
    //����  ��С��϶ ������䰵��
    dilate(_inMat, out, element);
    FqImageMat::show("����",out);

    // ��ʴ ��Сë�� �����������
    erode(_inMat, out, element);
    FqImageMat::show("��ʴ",out);

    //�߼���̬ѧ����������������Ϳ����ˣ�����Ҫѡ�����ֲ��������޸ĵ����������Ϳ�����
    // ������ �ȸ�ʴ�������� ȥ��ë��
    morphologyEx(_inMat, out, MORPH_OPEN, element);
    FqImageMat::show("������",out);

    // ������ �����ͣ��ڸ�ʴ ȥ����϶
    morphologyEx(_inMat, out, MORPH_CLOSE, element);
    FqImageMat::show("������",out);

    // �ݶ����� ���͵�ͼ��-��ʴ��ͼ�� ��Ե��Ϣ
    morphologyEx(_inMat, out, MORPH_GRADIENT, element);
    FqImageMat::show("�ݶ�����",out);

    // ��ñ ԭʼͼ��-�������� Ҫ������ë����Ϣ
    morphologyEx(_inMat, out, MORPH_TOPHAT, element);
    FqImageMat::show("��ñ",out);

    // ��ñ ��������-ԭʼͼ�� Ҫ�����ķ�϶��Ϣ
    morphologyEx(_inMat, out, MORPH_BLACKHAT, element);
    FqImageMat::show("��ñ",out);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    FqImageMat imageMat(cv::String("D:/Data/Code_Workspace/imagesTestEx/test.jpg"), IMREAD_UNCHANGED);

    if (imageMat.empty()){
        qDebug() << "imageMat is empty!" ;
        return ;
    }

    FqImageMat::setLabelPixmap(ui->label, imageMat, this->width()/2, this->height());

    //     //cvtColor
    //        Mat outStr ;
    //        cvtColor(imageMat.mat, outStr, CV_RGB2HLS_FULL);
    //        FqImageMat::setLabelPixmap(ui->label2, outStr, this->width()/2, this->height());

    //     //mask  for Mat.ptr()
    //        Mat maskOutMat = Mat::zeros(imageMat.mat.size(), imageMat.mat.type());
    //        testMask(0, imageMat.mat, maskOutMat);
    //        FqImageMat::setLabelPixmap(ui->label2, maskOutMat,  this->width()/2, this->height());

    //     //get Pixel with three or one channels
    //        Mat pixelMat = Mat::zeros(imageMat.mat.size(), imageMat.mat.type());

    //        // ע��ͨ����ƥ�� �����´������ͼ��������ѹ����
    //        Mat pixelMat2 = Mat::zeros(imageMat.mat.size(), CV_8UC1);
    //        cvtColor(imageMat.mat, pixelMat, CV_RGB2GRAY, 1);
    //        testPixel(imageMat.mat, pixelMat2);  // three channels
    //        //testPixel(pixelMat, pixelMat2);  // one channels
    //        FqImageMat::setLabelPixmap(ui->label2, pixelMat2,  this->width()/2, this->height());

    //     //ͼ����  G(x)=(1-a)F(x)+aQ(x)
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
    //        FqImageMat::setLabelPixmap(ui->label, str1,  this->width()/2, this->height());
    //        FqImageMat::setLabelPixmap(ui->label2, str2,  this->width()/2, this->height());
    //        double alpha = 0.5;
    //        // Ҫ��֤��Ҫ��ϵ�ͼ��ߴ������һ��
    //        if (str1.mat.size() == str2.mat.size() && str1.mat.type() == str2.mat.type()){
    //            // addWeighted(str1.mat, alpha, str2.mat, 1-alpha, 0.0,outStr);
    //            //add(str1.mat, str2.mat, outStr);
    //             multiply(str1.mat, str2.mat, outStr);
    //            imshow("test", outStr);
    //        }

    //     //ͼ��ԱȶȺ����Ȳ���  G(x) = aF(x) + b // aӰ��Աȶ� bӰ������
    //        Mat testMat;

    //        // ��ͨ��ͼ�����
    //        testMat = Mat::zeros(imageMat.mat.size(), imageMat.mat.type());
    //        Mat threeChannelMat = imageMat.mat;
    //        testBrightnessContrast(threeChannelMat, testMat);

    //        // ��ͨ��ͼ�����
    //        // testMat = Mat::zeros(imageMat.mat.size(), CV_8UC1);
    //        //Mat oneChannelMat;
    //        //cvtColor(imageMat.mat, oneChannelMat, CV_RGB2GRAY);
    //        //testBrightnessContrast(oneChannelMat, testMat);
    //        FqImageMat::setLabelPixmap(ui->label2, testMat,  this->width()/2, this->height());

    //    //������ɫͨ������ͨ���ں�
    //    testChannels(imageMat.mat);

    //    // openCv����ͼ��������
    //    testGraphicsText(imageMat.mat);

    //    // openCv  ͼ��ģ��
    //    testBlur(imageMat.mat);

    // ͼ�������̬ѧ���� ���� ��ʴ ������ ������ �ݶȼ��� ��ñ�ͺ�ñ
    testDilateErode(imageMat.mat);

}


MainWindow::~MainWindow()
{
    delete ui;
}

