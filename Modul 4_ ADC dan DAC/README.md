# Pertemuan 4

## 4.5.4 Percobaan 4A: Analog to Digital Converter (ADC)

1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?

> berfungsi membaca tegangan analog pada pin A0 yang berasal dari potensiometer. Potensiometer yang diputar menghasilkan tegangan antara 0V (kiri penuh) hingga 5V (kanan penuh). Fungsi ini mengubah tegangan tersebut menjadi nilai digital 10-bit (0-1023). Semakin besar tegangan, semakin besar nilai ADC. Nilai inilah yang kemudian digunakan untuk mengontrol posisi servo setelah melalui proses map().

2. Mengapa diperlukan fungsi map() dalam program tersebut?

> Fungsi map() diperlukan karena rentang nilai ADC (0-1023) tidak sama dengan rentang sudut servo (0-180 derajat). Jika langsung menggunakan nilai ADC (misalnya myservo.write(val)), servo hanya akan bergerak pada rentang yang sangat sempit maka dari itu dengan map(val, 0, 1023, 0, 180), nilai 0-1023 dipetakan secara proporsional ke 0-180, sehingga setiap putaran potensiometer menghasilkan pergerakan servo yang tepat.

3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md 

```c++
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan servo
const int potensioPin = A0;  // isi pin analog input (contoh A0)
const int servoPin = 9;      // isi pin digital untuk servo (PWM)

// ===================== VARIABEL =====================
// Variabel untuk menyimpan data ADC dan sudut servo
int pos = 0; // isi dengan tipe data dan inisialisasi awal
int val = 0; // isi dengan tipe data dan inisialisasi awal

void setup() {

  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(servoPin); // isi dengan servoPin

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(9600); // isi baud rate (contoh 9600)

}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  // Baca nilai dari potensiometer (rentang 0–1023)
  val = analogRead(); // isi dengan potensioPin

  // ===================== KONVERSI DATA =====================
  // Ubah nilai ADC menjadi sudut servo (30–150 derajat)
  pos = map(val,
             0,   	// isi nilai minimum ADC
             1023,  // isi nilai maksimum ADC
             30,   	// isi sudut minimum servo
             150);  // isi sudut maksimum servo

  // ===================== OUTPUT SERVO =====================
  // Gerakkan servo sesuai hasil mapping
  myservo.write(); // isi dengan variabel sudut

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan sudut servo ke Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(); // isi variabel ADC

  Serial.print(" | Sudut Servo: ");
  Serial.println(); // isi variabel sudut

  // ===================== STABILISASI =====================
  // Delay untuk memberi waktu servo bergerak stabil
  delay(100); // isi dalam milidetik
}
```

## 4.6.4 Percobaan 4B: Pulse Width Modulation (PWM)

1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!

> LED pada dasarnya hanya memiliki dua keadaan: menyala penuh (HIGH, 5V) atau mati (LOW, 0V). Namun, dengan PWM, pin digital dinyalakan dan dimatikan sangat cepat (sekitar 500 kali per detik). Mata manusia tidak mampu melihat kedipan cepat ini, sehingga yang terlihat adalah kecerahan rata-rata. analogWrite(ledPin, pwm) mengatur duty cycle (persentase waktu HIGH dalam satu periode). Semakin besar nilai PWM (0-255), semakin besar duty cycle, semakin terang LED.

2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?

> Hubungannya adalah proporsional linear. ADC membaca tegangan analog 0-5V menghasilkan nilai 0-1023. Nilai ini kemudian dipetakan ke rentang PWM 0-255 menggunakan fungsi map() karena resolusi PWM hanya 8-bit (255 level), sedangkan ADC 10-bit (1023 level). 

3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.

```c++
#include <Arduino.h> // library dasar Arduino (tidak wajib diubah)

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan LED PWM
const int potPin = A0;   // isi dengan pin analog (contoh A0)
const int ledPin = 9;   // isi dengan pin digital PWM (contoh 9)

// ===================== VARIABEL =====================
// Variabel untuk menyimpan hasil pembacaan dan konversi PWM
int nilaiADC = 0;  // isi dengan nilai awal (default 0)
int pwm = 0;       // isi dengan nilai awal (default 0)

void setup() {

  // ===================== OUTPUT SETUP =====================
  // Atur pin LED sebagai output
  pinMode(ledPin, );

  // ===================== SERIAL MONITOR =====================
  // Aktifkan komunikasi serial untuk melihat data pembacaan
  Serial.begin(9600); // isi baud rate (contoh 9600)
}

void loop() {

  // ===================== PEMBACAAN SENSOR =====================
  // Baca nilai analog dari potensiometer (rentang 0–1023)
  nilaiADC = analogRead(potPin); // isi dengan potPin

  // ===================== PEMROSESAN DATA (SCALING) =====================
  // Ubah nilai ADC (0–1023) menjadi nilai PWM (0–255)
  pwm = map(nilaiADC,
            0,   // isi nilai minimum ADC
            1023,   // isi nilai maksimum ADC
            0,   // isi PWM minimum
            255);  // isi PWM maksimum

  // ===================== OUTPUT PWM =====================
  if (pwm >= 50 && pwm <= 200){ //ketika nilai pwm 50-200
  	analogWrite(ledPin, pwm);   // led menyala sesuai pwm
  } else {
	analogWrite(ledPin, 0);       // led mati 
  }

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan PWM ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC); // isi variabel ADC

  Serial.print(" | PWM: ");
  Serial.println(pwm); // isi variabel PWM

  // ===================== STABILISASI SISTEM =====================
  // Delay untuk menstabilkan pembacaan dan tampilan data
  delay(50); // isi dalam milidetik (contoh 50)
}
```

## Dokumentasi

1. Percobaan 4A: Analog to Digital Converter (ADC)

![Percobaan 4A](Dokumentasi_Percobaan_4A.jpg)

2. Percobaan 4B: Pulse Width Modulation (PWM)

![Percobaan 4B](Dokumentasi_Percobaan_4A.jpg)
