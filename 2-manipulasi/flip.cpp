#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/photos/sample.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// Variabel Flipped
	cv::Mat flipped;
	
	// Function Flip
	// 0 = flip vertically, 1 = flip horizontally, -1 = flip both vertically and horizontally
	cv::flip(image, flipped, 1);

	// Simpan Hasil
	cv::imwrite("../assets/result/flipped.jpg", flipped);

	// Perbedaan
	cv::imshow("Gambar", image);
	cv::imshow("Flipped", flipped);
	cv::waitKey(0);
	return 0;
}