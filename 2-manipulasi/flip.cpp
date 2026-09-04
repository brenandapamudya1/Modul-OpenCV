#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/photos/sample.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// tambahkan variable flipped
	cv::Mat flipped;
	// function flip sesuai arah
    // 0 = flip vertikal, 1 = flip horizontal, -1 = flip keduanya
	cv::flip(image, flipped, 1);

	// simpan hasil ke folder result
	cv::imwrite("../assets/result/flipped.jpg", flipped);

	// show perbedaan imagenya
	cv::imshow("Gambar", image);
	cv::imshow("Flipped", flipped);
	cv::waitKey(0);
	return 0;
}