/*
    * Monocular Depth Estimation (Single Camera Pinhole Model)
    * Z = (fx * W_real) / w_pixel
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
	std::string calibPath = "yaml/kalibrasi.yaml";
	std::string imagePath = "../assets/calib/left01.jpg";
	std::string outputPath = "../assets/result/4-bev/depth_pinhole.jpg";
	std::string yamlPath = "yaml/depth.yaml";

	// 1. Baca parameter kalibrasi kamera
	cv::FileStorage fs(calibPath, cv::FileStorage::READ);
	if (!fs.isOpened()) {
		std::cerr << "Gagal membuka file kalibrasi: " << calibPath << std::endl;
		return -1;
	}

	double fx = fs["focal_length_x"];
	double fy = fs["focal_length_y"];
	double cx = fs["principal_point_x"];
	double cy = fs["principal_point_y"];
	int boardWidth = fs["board_width"];
	int boardHeight = fs["board_height"];
	float squareSize = fs["square_size"]; // mm
	fs.release();

	// 2. Baca gambar input
	cv::Mat img = cv::imread(imagePath);
	if (img.empty()) {
		std::cerr << "Gagal membuka gambar: " << imagePath << std::endl;
		return -1;
	}

	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	// 3. Deteksi objek referensi (Chessboard pattern)
	cv::Size boardSize(boardWidth, boardHeight);
	std::vector<cv::Point2f> corners;
	int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK;
	bool found = cv::findChessboardCorners(gray, boardSize, corners, flags);

	if (!found) {
		std::cerr << "Chessboard tidak terdeteksi pada gambar." << std::endl;
		return -1;
	}

	cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001);
	cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

	// Ukuran fisik asli objek referensi (jarak dari corner pertama ke corner terakhir pada baris atas)
	// (boardWidth - 1) * squareSize = (9 - 1) * 25.0 mm = 200 mm
	float realWidthMM = (boardWidth - 1) * squareSize;

	// Mengukur lebar objek pada piksel citra (jarak piksel antar corner horizontal)
	cv::Point2f topLeft = corners[0];
	cv::Point2f topRight = corners[boardWidth - 1];
	float pixelWidth = cv::norm(topRight - topLeft);

	// 4. Hitung Jarak/Kedalaman Z (Pinhole Camera Model)
	// Z = (fx * W_real) / w_pixel
	double depthZ = (fx * realWidthMM) / pixelWidth;

	// Pusat objek dalam piksel
	cv::Rect bbox = cv::boundingRect(corners);
	cv::Point2f centerPixel(bbox.x + bbox.width / 2.0f, bbox.y + bbox.height / 2.0f);

	// Hitung koordinat 3D (X, Y, Z) dalam mm
	double posX = ((centerPixel.x - cx) * depthZ) / fx;
	double posY = ((centerPixel.y - cy) * depthZ) / fy;

	// 5. Visualisasi Bounding Box, Pusat Objek, dan Teks Depth
	cv::Mat result = img.clone();
	cv::drawChessboardCorners(result, boardSize, corners, found);
	cv::rectangle(result, bbox, cv::Scalar(0, 255, 0), 2);
	cv::circle(result, centerPixel, 5, cv::Scalar(0, 0, 255), -1);

	char depthStr[64], posStr[64];
	snprintf(depthStr, sizeof(depthStr), "Depth Z: %.1f mm (%.2f m)", depthZ, depthZ / 1000.0);
	snprintf(posStr, sizeof(posStr), "3D Pos: X=%.1f, Y=%.1f, Z=%.1f mm", posX, posY, depthZ);

	cv::putText(result, depthStr, cv::Point(bbox.x, std::max(bbox.y - 25, 25)),
		cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
	cv::putText(result, posStr, cv::Point(bbox.x, std::max(bbox.y - 8, 40)),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1);

	cv::imwrite(outputPath, result);

	// 6. Simpan data depth ke YAML
	cv::FileStorage fsOut(yamlPath, cv::FileStorage::WRITE);
	if (fsOut.isOpened()) {
		fsOut << "image_path" << imagePath;
		fsOut << "reference_width_real_mm" << realWidthMM;
		fsOut << "reference_width_pixel" << pixelWidth;
		fsOut << "focal_length_x" << fx;
		fsOut << "depth_z_mm" << depthZ;
		fsOut << "depth_z_meter" << (depthZ / 1000.0);
		fsOut << "position_3d_x_mm" << posX;
		fsOut << "position_3d_y_mm" << posY;
		fsOut << "position_3d_z_mm" << depthZ;
		fsOut.release();
	}

	std::cout << "--- Single Camera Pinhole Depth Estimation ---" << std::endl;
	std::cout << "Focal Length (fx): " << fx << " px" << std::endl;
	std::cout << "Lebar Objek Fisik (W_real): " << realWidthMM << " mm" << std::endl;
	std::cout << "Lebar Objek Piksel (w_pixel): " << pixelWidth << " px" << std::endl;
	std::cout << "Kedalaman Terhitung (Z): " << depthZ << " mm (" << (depthZ / 1000.0) << " m)" << std::endl;
	std::cout << "Posisi 3D: X=" << posX << " mm, Y=" << posY << " mm, Z=" << depthZ << " mm" << std::endl;
	std::cout << "Output gambar tersimpan di: " << outputPath << std::endl;
	std::cout << "Data depth tersimpan di: " << yamlPath << std::endl;

	return 0;
}
