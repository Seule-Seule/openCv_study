#include "threshold.hpp"
#include <QDebug>

int threshold_value;
int threshold_max;
int type_value;
int type_max;
Mat src;

Threshold::Threshold()
{
    threshold_value = 127;
    threshold_max = 255;
    type_value = 2;
    type_max = 4;
}

void Threshold::ThresholdTest()
{

    src = imread("D:\\Data\\Code_Workspace\\imagesTestEx\\test2.jpg");
    if (src.empty()){
        qDebug() << "Image D:\\Data\\Code_Workspace\\imagesTestEx\\test2.jpg  read faile!";
        return ;
    }
    namedWindow("input window", CV_WINDOW_AUTOSIZE);
    namedWindow("output window", CV_WINDOW_AUTOSIZE);
    imshow("input window", src);

    cv::createTrackbar("threshold", "input window", &threshold_value, threshold_max, ThresholdDemo);
    cv::createTrackbar("type", "input window", &type_value, type_max, ThresholdDemo);

    waitKey(0);
}

void Threshold::ThresholdDemo(int, void*)
{
    Mat braySrc, dst;
    cvtColor(src, braySrc, CV_BGR2GRAY);
    cv::threshold(braySrc, dst, threshold_value, threshold_max, type_value);
    imshow("output window", dst);
}
