# Modul 4 - Bird's Eye View & Kalibrasi Kamera dengan OpenCV

Modul ini menjelaskan teknik transformasi perspektif Bird's Eye View (BEV) dan
kalibrasi kamera menggunakan library OpenCV di C++. Terdapat dua program yang
mencakup transformasi BEV dari gambar jalan dan kalibrasi kamera dengan chessboard pattern.

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
├── 1_bev.cpp      # Program transformasi Bird's Eye View
├── 2_calib.cpp     # Program kalibrasi kamera dengan chessboard
├── yaml/
│   └── kalibrasi.yaml   # Output hasil kalibrasi
└── README.md       # Dokumentasi modul
```

Asset yang digunakan oleh kedua program terdapat di folder `assets/`:

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
        └── Segmented_Image.jpg
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

## 2. 2_calib.cpp - Kalibrasi Kamera (Camera Calibration)

Program ini melakukan kalibrasi kamera menggunakan gambar chessboard pattern dari
dataset OpenCV. Proses kalibrasi menghasilkan camera matrix dan distortion
coefficients yang disimpan ke file YAML.

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

#### Langkah 3 - Kalibrasi Kamera

```cpp
cv::calibrateCamera(objectPoints, imagePoints, imageSize,
    cameraMatrix, distCoeffs, rvecs, tvecs);
```

Menghitung parameter intrinsik kamera (camera matrix dan distortion coefficients)
dari korespondensi titik 3D-2D.

#### Langkah 4 - Simpan Hasil

```cpp
cv::FileStorage fs(outputPath, cv::FileStorage::WRITE);
fs << "camera_matrix" << cameraMatrix;
fs << "distortion_coefficients" << distCoeffs;
```

Menyimpan hasil kalibrasi ke file `yaml/kalibrasi.yaml`.

### Output File (kalibrasi.yaml)

File YAML berisi:
- `camera_matrix` - Matriks intrinsik kamera (3x3)
- `distortion_coefficients` - Koefisien distorsi (1x5)
- `image_width`, `image_height` - Dimensi gambar
- `reprojection_error` - Error reprojeksi (semakin kecil semakin baik, < 1.0 px)

---

## Compile Semua Program Sekaligus

```bash
g++ 1_bev.cpp -o 1_bev $(pkg-config --cflags --libs opencv4) && \
g++ 2_calib.cpp -o 2_calib $(pkg-config --cflags --libs opencv4)
```

## Membersihkan File Binary

```bash
rm -f 1_bev 2_calib
```

## Catatan

- Pastikan file `road3.jpeg` berada di folder `assets/photos/` yang berada satu
  tingkat di atas folder `4-bev/`.
- Pastikan gambar kalibrasi (`left01.jpg` - `left14.jpg`) berada di folder
  `assets/calib/`.
- Path file dalam kode menggunakan `../assets/` karena binary program dijalankan
  dari dalam folder `4-bev/`.
- Hasil kalibrasi disimpan ke `4-bev/yaml/kalibrasi.yaml`.
- Hasil BEV disimpan ke folder `assets/result/4-bev/`.
- Dataset kalibrasi berasal dari
  [OpenCV Samples](https://github.com/opencv/opencv/tree/4.x/samples/data).
- Tekan tombol apapun pada jendela untuk menutup program.
