# Modul 4 - Bird's Eye View & Kalibrasi Kamera dengan OpenCV

**Author**: Brenanda Caesa Pamudya  
**Email**: brenandapamudya178@gmail.com  
> *If you have any questions or feedback, feel free to reach out via email!*

---

Modul ini menjelaskan teknik transformasi perspektif Bird's Eye View (BEV),
kalibrasi kamera, dan estimasi depth menggunakan library OpenCV di C++.
Terdapat tiga program yang mencakup transformasi BEV dari gambar jalan,
kalibrasi stereo kamera dengan chessboard pattern, dan stereo depth estimation.

## Prasyarat

- Compiler C++ (g++)
- Library OpenCV 4
- pkg-config (untuk mempermudah proses compile)

### Instalasi OpenCV (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install libopencv-dev
```

## Struktur Folder

```
4-bev/
├── 1_bev.cpp       # Program transformasi Bird's Eye View
├── 2_calib.cpp     # Program stereo kalibrasi kamera dengan chessboard
├── 3_depth.cpp     # Program stereo depth estimation
├── yaml/
│   ├── kalibrasi.yaml   # Output hasil kalibrasi
│   └── depth.yaml       # Output data depth
└── README.md       # Dokumentasi modul
```

Asset yang digunakan oleh ketiga program terdapat di folder `assets/`:

```
assets/
├── photos/
│   └── road3.jpeg
├── calib/
│   ├── left01.jpg - left14.jpg    # Gambar kalibrasi (kiri)
│   └── right01.jpg - right14.jpg  # Gambar kalibrasi (kanan)
└── result/
    └── 4-bev/
        ├── BEV_view.jpg
        ├── Segmented_Image.jpg
        ├── disparity_gray.jpg
        └── disparity_color.jpg
```

---

## 1. 1_bev.cpp - Transformasi Bird's Eye View

Program ini membaca gambar jalan, mendeteksi garis kuning dan putih menggunakan
color thresholding (HLS), kemudian melakukan transformasi perspektif untuk
menghasilkan tampilan Bird's Eye View.

### Cara Compile

```bash
g++ 1_bev.cpp -o 1_bev $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./1_bev
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 1_bev.cpp -o 1_bev -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_calib3d
./1_bev
```

### Penjelasan Kode

1. `cv::imread("../assets/photos/road3.jpeg")` - Membaca gambar jalan dari path
   yang ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `cv::resize(image, image, cv::Size(...))` - Mengubah ukuran gambar menjadi
   setengah dari ukuran asli.
3. `cv::Rect ROI_area(...)` - Mendefinisikan Region of Interest untuk area jalan
   bagian bawah gambar.
4. `cv::cvtColor(image(ROI_area), yellow_thresholded, cv::COLOR_BGR2HLS)` -
   Mengkonversi ROI ke color space HLS untuk deteksi garis kuning.
5. `cv::inRange(...)` - Melakukan thresholding untuk memisahkan garis kuning
   (`Scalar(15, 30, 80)` - `Scalar(35, 200, 255)`) dan garis putih
   (`Scalar(0, 180, 0)` - `Scalar(255, 255, 60)`).
6. `cv::bitwise_or(yellow_thresholded, white_thresholded, combined)` - Menggabungkan
   hasil deteksi garis kuning dan putih.
7. `cv::erode()` dan `cv::dilate()` - Operasi morfologi untuk membersihkan noise.
8. `cv::findContours(...)` - Mendeteksi kontur dari hasil thresholding.
9. `cv::approxPolyDP(...)` - Mengambil titik sudut dari kontur terbesar.
10. `cv::getPerspectiveTransform(src, dst)` - Menghitung matriks transformasi
    perspektif dari 4 titik sumber ke 4 titik tujuan.
11. `cv::warpPerspective(...)` - Menerapkan transformasi perspektif untuk
    menghasilkan Bird's Eye View.
12. `cv::imwrite(...)` - Menyimpan hasil BEV dan segmentasi ke folder result.

---

## 2. 2_calib.cpp - Stereo Kalibrasi Kamera (Stereo Camera Calibration)

Program ini melakukan stereo kalibrasi kamera menggunakan gambar chessboard pattern
dari dataset OpenCV. Proses kalibrasi menghasilkan camera matrix dan distortion
coefficients untuk kedua kamera (left & right), parameter stereo (R, T, E, F),
dan rectification parameters (R1, R2, P1, P2, Q) yang disimpan ke file YAML.

### Cara Compile

```bash
g++ 2_calib.cpp -o 2_calib $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./2_calib
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 2_calib.cpp -o 2_calib -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_calib3d
./2_calib
```

### Penjelasan Kode

Program ini terdiri dari 4 langkah utama:

#### Langkah 1 - Konfigurasi

```cpp
cv::Size boardSize(9, 6);
float squareSize = 25.0f;
```

Mendefinisikan ukuran chessboard pattern (9x6 inner corners) dan ukuran kotak
dalam milimeter.

#### Langkah 2 - Deteksi Chessboard Corners

```cpp
cv::findChessboardCorners(gray, boardSize, corners, ...);
cv::cornerSubPix(gray, corners, cv::Size(11, 11), ...);
```

Mendeteksi sudut-sudut chessboard pada setiap gambar, kemudian melakukan
sub-pixel refinement untuk akurasi yang lebih tinggi.

#### Langkah 3 - Kalibrasi Single Camera

```cpp
cv::calibrateCamera(objectPoints, imagePointsL, imageSize,
    cameraMatrixL, distCoeffsL, rvecsL, tvecsL);
cv::calibrateCamera(objectPoints, imagePointsR, imageSize,
    cameraMatrixR, distCoeffsR, rvecsR, tvecsR);
