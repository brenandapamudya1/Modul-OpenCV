/*
    * Stereo Depth Estimation menggunakan StereoSGBM.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main() {
	std::string calibPath = "yaml/kalibrasi.yaml";
	std::string leftPath = "../assets/calib/left01.jpg";
	std::string rightPath = "../assets/calib/right01.jpg";
	std::string outputDir = "../assets/result/4-bev/";
	std::string depthOutput = "yaml/depth.yaml";

	// Load parameter kalibrasi
	cv::FileStorage fs(calibPath, cv::FileStorage::READ);
	if (!fs.isOpened()) {
		std::cerr << "Tidak bisa membuka " << calibPath << std::endl;
		return -1;
	}

	cv::Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR;
	cv::Mat R1, R2, P1, P2, Q;

	fs["camera_matrix_left"] >> cameraMatrixL;
	fs["distortion_coefficients_left"] >> distCoeffsL;
	fs["camera_matrix_right"] >> cameraMatrixR;
	fs["distortion_coefficients_right"] >> distCoeffsR;
	fs["R1"] >> R1;
	fs["R2"] >> R2;
	fs["P1"] >> P1;
	fs["P2"] >> P2;
	fs["Q"] >> Q;
	fs.release();

	// Load gambar stereo
	cv::Mat imgL = cv::imread(leftPath);
	cv::Mat imgR = cv::imread(rightPath);
	if (imgL.empty() || imgR.empty()) {
		std::cerr << "Tidak bisa membuka gambar stereo" << std::endl;
		return -1;
	}

	// Rectify gambar
	cv::Mat mapL1, mapL2, mapR1, mapR2;
	cv::initUndistortRectifyMap(cameraMatrixL, distCoeffsL, R1, P1, imgL.size(), CV_16SC2, mapL1, mapL2);
	cv::initUndistortRectifyMap(cameraMatrixR, distCoeffsR, R2, P2, imgR.size(), CV_16SC2, mapR1, mapR2);

	cv::Mat rectL, rectR;
	cv::remap(imgL, rectL, mapL1, mapL2, cv::INTER_LINEAR);
	cv::remap(imgR, rectR, mapR1, mapR2, cv::INTER_LINEAR);

	// Convert ke grayscale
	cv::Mat grayL, grayR;
	cv::cvtColor(rectL, grayL, cv::COLOR_BGR2GRAY);
	cv::cvtColor(rectR, grayR, cv::COLOR_BGR2GRAY);

	// StereoSGBM
	int numDisparities = 16 * 5;
	int blockSize = 5;
	auto stereo = cv::StereoSGBM::create(
		0, numDisparities, blockSize,
		8 * 3 * blockSize * blockSize,      // P1
		32 * 3 * blockSize * blockSize,     // P2
		1, 63, 10, 100, 32,
		cv::StereoSGBM::MODE_SGBM_3WAY
	);

	cv::Mat disparity;
	stereo->compute(grayL, grayR, disparity);

	// Normalize disparity untuk visualisasi
	cv::Mat disparityGray, disparityColor;
	cv::normalize(disparity, disparityGray, 0, 255, cv::NORM_MINMAX, CV_8U);
	cv::applyColorMap(disparityGray, disparityColor, cv::COLORMAP_JET);

	// Konversi disparity ke depth 3D
	cv::Mat dispFloat;
	disparity.convertTo(dispFloat, CV_32F, 1.0 / 16.0);
	cv::Mat depth3D;
	cv::reprojectImageTo3D(dispFloat, depth3D, Q, true);

	// Depth values di 5 titik sample
	int h = imgL.rows, w = imgL.cols;
	cv::Point2i samplePoints[] = {
		{w / 2, h / 2},         // Center
		{w / 4, h / 4},         // Top-left
		{3 * w / 4, h / 4},    // Top-right
		{w / 4, 3 * h / 4},    // Bottom-left
		{3 * w / 4, 3 * h / 4}  // Bottom-right
	};
	std::string pointNames[] = {"Center", "Top-Left", "Top-Right", "Bottom-Left", "Bottom-Right"};

	std::cout << "--- Depth Values ---" << std::endl;
	for (int i = 0; i < 5; i++) {
		cv::Vec3f point = depth3D.at<cv::Vec3f>(samplePoints[i].y, samplePoints[i].x);
		float d = dispFloat.at<float>(samplePoints[i].y, samplePoints[i].x);
		std::cout << pointNames[i] << " (" << samplePoints[i].x << "," << samplePoints[i].y << ")"
				  << " -> X=" << point[0] << " Y=" << point[1] << " Z=" << point[2]
				  << " mm (disp=" << d << ")" << std::endl;
	}

	// Simpan hasil
	cv::imwrite(outputDir + "disparity_gray.jpg", disparityGray);
	cv::imwrite(outputDir + "disparity_color.jpg", disparityColor);

	// Simpan depth data ke YAML
	cv::FileStorage dfs(depthOutput, cv::FileStorage::WRITE);
	if (dfs.isOpened()) {
		dfs << "source_left" << leftPath;
		dfs << "source_right" << rightPath;
		dfs << "num_disparities" << numDisparities;
		dfs << "block_size" << blockSize;
		dfs << "Q_matrix" << Q;
		dfs << "depth_3d" << depth3D;
		dfs.release();
	}

	std::cout << std::endl;
	std::cout << "Disparity gray: " << outputDir + "disparity_gray.jpg" << std::endl;
	std::cout << "Disparity color: " << outputDir + "disparity_color.jpg" << std::endl;
	std::cout << "Depth data: " << depthOutput << std::endl;

	// Visualisasi
	cv::imshow("Left Rectified", rectL);
	cv::imshow("Disparity (Grayscale)", disparityGray);
	cv::imshow("Disparity (Colormap)", disparityColor);
	cv::waitKey(0);

	return 0;
}
