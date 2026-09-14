# Modul Pembelajaran OpenCV (C++)

**Author**: Brenanda Caesa Pamudya  
**Email**: brenandapamudya178@gmail.com  

---

Repositori ini berisi rangkaian modul praktikum dan eksperimen pemrosesan citra digital (*digital image processing*) serta komputer visi (*computer vision*) menggunakan library **OpenCV 4** berbasis **C++**.

## Daftar Modul

| Modul | Nama Modul | Deskripsi Topik |
|---|---|---|
| [`1-akses/`](1-akses/README.md) | **Akses Citra** | Dasar pembacaan gambar (`1_gambar.cpp`), pemutaran file video (`2_video.cpp`), dan pengaksesan feed kamera/webcam real-time (`3_kamera.cpp`). |
| [`2-manipulasi/`](2-manipulasi/README.md) | **Manipulasi Citra** | Teknik manipulasi piksel dasar: pencerminan (`1_flip.cpp`), perubahan skala (`2_resize.cpp`), dan pemotongan area tertentu (`3_roi.cpp`). |
| [`3-segmentation/`](3-segmentation/README.md) | **Segmentasi Citra** | Konversi ruang warna HLS (`1_convert.cpp`), pemisahan warna dengan thresholding (`2_thresholding.cpp`), perbaikan noise dengan morfologi (`3_morphology.cpp`), dan pipeline lengkap deteksi objek dengan kontur & bounding box (`main.cpp`). |
| [`4-bev/`](4-bev/README.md) | **BEV, Kalibrasi & Depth** | Transformasi perspektif Bird's Eye View (`1_bev.cpp`), kalibrasi stereo kamera pasang gambar left/right (`2_calib.cpp`), dan estimasi kedalaman 3D menggunakan StereoSGBM (`3_depth.cpp`). |

---

## Struktur Repositori

```text
OpenCV-Intern/
├── 1-akses/            # Modul 1: Akses citra, video, webcam
├── 2-manipulasi/       # Modul 2: Flip, resize, ROI
├── 3-segmentation/     # Modul 3: Konversi warna, thresholding, morfologi, kontur
├── 4-bev/              # Modul 4: BEV, Stereo Calibration, Stereo Depth Estimation
├── assets/             # Gambar & sampel data (photos, calib, video)
└── README.md           # Dokumentasi utama repositori
```

---

## Prasyarat & Instalasi

### 1. Sistem Operasi & Tools
- OS: Linux (Ubuntu/Debian) atau WSL
- Compiler: `g++` (mendukung C++17)
- Build helper: `pkg-config`

### 2. Instalasi OpenCV 4 di Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential libopencv-dev pkg-config
```

### 3. Verifikasi Instalasi OpenCV

```bash
pkg-config --modversion opencv4
```

---

## Perintah Kompilasi Umum

Gunakan perintah `g++` dengan flag `pkg-config` untuk melakukan kompilasi file C++ OpenCV:

```bash
g++ nama_program.cpp -o nama_binary $(pkg-config --cflags --libs opencv4)
```

Untuk petunjuk kompilasi dan penjelasan detail tiap program, silakan buka `README.md` pada masing-masing folder modul.

---

If you have any questions or feedback, feel free to reach out via email!
