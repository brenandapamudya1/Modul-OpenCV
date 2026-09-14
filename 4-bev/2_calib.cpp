/*
    * Single Camera Calibration (Pinhole Camera Model)
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
	cv::Size boardSize(9, 6);
	float squareSize = 25.0f; // mm
	std::string imageDir = "../assets/calib/";
	std::string outputPath = "yaml/kalibrasi.yaml";

	std::vector<int> indices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14};
	std::vector<std::string> imageFiles;
	for (int idx : indices) {
		char fname[32];
		snprintf(fname, sizeof(fname), "left%02d.jpg", idx);
		imageFiles.push_back(imageDir + fname);
	}

	std::vector<cv::Point3f> objp;
	for (int i = 0; i < boardSize.height; i++)
		for (int j = 0; j < boardSize.width; j++)
			objp.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));

	std::vector<std::vector<cv::Point3f>> objectPoints;
	std::vector<std::vector<cv::Point2f>> imagePoints;
	cv::Size imageSize;

	for (const auto& file : imageFiles) {
		cv::Mat img = cv::imread(file);
		if (img.empty()) continue;

		if (imageSize.empty()) imageSize = img.size();

		cv::Mat gray;
		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

		std::vector<cv::Point2f> corners;
		int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK;
		bool found = cv::findChessboardCorners(gray, boardSize, corners, flags);

		if (found) {
			cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001);
			cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);
			objectPoints.push_back(objp);
			imagePoints.push_back(corners);
		}
	}

	cv::Mat cameraMatrix, distCoeffs;
	std::vector<cv::Mat> rvecs, tvecs;

	double rms = cv::calibrateCamera(objectPoints, imagePoints, imageSize,
		cameraMatrix, distCoeffs, rvecs, tvecs);

	cv::FileStorage fs(outputPath, cv::FileStorage::WRITE);
	if (!fs.isOpened()) return -1;

	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;
	fs << "board_width" << boardSize.width;
	fs << "board_height" << boardSize.height;
	fs << "square_size" << squareSize;
	fs << "images_used" << (int)objectPoints.size();
	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;
	fs << "focal_length_x" << cameraMatrix.at<double>(0, 0);
	fs << "focal_length_y" << cameraMatrix.at<double>(1, 1);
	fs << "principal_point_x" << cameraMatrix.at<double>(0, 2);
	fs << "principal_point_y" << cameraMatrix.at<double>(1, 2);
	fs << "reprojection_error" << rms;
	fs.release();

	std::cout << "RMS Reprojection Error: " << rms << " px" << std::endl;
	std::cout << "Focal Length (fx, fy): " << cameraMatrix.at<double>(0, 0) << ", " << cameraMatrix.at<double>(1, 1) << std::endl;
	std::cout << "Principal Point (cx, cy): " << cameraMatrix.at<double>(0, 2) << ", " << cameraMatrix.at<double>(1, 2) << std::endl;
	std::cout << "Hasil kalibrasi tersimpan di: " << outputPath << std::endl;

	return 0;
}
