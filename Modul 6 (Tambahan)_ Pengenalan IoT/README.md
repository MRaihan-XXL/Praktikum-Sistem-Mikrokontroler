# Pertemuan 6

## 6.5.4 Percobaan 6A: External Interrupt (Kendali LED dengan Tombol)

1. Jelaskan proses bagaimana tombol dapat mengubah kondisi LED menggunakan interrupt!

> - JTombol dihubungkan ke pin 2 dengan mode INPUT_PULLUP. Saat tombol tidak ditekan, pin 2 membaca HIGH (karena terhubung ke 5V via resistor internal). Saat tombol ditekan, pin 2 terhubung ke GND sehingga membaca LOW → terjadi transisi dari HIGH ke LOW (falling edge).
> - Interrupt dengan mode FALLING sudah didaftarkan menggunakan attachInterrupt(). Ketika transisi tersebut terdeteksi, mikrokontroler langsung menghentikan program utama (loop()) dan melompat ke fungsi ISR (tombolInterrupt()).
> - Di dalam ISR, variabel ledState dibalik nilainya (!ledState). Setelah ISR selesai, program utama dilanjutkan.
> - Di loop(), nilai ledState terus ditulis ke pin LED (13 dan 12). Karena ISR mengubah ledState setiap tombol ditekan, LED berubah nyala/mati secara instan.


2. Apa fungsi attachInterrupt() pada program tersebut?

> Fungsi attachInterrupt() digunakan untuk menghubungkan pin interrupt dengan fungsi ISR serta menentukan mode pemicu. Format: attachInterrupt(digitalPinToInterrupt(pin), ISR, mode). Pada program, digitalPinToInterrupt(2) mengonversi pin 2 menjadi nomor interrupt 0, tombolInterrupt adalah nama fungsi ISR, dan FALLING adalah mode pemicu (tekan tombol).


3. Mengapa pada ISR tidak disarankan menggunakan delay() dan Serial.print()?

> ISR harus dieksekusi sesingkat mungkin karena selama ISR berjalan, interrupt lain ditunda dan program utama tidak berjalan. delay() akan menghentikan ISR terlalu lama, menyebabkan sistem tidak responsif bahkan bisa melewatkan interrupt lain. Serial.print() juga lambat karena menunggu pengiriman data serial, dan jika di dalam ISR dapat menyebabkan konflik dengan komunikasi serial utama. Prinsip ISR: hanya untuk tugas yang sangat cepat, seperti mengubah variabel atau menandai flag.


4. Apa fungsi keyword volatile pada variabel ledState?

> Keyword volatile memberitahu compiler bahwa nilai variabel dapat berubah kapan saja di luar alur normal program, khususnya oleh ISR yang dijalankan secara asinkron. Tanpa volatile, compiler dapat mengoptimalkan kode dengan menganggap nilai variabel tidak pernah berubah di loop(), sehingga perubahan dari ISR tidak terlihat. Dengan volatile, compiler selalu membaca nilai variabel dari memori setiap kali digunakan.


5. Pada percobaan digunakan mode interrupt FALLING. Modifikasikan program menggunakan mode interrupt lain (RISING, CHANGE, atau LOW) kemudian:
> - Jelaskan perbedaan cara kerja masing-masing mode interrupt tersebut
> - Analisis perubahan perilaku LED yang terjadi pada setiap mode
> - Sertakan source code dan penjelasan program dalam bentuk README.md

A. Mode RISING (interrupt dipicu saat tombol dilepaskan)
```c++
// Percobaan 6A - Mode RISING: LED berubah saat tombol DILEPASKAN
#include <Arduino.h>

volatile bool ledState = false;  // status LED (volatile karena diubah di ISR)

// ISR: dipanggil saat terjadi transisi LOW → HIGH (tombol dilepaskan)
void tombolInterrupt() {
    ledState = !ledState;        // balik status LED
}

void setup() {
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(2, INPUT_PULLUP);    // pull-up internal, pin HIGH saat tidak ditekan
    
    // Pasang interrupt pada pin 2, mode RISING (LOW → HIGH)
    attachInterrupt(digitalPinToInterrupt(2), tombolInterrupt, RISING);
}

void loop() {
    digitalWrite(13, ledState);
    digitalWrite(12, ledState);
}
```
> - Mode RISING mendeteksi perubahan tegangan dari LOW (0V) ke HIGH (5V).
> - Pada konfigurasi INPUT_PULLUP, saat tombol ditekan pin menjadi LOW, saat dilepaskan kembali HIGH.
> - Jadi interrupt terjadi saat jari melepas tombol, bukan saat menekan dan LED akan berubah kondisi setelah tombol dilepaskan.

