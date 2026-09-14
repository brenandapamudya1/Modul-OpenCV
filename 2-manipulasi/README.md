# Modul 2 - Manipulasi Citra dengan OpenCV

**Author**: Brenanda Caesa Pamudya  
**Email**: brenandapamudya178@gmail.com  

---

Modul ini menjelaskan manipulasi dasar citra menggunakan library OpenCV di C++.
Terdapat tiga program contoh yang mencakup flip (pencerminan), resize (perubahan ukuran),
dan ROI (Region of Interest).

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
2-manipulasi/
├── 1_flip.cpp    # Program pencerminan gambar
├── 2_resize.cpp  # Program perubahan ukuran gambar
├── 3_roi.cpp     # Program pemotongan region of interest
└── README.md     # Dokumentasi modul
```

Asset yang digunakan oleh ketiga program terdapat di folder `assets/`:

```
assets/
├── sample.jpg
├── sample2.jpg
└── sample.mp4
```

---

## 1. 1_flip.cpp - Pencerminan Gambar (Flip)

Program ini membaca file gambar dan melakukan pencerminan (flip) pada gambar.
Terdapat tiga mode flip yang tersedia:

- `0` - Flip vertikal (sumbu X)
- `1` - Flip horizontal (sumbu Y)
- `-1` - Flip keduanya (vertikal dan horizontal)

### Cara Compile

```bash
g++ 1_flip.cpp -o 1_flip $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./1_flip
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 1_flip.cpp -o 1_flip -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./1_flip
```

### Penjelasan Kode

1. `cv::imread("../assets/sample.jpg")` - Membaca file gambar dari path yang
   ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::Mat flipped` - Mendeklarasikan objek `cv::Mat` untuk menyimpan hasil flip.
4. `cv::flip(image, flipped, 1)` - Melakukan flip pada gambar. Parameter ketiga
   menentukan arah flip: `0` (vertikal), `1` (horizontal), `-1` (keduanya).
5. `cv::imshow("Gambar", image)` - Menampilkan gambar asli.
6. `cv::imshow("Flipped", flipped)` - Menampilkan gambar hasil flip.
7. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
   menutup jendela.

---

## 2. 2_resize.cpp - Perubahan Ukuran Gambar (Resize)

Program ini membaca file gambar dan mengubah ukurannya menjadi 640x480 pixel.

### Cara Compile

```bash
g++ 2_resize.cpp -o 2_resize $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./2_resize
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 2_resize.cpp -o 2_resize -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./2_resize
```

### Penjelasan Kode

1. `cv::imread("../assets/sample.jpg")` - Membaca file gambar dari path yang
   ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::Mat resized` - Mendeklarasikan objek `cv::Mat` untuk menyimpan hasil resize.
4. `cv::resize(image, resized, cv::Size(640, 480))` - Mengubah ukuran gambar
   menjadi 640x480 pixel. Parameter `cv::Size(640, 480)` menentukan ukuran target
   (width, height).
5. `cv::imshow("Gambar", image)` - Menampilkan gambar asli.
6. `cv::imshow("Resized", resized)` - Menampilkan gambar hasil resize.
7. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
   menutup jendela.

---

## 3. 3_roi.cpp - Region of Interest (ROI)

Program ini membaca file gambar dan memotong region of interest (area tertentu)
dari gambar berdasarkan koordinat rect (x, y, width, height).

### Cara Compile

```bash
g++ 3_roi.cpp -o 3_roi $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./3_roi
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ 3_roi.cpp -o 3_roi -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./3_roi
```

### Penjelasan Kode

1. `cv::imread("../assets/sample.jpg")` - Membaca file gambar dari path yang
   ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::Rect ROI(300, 200, 640, 480)` - Mendefinisikan area ROI dengan parameter
   `(x, y, width, height)`:
   - `300` - Posisi x (jarak dari kiri)
   - `200` - Posisi y (jarak dari atas)
   - `640` - Lebar area
   - `480` - Tinggi area
4. `cv::Mat roi_image = image(ROI)` - Memotong gambar sesuai area ROI yang
   didefinisikan.
5. `cv::imshow("Gambar", image)` - Menampilkan gambar asli.
6. `cv::imshow("Region of Interest", roi_image)` - Menampilkan hasil potongan ROI.
7. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
   menutup jendela.

---

## Compile Semua Program Sekaligus

```bash
g++ 1_flip.cpp -o 1_flip $(pkg-config --cflags --libs opencv4) && \
g++ 2_resize.cpp -o 2_resize $(pkg-config --cflags --libs opencv4) && \
g++ 3_roi.cpp -o 3_roi $(pkg-config --cflags --libs opencv4)
```

## Membersihkan File Binary

```bash
rm -f 1_flip 2_resize 3_roi
```

## Catatan

- Pastikan file `sample.jpg` berada di folder `assets/` yang berada satu tingkat
  di atas folder `2-manipulasi/`.
- Path file dalam kode menggunakan `../assets/` karena binary program dijalankan
  dari dalam folder `2-manipulasi/`.
- Untuk 1_flip.cpp, ubah parameter ketiga `cv::flip()` untuk mengubah arah flip.
- Untuk 2_resize.cpp, ubah `cv::Size(640, 480)` untuk mengubah ukuran target.
- Untuk 3_roi.cpp, ubah nilai `cv::Rect(x, y, width, height)` untuk memilih area
  yang berbeda dari gambar.
- Tekan tombol apapun pada jendela untuk menutup program.

---

If you have any questions or feedback, feel free to reach out via email!


