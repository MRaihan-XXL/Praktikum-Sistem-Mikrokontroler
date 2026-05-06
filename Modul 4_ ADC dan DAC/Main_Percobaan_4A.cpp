#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
// Tentukan pin yang digunakan untuk potensiometer dan servo
const int potensioPin = ;   // isi pin analog input (contoh A0)
const int servoPin = ;      // isi pin digital untuk servo (PWM)

// ===================== VARIABEL =====================
// Variabel untuk menyimpan data ADC dan sudut servo
int pos = ; // isi dengan tipe data dan inisialisasi awal
int val = ; // isi dengan tipe data dan inisialisasi awal

void setup() {

  // Hubungkan servo ke pin yang sudah ditentukan
  myservo.attach(); // isi dengan servoPin

  // Aktifkan komunikasi serial untuk monitoring
  Serial.begin(); // isi baud rate (contoh 9600)

}

void loop() {

  // ===================== PEMBACAAN ADC =====================
  // Baca nilai dari potensiometer (rentang 0–1023)
  val = analogRead(); // isi dengan potensioPin

  // ===================== KONVERSI DATA =====================
  // Ubah nilai ADC menjadi sudut servo (0–180 derajat)
  pos = map(val,
             0,   	// isi nilai minimum ADC
             1023,  // isi nilai maksimum ADC
             0,   	// isi sudut minimum servo
             180);  // isi sudut maksimum servo

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
