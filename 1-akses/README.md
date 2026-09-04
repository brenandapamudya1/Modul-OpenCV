# Modul 1 - Akses Citra dengan OpenCV

Modul ini menjelaskan dasar akses citra menggunakan library OpenCV di C++.
Terdapat tiga program contoh yang mencakup pembacaan gambar, pemutaran video,
dan akses kamera secara real-time.

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
1-akses/
├── gambar.cpp    # Program pembacaan dan tampilan gambar
├── video.cpp     # Program pemutaran file video
├── kamera.cpp    # Program akses kamera real-time
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

## 1. gambar.cpp - Membaca dan Menampilkan Gambar

Program ini membaca file gambar dari disk dan menampilkannya di jendela baru.

### Cara Compile

```bash
g++ gambar.cpp -o gambar $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./gambar
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ gambar.cpp -o gambar -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
./gambar
```

### Penjelasan Kode

1. `cv::imread("../assets/sample2.jpg")` - Membaca file gambar dari path yang
   ditentukan dan menyimpannya ke dalam objek `cv::Mat`.
2. `image.empty()` - Mengecek apakah gambar berhasil dimuat. Jika gagal, program
   akan mencetak pesan error ke `stderr`.
3. `cv::imshow("Gambar", image)` - Menampilkan gambar pada jendela bernama "Gambar".
4. `cv::waitKey(0)` - Menunggu hingga pengguna menekan sembarang tombol untuk
   menutup jendela.

---

## 2. video.cpp - Memutar File Video

Program ini membaca file video frame demi frame dan menampilkannya secara berurutan
hingga video selesai atau pengguna menekan tombol `q`.

### Cara Compile

```bash
g++ video.cpp -o video $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./video
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ video.cpp -o video -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio
./video
```

### Penjelasan Kode

1. `cv::VideoCapture cap("../assets/sample.mp4")` - Membuka file video untuk dibaca.
2. `cap >> frame` - Membaca satu frame dari video ke dalam objek `cv::Mat`.
3. `frame.empty()` - Mengecek apakah frame masih tersedia. Jika kosong, loop berhenti
   (video selesai).
4. `cv::imshow("Video", frame)` - Menampilkan frame saat ini.
5. `cv::waitKey(25)` - Menunggu 25 milidetik sebelum frame berikutnya. Jika tombol
   `q` ditekan, loop berhenti.
6. `cap.release()` - Melepaskan resource video.
7. `cv::destroyAllWindows()` - Menutup semua jendela yang terbuka.

---

## 3. kamera.cpp - Akses Kamera Real-Time

Program ini mengakses kamera perangkat (webcam) dan menampilkan feed secara
real-time hingga pengguna menekan tombol `q`.

### Cara Compile

```bash
g++ kamera.cpp -o kamera $(pkg-config --cflags --libs opencv4)
```

### Cara Jalankan

```bash
./kamera
```

### Cara Jalankan (tanpa pkg-config)

```bash
g++ kamera.cpp -o kamera -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio
./kamera
```

### Penjelasan Kode

1. `cv::VideoCapture cap(0)` - Membuka kamera dengan indeks 0 (kamera default).
   Jika menggunakan kamera eksternal, ganti indeks ke 1 atau seterusnya.
2. `cap >> frame` - Membaca satu frame dari kamera ke dalam objek `cv::Mat`.
3. `frame.empty()` - Mengecek apakah frame berhasil ditangkap. Jika kosong, loop
   berhenti.
4. `cv::imshow("Kamera", frame)` - Menampilkan frame dari kamera.
5. `cv::waitKey(25)` - Menunggu 25 milidetik sebelum frame berikutnya. Jika tombol
   `q` ditekan, loop berhenti.
6. `cap.release()` - Melepaskan resource kamera.
7. `cv::destroyAllWindows()` - Menutup semua jendela yang terbuka.

---

## Compile Semua Program Sekaligus

```bash
g++ gambar.cpp -o gambar $(pkg-config --cflags --libs opencv4) && \
g++ video.cpp -o video $(pkg-config --cflags --libs opencv4) && \
g++ kamera.cpp -o kamera $(pkg-config --cflags --libs opencv4)
```

## Membersihkan File Binary

```bash
rm -f gambar video kamera
```

## Catatan

- Pastikan file `sample2.jpg` dan `sample.mp4` berada di folder `assets/` yang
  berada satu tingkat di atas folder `1-akses/`.
- Path file dalam kode menggunakan `../assets/` karena binary program dijalankan
  dari dalam folder `1-akses/`.
- Untuk program kamera, pastikan perangkat memiliki webcam yang terdeteksi oleh
  sistem operasi.
- Tekan tombol apapun pada program gambar, atau tombol `q` pada program video dan
  kamera untuk menutup jendela.