B. Mode CHANGE (interrupt dipicu setiap kali pin berubah, baik tekan maupun lepas)
```c++
// Percobaan 6A - Mode CHANGE: LED berubah SETIAP KALI pin berubah (tekan & lepas)
#include <Arduino.h>

volatile bool ledState = false;

void tombolInterrupt() {
    ledState = !ledState;        // balik status setiap kali ada perubahan
}

void setup() {
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(2, INPUT_PULLUP);
    
    // Pasang interrupt pada pin 2, mode CHANGE (setiap perubahan HIGH↔LOW)
    attachInterrupt(digitalPinToInterrupt(2), tombolInterrupt, CHANGE);
}

void loop() {
    digitalWrite(13, ledState);
    digitalWrite(12, ledState);
}
```
> - Mode CHANGE memicu interrupt pada setiap transisi, baik dari HIGH→LOW (tekan) maupun LOW→HIGH (lepas).
> - Dalam satu kali siklus tekan+lepas, interrupt terjadi dua kali, sehingga LED akan berubah nyala/mati dua kali (kembali ke kondisi awal).
> - Akibatnya, LED mungkin tampak tidak berubah setelah satu kali tekan dan lepas, atau berubah dengan cepat dua kali.

C. Mode LOW (interrupt dipicu terus menerus selama pin dalam keadaan LOW)
```c++
// Percobaan 6A - Mode LOW: interrupt dipicu TERUS-MENERUS selama tombol ditekan
#include <Arduino.h>

volatile bool ledState = false;

void tombolInterrupt() {
    ledState = !ledState;        // akan dipanggil berulang kali saat tombol ditekan
}

void setup() {
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(2, INPUT_PULLUP);
    
    // Pasang interrupt pada pin 2, mode LOW (selama pin LOW, ISR terus dipanggil)
    attachInterrupt(digitalPinToInterrupt(2), tombolInterrupt, LOW);
}

void loop() {
    digitalWrite(13, ledState);
    digitalWrite(12, ledState);
}
```
> - Mode LOW menyebabkan interrupt terjadi terus‑menerus selama pin dalam keadaan LOW (tombol ditekan).
> - ISR akan dipanggil berkali‑kali (bukan sekali), sehingga LED akan berubah kondisi sangat cepat (berkedip) selama tombol masih ditekan.
> - Tidak stabil untuk aplikasi saklar on/off biasa; lebih cocok untuk deteksi tombol tahan (hold).


## 6.6.4 Percobaan 6B: Timer Menggunakan millis() (LED Blinking Non‑Blocking)

1. Jelaskan bagaimana fungsi millis() bekerja pada program tersebut!

> millis() mengembalikan jumlah milidetik sejak Arduino dinyalakan (atau di‑reset). Nilai kembali adalah unsigned long (0 hingga ~50 hari). Program menyimpan waktu terakhir LED berubah di variabel previousMillis. Setiap loop(), currentMillis = millis(). Jika selisih currentMillis - previousMillis >= interval (1000 ms), maka sudah waktunya mengubah status LED. Ini adalah metode non‑blocking.


2. Apa perbedaan utama antara delay() dan millis()?

| delay()                    | millis()                    |
| -------------------------- | --------------------------- |
| Blocking: program berhenti total selama waktu yang ditentukan.        | Non‑blocking: program terus berjalan, hanya mengecek waktu.      |
| Mudah digunakan untuk satu tugas.| Cocok untuk multitasking sederhana.      |
| Tidak dapat menjalankan tugas lain selama jeda.   | Dapat menjalankan banyak tugas periodik sekaligus. |


3. Mengapa metode millis() disebut non-blocking

> Karena program tidak berhenti menunggu. Di dalam loop(), eksekusi terus berjalan, hanya pada setiap iterasi dicek apakah sudah mencapai interval. Selama belum mencapai interval, program bebas melakukan pekerjaan lain (misal membaca sensor, mengirim data, dll.). Tidak ada perintah yang menghentikan aliran program secara paksa seperti delay().



4. M4. Modifikasi program agar:
> - LED pertama berkedip setiap 1 detik
> - LED kedua berkedip setiap 500 ms
> - Tanpa menggunakan delay()

```c++
#include <Arduino.h>

unsigned long previousMillis = 0; // waktu terakhir LED berubah

const long interval = 1000; // interval kedip: 1000 ms

bool ledState = false; // status LED saat ini

void setup() {
    pinMode(13, OUTPUT);
    pinMode(6, OUTPUT);  // Pin 13 dan pin 6 sebagai output
}

void loop() {
    // Ambil waktu saat ini
    unsigned long currentMillis = millis();

    // Cek apakah sudah melewati interval
    if(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // simpan waktu terakhir
        ledState = !ledState; // toggle status LED
        digitalWrite(13, ledState);
        digitalWrite(6, ledState);// tulis ke pin LED
    }
}
```

## Dokumentasi

1. Percobaan 5A: Multitasking dengan FreeRTOS (LED + Potensiometer)

![Percobaan 5A](Dokumentasi_Percobaan_6A.jpg)

2. Percobaan 5B: Komunikasi Task dengan Queue

![Percobaan 5B](Dokumentasi_Percobaan_6B.jpg)
![Hasil Output 5B](Hasil_Dokumentasi_Percobaan_6B.png )

