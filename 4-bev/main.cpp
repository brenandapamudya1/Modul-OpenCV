/*
    * ini adalah contoh penggunaan code untuk sample jalannya.
*/

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("../assets/photos/road3.jpeg");
    if(image.empty()){
        std::cerr << "Couldn't find image!" << std::endl;
    }

    // Resize
    cv::resize(image, image, cv::Size(
        (int)(image.size().width / 2),
        (int)(image.size().height / 2)
    ));

    // Definisi ROI
    cv::Rect ROI_area(
        0,
        (int)(image.rows / 1.8),
        image.cols,
        (int)(image.rows / 2.5)
    );

    // Deteksi Garis Kuning
    cv::Mat yellow_thresholded;
    cv::cvtColor(image(ROI_area), yellow_thresholded, cv::COLOR_BGR2HLS);
    cv::inRange(
        yellow_thresholded,
        cv::Scalar(15, 30, 80),
        cv::Scalar(35, 200, 255),
        yellow_thresholded
    );

    // Deteksi Garis Putih
    cv::Mat white_thresholded;
    cv::cvtColor(image(ROI_area), white_thresholded, cv::COLOR_BGR2HLS);
    cv::inRange(
        white_thresholded,
        cv::Scalar(0, 180, 0),
        cv::Scalar(255, 255, 60),
        white_thresholded
    );

    // Gabungkan Hasil Deteksi Kuning & Putih
    cv::Mat combined;
    cv::bitwise_or(yellow_thresholded, white_thresholded, combined);

    // Morfologi untuk noise
    cv::erode(combined, combined, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(combined, combined, cv::Mat(), cv::Point(-1, -1), 8);
    cv::erode(combined, combined, cv::Mat(), cv::Point(-1, -1), 5);

    // Menerapkan hasil threshold ke gambar asli
    cv::Mat image_thresholded = cv::Mat::zeros(image.size(), CV_8UC1);
    combined.copyTo(image_thresholded(ROI_area));

    // Mencari kontur
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(image_thresholded, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Mengambil titik sudut dari kontur terbesar
    cv::Mat segment = image.clone();
    std::vector<cv::Point> titik_sudut;

    for (size_t i = 0; i < contours.size(); i++){
        if (cv::contourArea(contours[i]) > 500) {
            cv::approxPolyDP(
                contours[i],
                titik_sudut,
                0.01 * cv::arcLength(contours[i], true),
                true
            );
            break;
        }
    }

    // Menggambar titik sudut dan kotak ROI
    for(size_t i = 0; i < titik_sudut.size(); i++){
        cv::circle(segment, titik_sudut[i], 5, cv::Scalar(0, 0, 255), cv::FILLED);
    }
    cv::rectangle(segment, ROI_area, cv::Scalar(255, 0, 0), 2);

    // Mendefinisikan titik sumber dan tujuan untuk BEV
    cv::Point2f src[4] = {
        cv::Point2f(image.cols * 0.4f, image.rows * 0.6f),
        cv::Point2f(image.cols * 0.6f, image.rows * 0.6f),
        cv::Point2f(image.cols * 0.1f, image.rows * 1.0f),
        cv::Point2f(image.cols * 0.9f, image.rows * 1.0f)
    };

    cv::Point2f dst[4] = {
        cv::Point2f(image.cols * 0.2f, 0),
        cv::Point2f(image.cols * 0.8f, 0),
        cv::Point2f(image.cols * 0.2f, image.rows),
        cv::Point2f(image.cols * 0.8f, image.rows)
    };
    
    // Transformasi perspektif
    cv::Mat BEV_plot;
    cv::Mat BEV_view;
    BEV_plot = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(image, BEV_view, BEV_plot, image.size());

    // Simpan Hasil
    cv::imwrite("../assets/results/4-bev/BEV_view.jpg", BEV_view);
    cv::imwrite("../assets/results/4-bev/Segmented_Image.jpg", segment);

    // Menampilkan hasil
    cv::imshow("Image", image);
    cv::imshow("Image Thresholded", image_thresholded);
    cv::imshow("Segmented Image", segment);
    cv::imshow("Bird Eye View", BEV_view);

    cv::waitKey(0);

    return 0;
}