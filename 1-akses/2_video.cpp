#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap("../assets/videos/sample.mp4");
    cv::Mat frame;

    while (cap.isOpened()) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cv::imshow("Video", frame);

        if (cv::waitKey(25) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
