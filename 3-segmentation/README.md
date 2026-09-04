# Modul 3 - Segmentasi Citra dengan OpenCV

Modul ini menjelaskan teknik segmentasi citra menggunakan library OpenCV di C++.
Terdapat empat program contoh yang mencakup konversi warna, thresholding,
operasi morfologi, dan deteksi kontur dengan bounding box.

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
3-segmentation/
├── 1_convert.cpp        # Program konversi warna BGR ke HLS
├── 2_thresholding.cpp   # Program thresholding dengan inRange
├── 3_morphology.cpp     # Program operasi morfologi erode dan dilate
├── main.cpp             # Program gabungan: convert + thresholding + morphology + contour
└── README.md            # Dokumentasi modul
```

Asset yang digunakan oleh ketiga program terdapat di folder `assets/`:

```
assets/
├── photos/
│   ├── sample.jpg
│   └── sample2.jpg
├── videos/
│   └── sample.mp4
└── result/
    └── 3-segmentation/
        ├── convert_result.jpg
        ├── thresholding_result.jpg
        ├── morphology_result.jpg
        └── final_result.jpg
```

---

## 1. 1_convert.cpp - Konversi Warna (Color Conversion)

Program ini membaca file gambar dan mengubah color space dari BGR (Blue-Green-Red)
menjadi HLS (Hue-Lightness-Saturation).

### Cara Compile

```bash
g++ 1_convert.cpp -o 1_convert $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./1_convert
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 1_convert.cpp -o 1_convert -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./1_convert
```

### Penjelasan Kode

1. `cv::imread("../assets/photos/sample2.jpg")` - Membaca file gambar dari path
   yang ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::Mat hlsImage` - Mendeklarasikan objek `cv::Mat` untuk menyimpan hasil
   konversi warna.
4. `cv::cvtColor(image, hlsImage, cv::COLOR_BGR2HLS)` - Mengkonversi gambar dari
   color space BGR ke HLS.
5. `cv::imwrite("../assets/result/3-segmentation/convert_result.jpg", hlsImage)` -
   Menyimpan hasil konversi ke folder result.
6. `cv::imshow("Real Image", image)` - Menampilkan gambar asli.
7. `cv::imshow("HLS Image", hlsImage)` - Menampilkan gambar hasil konversi HLS.
8. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
   menutup jendela.

---

## 2. 2_thresholding.cpp - Thresholding

Program ini membaca file gambar dan melakukan thresholding menggunakan fungsi
`cv::inRange()` untuk memisahkan objek berdasarkan rentang nilai warna tertentu.

### Cara Compile

```bash
g++ 2_thresholding.cpp -o 2_thresholding $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./2_thresholding
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 2_thresholding.cpp -o 2_thresholding -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./2_thresholding
```

### Penjelasan Kode

1. `cv::imread("../assets/photos/sample2.jpg")` - Membaca file gambar dari path
   yang ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::Mat thresholdedImage` - Mendeklarasikan objek `cv::Mat` untuk menyimpan
   hasil thresholding.
4. `cv::inRange(image, cv::Scalar(11, 42, 61), cv::Scalar(255, 211, 255), thresholdedImage)` -
   Melakukan thresholding dengan rentang warna:
   - `cv::Scalar(11, 42, 61)` - Batas bawah (lower bound)
   - `cv::Scalar(255, 211, 255)` - Batas atas (upper bound)
   - Pixel yang berada dalam rentang akan berwarna putih, di luar rentang akan
     berwarna hitam.
5. `cv::imwrite("../assets/result/3-segmentation/thresholding_result.jpg", thresholdedImage)` -
   Menyimpan hasil thresholding ke folder result.
6. `cv::imshow("Real Image", image)` - Menampilkan gambar asli.
7. `cv::imshow("Thresholded Image", thresholdedImage)` - Menampilkan gambar hasil
   thresholding.
8. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
   menutup jendela.

---

## 3. 3_morphology.cpp - Operasi Morfologi (Erode & Dilate)

Program ini membaca file gambar dan melakukan operasi morfologi erode (penipisan)
dan dilate (pembesaran) untuk membersihkan noise pada gambar.

### Cara Compile

```bash
g++ 3_morphology.cpp -o 3_morphology $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./3_morphology
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 3_morphology.cpp -o 3_morphology -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./3_morphology
```

### Penjelasan Kode

1. `cv::imread("../assets/photos/sample2.jpg")` - Membaca file gambar dari path
   yang ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::Mat morphImage = image.clone()` - Mendeklarasikan objek `cv::Mat` sebagai
   salinan dari gambar asli.
