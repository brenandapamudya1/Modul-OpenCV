/*
    * Single Camera Calibration menggunakan chessboard pattern.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
	cv::Size boardSize(9, 6);
	float squareSize = 25.0f;
	std::string imageDir = "../assets/calib/";
	std::string outputPath = "yaml/kalibrasi.yaml";

	// Daftar gambar kalibrasi
	std::vector<std::string> imageFiles;
	std::vector<int> indices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14};
	for (int idx : indices) {
		char filename[32];
		snprintf(filename, sizeof(filename), "left%02d.jpg", idx);
		imageFiles.push_back(imageDir + filename);
	}

	// Object points (koordinat 3D chessboard)
	std::vector<cv::Point3f> objp;
	for (int i = 0; i < boardSize.height; i++)
		for (int j = 0; j < boardSize.width; j++)
			objp.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));

	std::vector<std::vector<cv::Point3f>> objectPoints;
	std::vector<std::vector<cv::Point2f>> imagePoints;
	cv::Size imageSize;

	// Deteksi chessboard corners
	for (size_t i = 0; i < imageFiles.size(); i++) {
		cv::Mat image = cv::imread(imageFiles[i]);
		if (image.empty()) continue;

		if (imageSize.empty()) imageSize = image.size();

		cv::Mat gray;
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

		std::vector<cv::Point2f> corners;
		bool found = cv::findChessboardCorners(gray, boardSize, corners,
			cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK
		);

		if (found) {
			cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
				cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001)
			);
			objectPoints.push_back(objp);
			imagePoints.push_back(corners);
		}
	}

	// Kalibrasi
	cv::Mat cameraMatrix, distCoeffs;
	std::vector<cv::Mat> rvecs, tvecs;
	double rmsError = cv::calibrateCamera(
		objectPoints, imagePoints, imageSize,
		cameraMatrix, distCoeffs, rvecs, tvecs
	);

	// Simpan hasil ke YAML
	cv::FileStorage fs(outputPath, cv::FileStorage::WRITE);
	if (!fs.isOpened()) {
		std::cerr << "Tidak bisa membuat file " << outputPath << std::endl;
		return -1;
	}

	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;
	fs << "board_width" << boardSize.width;
	fs << "board_height" << boardSize.height;
	fs << "square_size" << squareSize;
	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;
	fs << "reprojection_error" << rmsError;
	fs << "images_used" << (int)objectPoints.size();
	fs.release();

	std::cout << "RMS Error: " << rmsError << " px" << std::endl;
	std::cout << "Disimpan ke: " << outputPath << std::endl;

	// Visualisasi: Original vs Undistorted
	cv::Mat sample = cv::imread(imageFiles[0]);
	if (!sample.empty()) {
		cv::Mat undistorted;
		cv::undistort(sample, undistorted, cameraMatrix, distCoeffs);

		cv::Mat comparison;
		cv::hconcat(sample, undistorted, comparison);
		cv::putText(comparison, "Original", cv::Point(10, 30),
			cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
		cv::putText(comparison, "Undistorted", cv::Point(sample.cols + 10, 30),
			cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

		cv::imshow("Kalibrasi: Original vs Undistorted", comparison);
		cv::waitKey(0);
	}

	return 0;
}
