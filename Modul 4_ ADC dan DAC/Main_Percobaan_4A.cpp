// Percobaan 4A: ADC untuk mengontrol posisi servo
#include <Servo.h>          // library untuk servo [3]

Servo myservo;              // buat objek servo

const int potensioPin = A0; // potensiometer di pin A0 [2]
const int servoPin = 9;     // servo di pin 9 (PWM) [5]

int val = 0;   // menyimpan nilai ADC (0-1023)
int pos = 0;   // menyimpan sudut servo (0-180)

void setup() {

    myservo.attach(servoPin);   // hubungkan servo ke pin 9 [3]

    Serial.begin(9600);         // komunikasi serial untuk monitoring [4]

}

void loop() {
    // Baca nilai analog dari potensiometer (0-1023) [2]
    val = analogRead(potensioPin);
    
    // Mapping nilai ADC ke sudut servo (0-180 derajat) [4]
    pos = map(val, 
                0, 
                1023, 
                0, 
                180);
    
    // Gerakkan servo ke posisi yang sudah dipetakan [3]
    myservo.write(pos);
    
    // Tampilkan data ke Serial Monitor [5]
    Serial.print("ADC Potensio: ");
    Serial.print(val);
  
    Serial.print(" | Sudut Servo: ");
    Serial.println(pos);
    
    delay(100);  // delay 100 ms untuk stabilisasi servo (tanpa jitter)
}
