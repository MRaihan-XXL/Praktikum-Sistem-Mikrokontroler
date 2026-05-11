#include <Arduino_FreeRTOS.h>

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

// Definisi pin
const int ledPin1 = 8;
const int ledPin2 = 7;
const int potPin = A0; // Pin analog untuk potensiometer

void setup() {
  // Inisialisasi komunikasi serial pada 9600 bits per second:
  Serial.begin(9600);

  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint,  "task3", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // Loop dibiarkan kosong karena eksekusi program dikendalikan oleh FreeRTOS Scheduler
}

void TaskBlink1(void *pvParameters) {
  pinMode(ledPin1, OUTPUT);
  while(1) {
    // Membaca nilai potensiometer (0 - 1023)
    int sensorValue = analogRead(potPin);
    
    // Memetakan nilai sensor menjadi nilai jeda (delay) antara 50 ms hingga 1000 ms
    int blinkDelay = map(sensorValue, 0, 1023, 50, 1000);

    Serial.print("Task1 Delay: ");
    Serial.println(blinkDelay);

    digitalWrite(ledPin1, HIGH);
    vTaskDelay( blinkDelay / portTICK_PERIOD_MS );
    digitalWrite(ledPin1, LOW);
    vTaskDelay( blinkDelay / portTICK_PERIOD_MS );
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(ledPin2, OUTPUT);
  while(1) {
    Serial.println("Task2");
    digitalWrite(ledPin2, HIGH);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
    digitalWrite(ledPin2, LOW);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.print("Counter: ");
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }
}
