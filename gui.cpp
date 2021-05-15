#include <opencv2/opencv.hpp>

using namespace cv;

void initUI(){
    Mat background = Mat(Size(256,256), CV_8U);
    imshow("display", background);
    waitKey(0);
}