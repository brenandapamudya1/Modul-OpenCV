#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("../assets/photos/sample2.jpg");

    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::imshow("Gambar", image);
    cv::waitKey(0);

    return 0;
}
