#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("../assets/photos/sample.jpg");
	if (image.empty()){
		std::cerr<<"Couldn't open or find the image"<<std::endl;
		return -1;
	}
	// tambahkan variable roi
    cv::Rect ROI(300, 200, 640, 480); // x, y, width, height
	cv::Mat roi_image = image(ROI);

	// simpan hasil ke folder result
	cv::imwrite("../assets/result/roi.jpg", roi_image);

    // perbedaannya
    cv::imshow("Gambar", image);
    cv::imshow("Region of Interest", roi_image);
    cv::waitKey(0);
	return 0;
}
