#include <Arduino.h> // library dasar Arduino (tidak wajib diubah)

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan LED PWM
const int potPin = ;   // isi dengan pin analog (contoh A0)
const int ledPin = ;   // isi dengan pin digital PWM (contoh 9)

// ===================== VARIABEL =====================
// Variabel untuk menyimpan hasil pembacaan dan konversi PWM
int nilaiADC = ;  // isi dengan nilai awal (default 0)
int pwm = ;       // isi dengan nilai awal (default 0)

void setup() {

  // ===================== OUTPUT SETUP =====================
  // Atur pin LED sebagai output
  pinMode(ledPin, );

  // ===================== SERIAL MONITOR =====================
  // Aktifkan komunikasi serial untuk melihat data pembacaan
  Serial.begin(); // isi baud rate (contoh 9600)
}

void loop() {

  // ===================== PEMBACAAN SENSOR =====================
  // Baca nilai analog dari potensiometer (rentang 0–1023)
  nilaiADC = analogRead(); // isi dengan potPin

  // ===================== PEMROSESAN DATA (SCALING) =====================
  // Ubah nilai ADC (0–1023) menjadi nilai PWM (0–255)
  pwm = map(nilaiADC,
            ,   // isi nilai minimum ADC
            ,   // isi nilai maksimum ADC
            ,   // isi PWM minimum
            );  // isi PWM maksimum

  // ===================== OUTPUT PWM =====================
  // Kirim sinyal PWM ke LED (mengatur kecerahan)
  analogWrite(ledPin, ); // isi dengan variabel PWM

  // ===================== MONITORING DATA =====================
  // Tampilkan data ADC dan PWM ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(); // isi variabel ADC

  Serial.print(" | PWM: ");
  Serial.println(); // isi variabel PWM

  // ===================== STABILISASI SISTEM =====================
  // Delay untuk menstabilkan pembacaan dan tampilan data
  delay(); // isi dalam milidetik (contoh 50)
}