4. `cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 2)` - Melakukan
   erode (penipisan) dengan iterasi sebanyak 2 kali. Erode menghilangkan pixel
   tepi objek, sehingga objek menjadi lebih kecil.
5. `cv::dilate(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 8)` - Melakukan
   dilate (pembesaran) dengan iterasi sebanyak 8 kali. Dilate menambah pixel tepi
   objek, sehingga objek menjadi lebih besar.
6. `cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 5)` - Melakukan
   erode lagi dengan iterasi sebanyak 5 kali untuk menormalkan ukuran objek.
7. `cv::imwrite("../assets/result/3-segmentation/morphology_result.jpg", morphImage)` -
   Menyimpan hasil morfologi ke folder result.
8. `cv::imshow("Real Image", image)` - Menampilkan gambar asli.
9. `cv::imshow("Morphology Result", morphImage)` - Menampilkan gambar hasil
   operasi morfologi.
10. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
    menutup jendela.

---

## 4. main.cpp - Program Gabungan (Pipeline Lengkap)

Program ini merupakan gabungan dari ketiga program sebelumnya (convert, thresholding,
morphology) serta penambahan fungsi untuk mendeteksi kontur dan membuat bounding box
pada objek yang terdeteksi.

### Cara Compile

```bash
g++ main.cpp -o main $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./main
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ main.cpp -o main -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./main
```

### Penjelasan Kode

Program ini terdiri dari 5 langkah utama:

#### Langkah 1 - Konversi Warna

```cpp
cv::cvtColor(image, hlsImage, cv::COLOR_BGR2HLS);
```

Mengkonversi gambar dari BGR ke HLS.

#### Langkah 2 - Thresholding

```cpp
cv::inRange(hlsImage, cv::Scalar(11, 42, 61), cv::Scalar(255, 211, 255), thresholdedImage);
```

Melakukan thresholding pada gambar HLS untuk memisahkan objek dari latar belakang.

#### Langkah 3 - Operasi Morfologi

```cpp
cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 2);
cv::dilate(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 8);
cv::erode(morphImage, morphImage, cv::Mat(), cv::Point(-1, -1), 5);
```

Membersihkan noise pada hasil thresholding menggunakan erode dan dilate.

#### Langkah 4 - Deteksi Kontur

```cpp
std::vector<std::vector<cv::Point>> contours;
cv::findContours(morphImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
```

Mendeteksi kontur (garis tepi) pada gambar biner. `cv::RETR_EXTERNAL` hanya
mengambil kontur terluar, dan `cv::CHAIN_APPROX_SIMPLE` menyimpan titik-titik
penting saja.

#### Langkah 5 - Bounding Box

```cpp
for (size_t i = 0; i < contours.size(); i++) {
    if (cv::contourArea(contours[i]) > 1000) {
        cv::Rect boundingBox = cv::boundingRect(contours[i]);
        cv::rectangle(result, boundingBox, cv::Scalar(0, 255, 0), 2);
    }
}
```

Membuat bounding kotak (rectangle) pada setiap kontur yang memiliki area lebih
dari 1000 pixel. Filter area digunakan untuk menghilangkan kontur kecil yang
bukan objek utama.

#### Simpan Hasil

```cpp
cv::imwrite("../assets/result/3-segmentation/final_result.jpg", result);
```

Menyimpan hasil akhir ke folder result.

---

## Compile Semua Program Sekaligus

```bash
g++ 1_convert.cpp -o 1_convert $(pkg-config --cflags --libs opencv4) && \
g++ 2_thresholding.cpp -o 2_thresholding $(pkg-config --cflags --libs opencv4) && \
g++ 3_morphology.cpp -o 3_morphology $(pkg-config --cflags --libs opencv4) && \
g++ main.cpp -o main $(pkg-config --cflags --libs opencv4)
```

## Membersihkan File Binary

```bash
rm -f 1_convert 2_thresholding 3_morphology main
```

## Catatan

- Pastikan file `sample2.jpg` berada di folder `assets/photos/` yang berada satu
  tingkat di atas folder `3-segmentation/`.
- Path file dalam kode menggunakan `../assets/photos/` karena binary program
  dijalankan dari dalam folder `3-segmentation/`.
- Hasil output disimpan ke folder `assets/result/3-segmentation/`.
- Untuk program thresholding, nilai `cv::Scalar()` dapat disesuaikan untuk
  mendeteksi warna yang berbeda.
- Untuk program morphology, jumlah iterasi erode dan dilate dapat disesuaikan
  sesuai kebutuhan.
- Tekan tombol apapun pada jendela untuk menutup program.
