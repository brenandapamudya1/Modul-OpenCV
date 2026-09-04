/*
 *  File ini merupakan gabungan dari 3 file sebelumnya,
    *  yaitu convert.cpp, thresholding.cpp, dan morphology.cpp
    *  serta penambahan fungsi untuk mendeteksi kontur dan membuat bounding box.
*/

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/photos/sample2.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
    // 1. Convert Color
	// Variabel hlsImage
	cv::Mat hlsImage;

	// Function Convert Color
	cv::cvtColor(image, hlsImage, cv::COLOR_BGR2HLS);

    // 2. Thresholding
    // Variabel thresholdedImage
    cv::Mat thresholdedImage;

    // Function Thresholding
    cv::inRange(hlsImage, cv::Scalar(11, 42, 61), cv::Scalar(255, 211, 255), thresholdedImage);

    // 3. Morphological Erode & Dilate
    // Variabel morphImage
    cv::Mat morphImage = thresholdedImage.clone();

    // Function Morphological Erode & Dilate
    cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 8);
    cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 5);

    // 4. Contour Detection
    std::vector<std::vector<cv::Point>> contours;

    // Function Find Contours
    cv::findContours(morphImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 5. Bounding Box
    // Variabel Hasil
    cv::Mat result = image.clone();

    // Function Draw Bounding Box
    for (size_t i = 0; i < contours.size(); i++) {
        if(cv::contourArea(contours[i]) > 1000) { // Filter area
            cv::Rect boundingBox = cv::boundingRect(contours[i]);
            cv::rectangle(result, boundingBox, cv::Scalar(0, 255, 0), 2);
        }
    }

    // Simpan Hasil
    cv::imwrite("../assets/result/3-segmentation/final_result.jpg", result);

	// Perbedaan
	cv::imshow("Real Image", image);
    cv::imshow("Morphological Image", morphImage);
    cv::imshow("Bounding Box Result", result);

	cv::waitKey(0);
	return 0;
}