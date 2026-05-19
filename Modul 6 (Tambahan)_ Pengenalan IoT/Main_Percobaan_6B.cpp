#include <Arduino.h>

unsigned long previousMillis = 0; // waktu terakhir LED berubah

const long interval = 1000; // interval kedip: 1000 ms

bool ledState = false; // status LED saat ini

void setup() {
    pinMode(13, OUTPUT); // Pin 13 sebagai output
}

void loop() {
    // Ambil waktu saat ini
    unsigned long currentMillis = millis();

    // Cek apakah sudah melewati interval
    if(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // simpan waktu terakhir
        ledState = !ledState; // toggle status LED
        digitalWrite(13, ledState);// tulis ke pin LED
    }
}
