#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <opencv2/opencv.hpp>

using namespace cv;

class Convolution
{
public:
    Convolution();
public:
    void ConvolutionTest();

    // 卷积边缘处理
    void ConvolutionTestEdge();

    // Sobel算子
    void SobelTest();

    // Laplance 算子
    void LaplanceTest();

    // Chnny 边缘检测
    void ChnnyTset();

    // 霍夫变换
    void HoughTransfromTest();
private:
    Mat src;

    Mat RebortX ;
    Mat RebortY ;
    Mat SobelX ;
    Mat SobelY ;
    Mat Lapulasi;
};

#endif // CONVOLUTION_HPP
