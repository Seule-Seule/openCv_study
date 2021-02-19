﻿#include "convolution.hpp"
#include <QDebug>

Convolution::Convolution()
{

    src = imread("D:\\Data\\Code_Workspace\\imagesTestEx\\test2.jpg");
    if (src.empty()){
        qDebug() << "Image D:\\Data\\Code_Workspace\\imagesTestEx\\test2.jpg  read faile!";
        return ;
    }
    RebortX = (Mat_<int>(2,2) << 1, 0, 0, -1);
    RebortY = (Mat_<int>(2,2) << 0, 1, -1, 0);
    SobelX = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    SobelY = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Lapulasi = (Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);

    namedWindow("src", CV_WINDOW_AUTOSIZE);
    namedWindow("RebortX", CV_WINDOW_AUTOSIZE);
    namedWindow("RebortY", CV_WINDOW_AUTOSIZE);
    namedWindow("SobelX", CV_WINDOW_AUTOSIZE);
    namedWindow("SobelY", CV_WINDOW_AUTOSIZE);
    namedWindow("Lapulasi", CV_WINDOW_AUTOSIZE);

    imshow("src", src);
}

void Convolution::ConvolutionTest()
{
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
