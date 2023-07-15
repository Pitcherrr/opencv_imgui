#include <iostream>
#include <thread>
#include <atomic>
#include <opencv2/opencv.hpp>

std::atomic<bool> stopCapture(false);
std::atomic<cv::Mat*> sharedFrame(nullptr);

void captureThread()
{
    cv::VideoCapture cap(0);
    double des_fps = 30;
    cap.set(cv::CAP_PROP_FPS, des_fps);
    std::cout << cap.set(cv::CAP_PROP_FPS, des_fps) << std::endl;

    if (!cap.isOpened())
    {
        stopCapture = true;
        return;
    }

    cv::Mat frame;
    std::cout << cap.set(5, des_fps) << std::endl;

    while (!stopCapture)
    {
        cap >> frame;
        sharedFrame.store(&frame);
    }
}

int main()
{
    std::cout << "Starting Main" << std::endl;
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);
    std::thread captureThreadObj(captureThread);

    // captureThreadObj.join();

    while (!stopCapture)
    {
        cv::Mat* framePtr = sharedFrame.load();
        if (framePtr)
        {
            cv::imshow("Webcam", *framePtr);
            sharedFrame.store(nullptr);
            // std::cout << stopCapture << std::endl;
        }

        if (cv::waitKey(33) >= 0) break;
    }

    stopCapture = true;

    captureThreadObj.join();

    return 0;
}