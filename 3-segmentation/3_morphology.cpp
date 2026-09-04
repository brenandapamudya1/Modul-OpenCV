
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/sample2.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// Variabel morphImage
    cv::Mat morphImage = image.clone();

    // Function Morphological Erode & Dilate
	cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 2);
	cv::dilate(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 8);
	cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 5);

	// Simpan Hasil
	cv::imwrite("../assets/result/3-segmentation/morphology_result.jpg", morphImage);

    // Perbedaan
    cv::imshow("Real Image", image);
    cv::imshow("Morphology Result", morphImage);

	cv::waitKey(0);
	return 0;
}