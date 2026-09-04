#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/photos/sample2.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// Variabel hlsImage
	cv::Mat hlsImage;

	// Function Convert Color
	cv::cvtColor(image, hlsImage, cv::COLOR_BGR2HLS);

	// Simpan Hasil
	cv::imwrite("../assets/result/3-segmentation/convert_result.jpg", hlsImage);

	// Perbedaan
	cv::imshow("Real Image", image);
	cv::imshow("HLS Image", hlsImage);

	cv::waitKey(0);
	return 0;
}