```

Menghitung parameter intrinsik masing-masing kamera (left & right).

#### Langkah 4 - Stereo Calibration

```cpp
cv::stereoCalibrate(objectPoints, imagePointsL, imagePointsR,
    cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR,
    imageSize, R, T, E, F, cv::CALIB_FIX_INTRINSIC, ...);
```

Menghitung relasi spasial antar kamera (rotation R, translation T, essential
matrix E, fundamental matrix F).

#### Langkah 5 - Stereo Rectification

```cpp
cv::stereoRectify(cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR,
    imageSize, R, T, R1, R2, P1, P2, Q, ...);
```

Menghitung rectification transforms (R1, R2), projection matrices (P1, P2),
dan disparity-to-depth matrix (Q) yang digunakan oleh `3_depth.cpp`.

#### Langkah 6 - Simpan Hasil

```cpp
cv::FileStorage fs(outputPath, cv::FileStorage::WRITE);
fs << "camera_matrix_left" << cameraMatrixL;
fs << "Q" << Q;
```

Menyimpan semua parameter ke file `yaml/kalibrasi.yaml`.

### Output File (kalibrasi.yaml)

File YAML berisi:
- `camera_matrix_left`, `camera_matrix_right` - Matriks intrinsik kamera (3x3)
- `distortion_coefficients_left`, `distortion_coefficients_right` - Koefisien distorsi (1x5)
- `R`, `T`, `E`, `F` - Parameter stereo
- `R1`, `R2`, `P1`, `P2`, `Q` - Rectification & projection matrices
- `image_width`, `image_height` - Dimensi gambar
- `reprojection_error_left`, `reprojection_error_right`, `reprojection_error_stereo`

---

## 3. 3_depth.cpp - Stereo Depth Estimation

Program ini menghitung depth (kedalaman) dari sepasang gambar stereo menggunakan
algorithm Semi-Global Block Matching (StereoSGBM). Program membaca parameter
kalibrasi dari `yaml/kalibrasi.yaml`, melakukan rectification, menghitung
disparity map, dan mengkonversinya ke depth 3D.

### Cara Compile

```bash
g++ 3_depth.cpp -o 3_depth $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./3_depth
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 3_depth.cpp -o 3_depth -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_calib3d
./3_depth
```

### Penjelasan Kode

Program ini terdiri dari 5 langkah utama:

#### Langkah 1 - Load Parameter Kalibrasi

```cpp
cv::FileStorage fs(calibPath, cv::FileStorage::READ);
fs["camera_matrix_left"] >> cameraMatrixL;
fs["Q"] >> Q;
```

Membaca parameter kalibrasi stereo dari `yaml/kalibrasi.yaml`, termasuk camera
matrices, distortion coefficients, dan rectification parameters (R1, R2, P1, P2, Q).

#### Langkah 2 - Rectify Gambar

```cpp
cv::initUndistortRectifyMap(cameraMatrixL, distCoeffsL, R1, P1, ...);
cv::remap(imgL, rectL, mapL1, mapL2, cv::INTER_LINEAR);
```

Melakukan undistortion dan rectification pada kedua gambar sehingga epipolar
lines menjadi horizontal dan sejajar.

#### Langkah 3 - Compute Disparity

```cpp
auto stereo = cv::StereoSGBM::create(0, numDisparities, blockSize, P1, P2, ...);
stereo->compute(grayL, grayR, disparity);
```

Menghitung disparity map menggunakan StereoSGBM. Pixel yang lebih terang pada
disparity map menandakan objek yang lebih dekat ke kamera.

#### Langkah 4 - Konversi ke Depth 3D

```cpp
cv::reprojectImageTo3D(dispFloat, depth3D, Q, true);
```

Mengkonversi disparity map ke koordinat 3D (X, Y, Z dalam milimeter)
menggunakan matrix Q dari stereo rectification.

#### Langkah 5 - Output Hasil

Program menghasilkan 4 output:
- `assets/result/4-bev/disparity_gray.jpg` - Disparity map grayscale
- `assets/result/4-bev/disparity_color.jpg` - Disparity map dengan colormap JET
- Terminal output - Depth values di 5 titik sample (center + 4 kuadran)
- `yaml/depth.yaml` - Data depth 3D lengkap

---

## Compile Semua Program Sekaligus

```bash
g++ 1_bev.cpp -o 1_bev $(pkg-config --cflags --libs opencv4) && \
g++ 2_calib.cpp -o 2_calib $(pkg-config --cflags --libs opencv4) && \
g++ 3_depth.cpp -o 3_depth $(pkg-config --cflags --libs opencv4)
```

## Membersihkan File Binary

```bash
rm -f 1_bev 2_calib 3_depth
```

## Catatan

- Pastikan file `road3.jpeg` berada di folder `assets/photos/` yang berada satu
  tingkat di atas folder `4-bev/`.
- Pastikan gambar kalibrasi (`left01.jpg` - `left14.jpg` dan `right01.jpg` -
  `right14.jpg`) berada di folder `assets/calib/`.
- Path file dalam kode menggunakan `../assets/` karena binary program dijalankan
  dari dalam folder `4-bev/`.
- Jalankan `2_calib` terlebih dahulu sebelum `3_depth`, karena `3_depth`
  membutuhkan file `yaml/kalibrasi.yaml` sebagai input.
- Hasil kalibrasi dan depth disimpan ke `4-bev/yaml/`.
- Hasil BEV dan disparity disimpan ke folder `assets/result/4-bev/`.
- Dataset kalibrasi berasal dari
  [OpenCV Samples](https://github.com/opencv/opencv/tree/4.x/samples/data).
- Tekan tombol apapun pada jendela untuk menutup program.

