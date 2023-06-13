
#include "opencv2/opencv.hpp"

int main(int, char**){
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;
    cv::Mat frame;
    for (;;)
    {
        cap >> frame;
        cv::imshow("Webcam", frame);
        if (cv::waitKey(33) >= 0) break;
    }
return 0;
}