#include "opencv2/opencv.hpp"

int main(int, char**){
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap(0);
    double des_fps = 30; 
    cap.set(cv::CAP_PROP_FPS, des_fps);
    std::cout << cap.set(cv::CAP_PROP_FPS, des_fps) << std::endl;

    if(!cap.isOpened())
        return -1;
    cv::Mat frame;

    std::cout << cap.set(5, des_fps) << std::endl;
    for (;;)
    {
        cap >> frame;
        cv::imshow("Webcam", frame);
        double fps = cap.get(cv::CAP_PROP_FPS);
        // std::cout << cap.get(cv::CAP_PROP_FPS) << std::endl;
        if (cv::waitKey(33) >= 0) break;
    }
return 0;
}