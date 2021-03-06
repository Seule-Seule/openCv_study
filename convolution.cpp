﻿#include "convolution.hpp"
#include <QDebug>

Convolution::Convolution()
{

    src = imread("D:\\Data\\Code_Workspace\\imagesTestEx\\lineABCD.png");
    if (src.empty()){
        qDebug() << "Image D:\\Data\\Code_Workspace\\imagesTestEx\\lineABCD.jpg  read faile!";
        return ;
    }
    RebortX = (Mat_<int>(2,2) << 1, 0, 0, -1);
    RebortY = (Mat_<int>(2,2) << 0, 1, -1, 0);
    SobelX = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    SobelY = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Lapulasi = (Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);

}

void Convolution::ConvolutionTest()
{
    namedWindow("src", CV_WINDOW_AUTOSIZE);
    namedWindow("RebortX", CV_WINDOW_AUTOSIZE);
    namedWindow("RebortY", CV_WINDOW_AUTOSIZE);
    namedWindow("SobelX", CV_WINDOW_AUTOSIZE);
    namedWindow("SobelY", CV_WINDOW_AUTOSIZE);
    namedWindow("Lapulasi", CV_WINDOW_AUTOSIZE);

    imshow("src", src);

    Mat RebortXMat, RebortYMat, SobelXMat, SobelYMat, LapulasiMat;

    //RebortX
    filter2D(src, RebortXMat, -1, RebortX, Point(-1, -1), 0);
    imshow("RebortX",RebortXMat);

    //RebortY
    filter2D(src, RebortYMat, -1, RebortY, Point(-1, -1), 0);
    imshow("RebortY",RebortYMat);

    //SobelX
    filter2D(src, SobelXMat, -1, SobelX, Point(-1, -1), 0);
    imshow("SobelX",SobelXMat);

    //SobelY
    filter2D(src, SobelYMat, -1, SobelY, Point(-1, -1), 0);
    imshow("SobelY",SobelYMat);

    //Lapulasi
    filter2D(src, LapulasiMat, -1, Lapulasi, Point(-1, -1), 0);
    imshow("Lapulasi",LapulasiMat);


    Mat RebortMat, SobelMat;
    namedWindow("Rebort", CV_WINDOW_AUTOSIZE);
    namedWindow("Sobel", CV_WINDOW_AUTOSIZE);

    // Rebort X + Rebort Y
    add(RebortXMat, RebortYMat, RebortMat);
    imshow("Rebort", RebortMat);

    // Sobel X + Sobel Y
    add(SobelXMat, SobelYMat, SobelMat);
    imshow("Sobel", SobelMat);
}


void Convolution::ConvolutionTestEdge()
{
    Mat dst;
    namedWindow("ConvolutionTestEdge", CV_WINDOW_AUTOSIZE);
    namedWindow("src", CV_WINDOW_AUTOSIZE);
    imshow("src", src);

    int top =int(0.05 * src.rows);
    int bottom = int(0.05 * src.rows);
    int left = int(0.05 * src.cols);
    int right = int(0.05 * src.cols);
    RNG rng(12345);   //随机数
    int borderType = BORDER_DEFAULT;

    int c = 0;
    while (true) {
        c = waitKey(500);
        // ESC
        if (char(c) == 27) {
            break;
        }
        if (char(c) == 'r') {
            borderType = BORDER_REPLICATE;
        }
        else if (char(c) == 'v') {
            borderType = BORDER_WRAP;
        }
        else if (char(c) == 'c') {
            borderType = BORDER_CONSTANT;
        }
        else if (char(c) == 'd') {
            borderType = BORDER_DEFAULT;
        }
        Scalar color = Scalar(rng.uniform(0, 255),rng.uniform(0, 255), rng.uniform(0, 255));
        copyMakeBorder(src,dst,top,bottom,left,right,borderType,color);
        imshow("ConvolutionTestEdge", dst);
    }

    waitKey(0);
}


