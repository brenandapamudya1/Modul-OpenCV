
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/sample2.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// Variabel thresholdedImage
    cv::Mat thresholdedImage;

    // Function Thresholding
    cv::inRange(image, cv::Scalar(11, 42, 61), cv::Scalar(255, 211, 255), thresholdedImage);

    // Simpan Hasil
    cv::imwrite("../assets/result/3-segmentation/thresholding_result.jpg", thresholdedImage);

    // Perbedaan
    cv::imshow("Real Image", image);
    cv::imshow("Thresholded Image", thresholdedImage);

	cv::waitKey(0);
	return 0;
}