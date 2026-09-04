#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/sample.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// tambahkan variable resized
	cv::Mat resized;
	// function resize
	cv::resize(image, resized, cv::Size(640, 480));

	// show perbedaan imagenya
	cv::imshow("Gambar", image);
	cv::imshow("Resized", resized);
	cv::waitKey(0);
	return 0;
}