void Convolution::SobelTest()
{
    imshow("src", src);
    Mat dst, gray, gaussian, grayX, grayY;

    GaussianBlur(src, gaussian, Size(3,3), 0);
    cvtColor(gaussian, gray, CV_BGR2GRAY);

    imshow("gray", gray);

    Sobel(gray, grayX, CV_16S, 1, 0, 3); // （CV_16S改写成-1与输入一样8U类型的话会漏掉很多信息，效果精准度会变差）
    Sobel(gray, grayY, CV_16S, 0, 1, 3);

//    Scharr(gray, grayX, CV_16S, 1, 0);
//    Scharr(gray, grayY, CV_16S, 1, 0);

    convertScaleAbs(grayX, grayX);  // 计算图像像素绝对值
    convertScaleAbs(grayY, grayY);

    imshow("grayX", grayX);
    imshow("grayY", grayY);

    addWeighted(grayX, 0.5, grayY, 0.5, 0, dst);

    imshow("dst", dst);
}

void Convolution::LaplanceTest()
{
    imshow("src", src);
    Mat dst, gray, gaussian, laplanceMat;

    GaussianBlur(src, gaussian, Size(3,3), 0);
    cvtColor(gaussian, gray, CV_BGR2GRAY);

    imshow("gray", gray);

    Laplacian(gray, laplanceMat, CV_16S, 3);
    convertScaleAbs(laplanceMat, laplanceMat);

    threshold(laplanceMat, laplanceMat, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);

    imshow("laplanceMat", laplanceMat);
}

int tValue = 50;
int tMax = 255;

Mat inMat, dst;
void ChnnyDemo(int, void*)
{
    Canny(inMat, dst, tValue, tValue*3, 3);
    imshow("Chnny", dst);

}

void Convolution::ChnnyTset()
{
    imshow("src", src);
    namedWindow("Chnny", CV_WINDOW_AUTOSIZE);
    Mat dst, gray, gaussian;

    cvtColor(src, gray, CV_BGR2GRAY);

    GaussianBlur(gray, gaussian, Size(3,3), 0);
    gaussian.copyTo(inMat);

    createTrackbar("Chnny T1 Value:", "Chnny", &tValue, tMax, ChnnyDemo);
    ChnnyDemo(0,0);
    waitKey(0);
}

void Convolution::HoughTransfromTest()
{
    imshow("src", src);
    Mat gray, dstMat;
    // 边缘检测
    Canny(src, gray, 100, 200, 5);
    imshow("grayMat", gray);

    // 直线提取
    std::vector<Vec4f>  pLines;
    HoughLinesP(gray, pLines, 1, CV_PI/180.0, 100, 0, 20);

    src.copyTo(dstMat, ~gray);
    Scalar color = Scalar(0, 0, 255);
    for (size_t lineIndex = 0; lineIndex < pLines.size(); lineIndex++)
    {
        Vec4f  hLine = pLines[lineIndex];
        line(dstMat, Point(hLine[0], hLine[1]), Point(hLine[2], hLine[3]), color, 1, LINE_4);
    }

    imshow("dstMat", dstMat);

    // 霍夫圆检测
    Mat srcC, midC, dstC;
    srcC = imread("D:\\Data\\Code_Workspace\\imagesTestEx\\circles.jpg");
    if (srcC.empty()){
        qDebug() << "Image D:\\Data\\Code_Workspace\\imagesTestEx\\circles.jpg read faile!";
        return ;
    }
    imshow("霍夫圆检测原图",srcC);
    srcC.copyTo(dstC);

    medianBlur(srcC, midC, 3);// 中值模糊
    cvtColor(midC, midC, CV_BGR2GRAY);
    std::vector<Vec3f> pcirclrs;
    HoughCircles(midC, pcirclrs, CV_HOUGH_GRADIENT, 1, 20, 100, 30, 5, 150);

    for (size_t circleIndex = 0; circleIndex<pcirclrs.size(); circleIndex++){
        Vec3f Hcircle = pcirclrs[circleIndex];
        circle(dstC, Point(Hcircle[0], Hcircle[1]), Hcircle[2], Scalar(0,0,255), 2, LINE_AA);
        circle(dstC, Point(Hcircle[0], Hcircle[1]), 1, Scalar(100,100,100), 2, LINE_AA);
    }
    imshow("霍夫圆检测", dstC);

    waitKey(0);
}


