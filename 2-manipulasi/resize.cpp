#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/photos/sample.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// Variabel Resized
	cv::Mat resized;

	// Function Resize
	cv::resize(image, resized, cv::Size(640, 480));

	// Simpan Hasil
	cv::imwrite("../assets/result/2-manipulasi/resize.jpg", resized);

	// Perbedaan
	cv::imshow("Gambar", image);
	cv::imshow("Resized", resized);
	cv::waitKey(0);
	return 0;
}
