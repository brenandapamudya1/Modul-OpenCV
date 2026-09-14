/*
    * Stereo Camera Calibration menggunakan chessboard pattern.
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
	std::vector<int> indices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14};
	std::vector<std::string> leftFiles, rightFiles;
	for (int idx : indices) {
		char lname[32], rname[32];
		snprintf(lname, sizeof(lname), "left%02d.jpg", idx);
		snprintf(rname, sizeof(rname), "right%02d.jpg", idx);
		leftFiles.push_back(imageDir + lname);
		rightFiles.push_back(imageDir + rname);
	}

	// Object points (koordinat 3D chessboard)
	std::vector<cv::Point3f> objp;
	for (int i = 0; i < boardSize.height; i++)
		for (int j = 0; j < boardSize.width; j++)
			objp.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));

	std::vector<std::vector<cv::Point3f>> objectPoints;
	std::vector<std::vector<cv::Point2f>> imagePointsL, imagePointsR;
	cv::Size imageSize;

	// Deteksi chessboard corners pada kedua kamera
	for (size_t i = 0; i < leftFiles.size(); i++) {
		cv::Mat imgL = cv::imread(leftFiles[i]);
		cv::Mat imgR = cv::imread(rightFiles[i]);
		if (imgL.empty() || imgR.empty()) continue;

		if (imageSize.empty()) imageSize = imgL.size();

		cv::Mat grayL, grayR;
		cv::cvtColor(imgL, grayL, cv::COLOR_BGR2GRAY);
		cv::cvtColor(imgR, grayR, cv::COLOR_BGR2GRAY);

		std::vector<cv::Point2f> cornersL, cornersR;
		int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK;
		bool foundL = cv::findChessboardCorners(grayL, boardSize, cornersL, flags);
		bool foundR = cv::findChessboardCorners(grayR, boardSize, cornersR, flags);

		if (foundL && foundR) {
			cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001);
			cv::cornerSubPix(grayL, cornersL, cv::Size(11, 11), cv::Size(-1, -1), criteria);
			cv::cornerSubPix(grayR, cornersR, cv::Size(11, 11), cv::Size(-1, -1), criteria);
			objectPoints.push_back(objp);
			imagePointsL.push_back(cornersL);
			imagePointsR.push_back(cornersR);
		}
	}

	// Single camera calibration (left & right)
	cv::Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR;
	std::vector<cv::Mat> rvecsL, tvecsL, rvecsR, tvecsR;

	double rmsL = cv::calibrateCamera(objectPoints, imagePointsL, imageSize,
		cameraMatrixL, distCoeffsL, rvecsL, tvecsL);
	double rmsR = cv::calibrateCamera(objectPoints, imagePointsR, imageSize,
		cameraMatrixR, distCoeffsR, rvecsR, tvecsR);

	// Stereo calibration
	cv::Mat R, T, E, F;
	double rmsStereo = cv::stereoCalibrate(
		objectPoints, imagePointsL, imagePointsR,
		cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR,
		imageSize, R, T, E, F,
		cv::CALIB_FIX_INTRINSIC,
		cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 100, 1e-6)
	);

	// Stereo rectification
	cv::Mat R1, R2, P1, P2, Q;
	cv::stereoRectify(
		cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR,
		imageSize, R, T, R1, R2, P1, P2, Q,
		cv::CALIB_ZERO_DISPARITY, -1, imageSize
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
	fs << "images_used" << (int)objectPoints.size();

	// Left camera
	fs << "camera_matrix_left" << cameraMatrixL;
	fs << "distortion_coefficients_left" << distCoeffsL;
	fs << "reprojection_error_left" << rmsL;

	// Right camera
	fs << "camera_matrix_right" << cameraMatrixR;
	fs << "distortion_coefficients_right" << distCoeffsR;
	fs << "reprojection_error_right" << rmsR;

	// Stereo params
	fs << "R" << R;
	fs << "T" << T;
	fs << "E" << E;
	fs << "F" << F;
	fs << "reprojection_error_stereo" << rmsStereo;

	// Rectification params
	fs << "R1" << R1;
	fs << "R2" << R2;
	fs << "P1" << P1;
	fs << "P2" << P2;
	fs << "Q" << Q;

	fs.release();

	std::cout << "RMS Left: " << rmsL << " px" << std::endl;
	std::cout << "RMS Right: " << rmsR << " px" << std::endl;
	std::cout << "RMS Stereo: " << rmsStereo << " px" << std::endl;
	std::cout << "Disimpan ke: " << outputPath << std::endl;

	// Visualisasi: Rectified pair
	cv::Mat mapL1, mapL2, mapR1, mapR2;
	cv::initUndistortRectifyMap(cameraMatrixL, distCoeffsL, R1, P1, imageSize, CV_16SC2, mapL1, mapL2);
	cv::initUndistortRectifyMap(cameraMatrixR, distCoeffsR, R2, P2, imageSize, CV_16SC2, mapR1, mapR2);

	cv::Mat sampleL = cv::imread(leftFiles[0]);
	cv::Mat sampleR = cv::imread(rightFiles[0]);
	if (!sampleL.empty() && !sampleR.empty()) {
		cv::Mat rectL, rectR;
		cv::remap(sampleL, rectL, mapL1, mapL2, cv::INTER_LINEAR);
		cv::remap(sampleR, rectR, mapR1, mapR2, cv::INTER_LINEAR);

		cv::Mat comparison;
		cv::hconcat(rectL, rectR, comparison);
		for (int y = 0; y < comparison.rows; y += 32)
			cv::line(comparison, cv::Point(0, y), cv::Point(comparison.cols, y), cv::Scalar(0, 255, 0));

		cv::imshow("Rectified Stereo Pair", comparison);
		cv::waitKey(0);
	}

	return 0;
}
