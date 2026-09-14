# Modul 4 - Bird's Eye View & Kalibrasi Kamera dengan OpenCV

**Author**: Brenanda Caesa Pamudya  
**Email**: brenandapamudya178@gmail.com  

---

Modul ini menjelaskan teknik transformasi perspektif Bird's Eye View (BEV),
kalibrasi single camera (pinhole model), dan monocular depth estimation menggunakan library OpenCV di C++.
Terdapat tiga program yang mencakup transformasi BEV dari gambar jalan,
kalibrasi single camera dengan chessboard pattern, dan monocular depth estimation berbasis Pinhole Camera Model.

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
├── 2_calib.cpp     # Program kalibrasi single camera (pinhole model)
├── 3_depth.cpp     # Program monocular depth estimation (pinhole model)
├── yaml/
│   ├── kalibrasi.yaml   # Output hasil kalibrasi intrinsik kamera
│   └── depth.yaml       # Output data depth 3D terhitung
└── README.md       # Dokumentasi modul
```

Asset yang digunakan oleh ketiga program terdapat di folder `assets/`:

```
assets/
├── photos/
│   └── road3.jpeg
├── calib/
│   └── left01.jpg - left14.jpg    # Gambar kalibrasi single camera
└── result/
    └── 4-bev/
        ├── BEV_view.jpg
        ├── Segmented_Image.jpg
        └── depth_pinhole.jpg
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

## 2. 2_calib.cpp - Single Camera Calibration (Pinhole Camera Model)

Program ini melakukan kalibrasi kamera tunggal (single camera / monocular) menggunakan
sekelompok gambar chessboard pattern. Kalibrasi menghasilkan matriks intrinsik kamera ($K$),
focal length ($f_x, f_y$), titik utama ($c_x, c_y$), dan koefisien distorsi yang disimpan ke
file `yaml/kalibrasi.yaml`.

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

#### Langkah 1 - Konfigurasi Chessboard

```cpp
cv::Size boardSize(9, 6);
float squareSize = 25.0f; // mm
```

Mendefinisikan jumlah sudut dalam chessboard (9x6 inner corners) dan ukuran fisik kotak dalam mm.

#### Langkah 2 - Deteksi Sudut Chessboard

```cpp
cv::findChessboardCorners(gray, boardSize, corners, flags);
cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);
```

Mendeteksi posisi 2D sudut-sudut chessboard pada setiap gambar dan melakukan penyempurnaan sub-piksel.

#### Langkah 3 - Kalibrasi Kamera Single

```cpp
cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
```

Menghitung matriks intrinsik kamera ($K$) dan koefisien distorsi berdasarkan pemetaan koordinat 3D objek ke 2D gambar.

#### Langkah 4 - Simpan Parameter ke YAML

```cpp
cv::FileStorage fs(outputPath, cv::FileStorage::WRITE);
fs << "camera_matrix" << cameraMatrix;
fs << "focal_length_x" << cameraMatrix.at<double>(0, 0);
fs << "focal_length_y" << cameraMatrix.at<double>(1, 1);
```

Menyimpan semua parameter intrinsik ke file `yaml/kalibrasi.yaml`.

---

## 3. 3_depth.cpp - Monocular Depth Estimation (Pinhole Camera Model)

Program ini menghitung estimasi jarak/kedalaman ($Z$) dari kamera tunggal (misal webcam atau HP)
menggunakan **Pinhole Camera Model** berbasis perbandingan segitiga sebanding (*triangle similarity*):

$$Z = \frac{f_x \cdot W_{\text{real}}}{w_{\text{pixel}}}$$

di mana:
- $f_x$: Focal length kamera dalam piksel (dari `yaml/kalibrasi.yaml`)
- $W_{\text{real}}$: Lebar fisik asli objek referensi (mm)
- $w_{\text{pixel}}$: Lebar objek terukur pada gambar piksel
- $Z$: Jarak terhitung dari kamera ke objek (mm atau meter)

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

1. **Load Parameter Kalibrasi**: Membaca $f_x, f_y, c_x, c_y$ dari `yaml/kalibrasi.yaml`.
2. **Deteksi Objek Referensi**: Mendeteksi sudut chessboard dan mengukur lebar piksel ($w_{\text{pixel}}$).
3. **Hitung Kedalaman Pinhole**: Menghitung $Z = \frac{f_x \cdot W_{\text{real}}}{w_{\text{pixel}}}$.
4. **Hitung Posisi 3D**:
   $$X = \frac{(x_{\text{pixel}} - c_x) \cdot Z}{f_x}, \quad Y = \frac{(y_{\text{pixel}} - c_y) \cdot Z}{f_y}$$
5. **Visualisasi & Simpan Result**: Menggambar bounding box, titik pusat, label jarak ($Z$), dan menyimpan ke `assets/result/4-bev/depth_pinhole.jpg` serta `yaml/depth.yaml`.

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

- Pastikan file `road3.jpeg` berada di folder `assets/photos/`.
- Gambar kalibrasi (`left01.jpg` - `left14.jpg`) berada di folder `assets/calib/`.
- Jalankan `2_calib` terlebih dahulu sebelum `3_depth`, karena `3_depth` membutuhkan file `yaml/kalibrasi.yaml` sebagai input parameter intrinsik.
- Hasil kalibrasi dan depth tersimpan di `4-bev/yaml/`.
- Hasil visualisasi tersimpan di folder `assets/result/4-bev/`.

---

If you have any questions or feedback, feel free to reach out via email!
