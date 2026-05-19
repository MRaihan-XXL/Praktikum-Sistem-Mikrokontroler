#include <Arduino.h>

// Variabel volatile agar dapat diubah dalam ISR
volatile bool ledState = false;

// ISR: dijalankan saat tombol ditekan (FALLING edge)
void tombolInterrupt() {
    ledState = !ledState; // Toggle status LED
}

void setup() {

    // Konfigurasi pin 13 sebagai output (LED)
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    
    // Konfigurasi pin 2 sebagai input dengan pull-up internal
    pinMode(2, INPUT_PULLUP);

    // Daftarkan ISR pada pin 2, dipicu FALLING (tombol ditekan)
    attachInterrupt(
        digitalPinToInterrupt(2),
        tombolInterrupt,
        FALLING
    );
}

void loop() {
    
    // Tulis status LED sesuai variabel ledState
    digitalWrite(13, ledState);
    digitalWrite(12, ledState);
}
