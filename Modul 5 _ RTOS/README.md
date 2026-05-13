# Pertemuan 5

## 5.5.4 Percobaan 5A: Multitasking dengan FreeRTOS (LED + Potensiometer)

1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

> Ketiga task berjalan secara bergantian (concurrent), bukan bersamaan (paralel murni) karena Arduino Uno hanya memiliki satu inti prosesor. Mekanismenya diatur oleh scheduler FreeRTOS. Scheduler membagi waktu prosesor secara bergantian ke setiap task dalam waktu yang sangat singkat. Ketika suatu task memanggil vTaskDelay(), task tersebut masuk ke status blocked dan scheduler langsung beralih ke task lain yang siap berjalan. Karena pergantian task terjadi sangat cepat, efeknya terlihat seolah semua task berjalan bersamaan.
> 
> Pada program ini, ketiga task memiliki prioritas yang sama (prioritas 1). Scheduler menggunakan skema round‑robin dengan time slicing, sehingga setiap task mendapat jatah waktu eksekusi secara bergantian.


2. Mengapa diperlukan fungsi map() dalam program tersebut?

> Langkah menambahkan task keempat:
> 1. Deklarasikan fungsi task baru, misal void TaskBaru(void *pvParameters).
> 2. Definisikan fungsi tersebut di bawah loop() dengan infinite loop while(1) dan vTaskDelay().
> 3. Di dalam setup(), panggil xTaskCreate(TaskBaru, "task4", 128, NULL, 1, NULL).
> 4. Pastikan stack size (128) cukup untuk task baru. Jika tidak, tingkatkan (misal 256).
> 5. Upload ulang program.
> 
> Contoh task keempat untuk menyalakan LED di pin 6 setiap 700 ms:
```c++
void TaskBaru(void *pvParameters) {
  pinMode(6, OUTPUT);
  while(1) {
    digitalWrite(6, HIGH);
    vTaskDelay(700 / portTICK_PERIOD_MS);
    digitalWrite(6, LOW);
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}
```


3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md.

```c++
#include <Arduino_FreeRTOS.h>

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);
void TaskPot(void *pvParameters);

const int ledPin1 = 8;   // LED merah (kecepatan dikontrol potensio)
const int ledPin2 = 7;   // LED kuning
const int potPin = A0;   // potensiometer

int potValue = 0;
int delayTime = 200;      // delay default (ms)

void setup() {
  Serial.begin(9600);
  
  // Membuat 4 task dengan prioritas sama (1)
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint,  "task3", 128, NULL, 1, NULL);
  xTaskCreate(TaskPot,    "task4", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();   // Mulai RTOS scheduler
}

void loop() {
  // Kosong – semua eksekusi ditangani oleh scheduler
}

// Task 1: LED merah (pin 8) – kecepatan mengikuti potensiometer
void TaskBlink1(void *pvParameters) {
  pinMode(ledPin1, OUTPUT);
  while (1) {
    digitalWrite(ledPin1, HIGH);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    digitalWrite(ledPin1, LOW);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    Serial.print("Task1 delay: ");
    Serial.println(delayTime);
  }
}

// Task 2: LED kuning (pin 7) – kecepatan 2× lebih lambat dari LED merah
void TaskBlink2(void *pvParameters) {
  pinMode(ledPin2, OUTPUT);
  while (1) {
    digitalWrite(ledPin2, HIGH);
    vTaskDelay(2 * delayTime / portTICK_PERIOD_MS);
    digitalWrite(ledPin2, LOW);
    vTaskDelay(2 * delayTime / portTICK_PERIOD_MS);
  }
}

// Task 3: Mencetak counter setiap 500ms (untuk monitoring)
void Taskprint(void *pvParameters) {
  int counter = 0;
  while (1) {
    counter++;
    Serial.print("Counter: ");
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task 4: Membaca potensiometer dan mengupdate delayTime
void TaskPot(void *pvParameters) {
  while (1) {
    potValue = analogRead(A0);
    // Mapping nilai ADC 0-1023 ke delay 100ms – 1000ms
    delayTime = map(potValue, 0, 1023, 100, 1000);
	
    vTaskDelay(100 / portTICK_PERIOD_MS);   // update setiap 100ms
  }
}
```


## 5.6.4 Percobaan 5B: Komunikasi Task dengan Queue

1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!

> Sama seperti percobaan 5A, kedua task (read_data dan display) berjalan secara bergantian (concurrent) di bawah pengelolaan scheduler FreeRTOS. Task read_data mengirim data ke queue, lalu vTaskDelay(100) menyebabkan task tersebut blocked selama 100 ms, sehingga scheduler beralih ke task display. Task display menunggu data dari queue dengan xQueueReceive(..., portMAX_DELAY). Jika queue kosong, task display juga blocked. Ketika read_data mengirim data, queue menjadi tidak kosong, maka task display di‑unblock dan langsung menampilkan data. Setelah selesai, scheduler dapat beralih lagi ke task lain.


2. Apakah program ini berpotensi mengalami race condition? Jelaskan!

> Program ini tidak berpotensi mengalami race condition karena queue FreeRTOS sudah dirancang sebagai mekanisme komunikasi antar task yang thread‑safe. Race condition terjadi ketika dua task mengakses sumber daya bersama secara bersamaan tanpa sinkronisasi. Pada program queue:
> 1. xQueueSend dan xQueueReceive adalah fungsi yang aman (dilindungi oleh critical section internal FreeRTOS).
> 2. Data dikirim melalui queue (by copy), bukan by reference, sehingga tidak ada akses langsung ke variabel yang sama.
Jika dua task mencoba mengirim ke queue secara bersamaan, mekanisme internal queue akan menjadwalkan akses secara berurutan.


3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md.

> Hasil modifikasi (menggunakan sensor DHT11) menghasilkan pembacaan suhu dan kelembaban secara dinamis sesuai kondisi lingkungan. Program:

```c++
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

struct readings {
  float temp;
  float hum;
};

QueueHandle_t my_queue;

void read_data(void *pvParameters) {
  struct readings data;
  while(1) {
    data.temp = dht.readTemperature();
    data.hum = dht.readHumidity();
    xQueueSend(my_queue, &data, portMAX_DELAY);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void display_data(void *pvParameters) {
  struct readings data;
  while(1) {
    if(xQueueReceive(my_queue, &data, portMAX_DELAY) == pdPASS) {
      Serial.print("Suhu : ");
      Serial.print(data.temp);
      Serial.println(" C");
      Serial.print("Kelembaban : ");
      Serial.print(data.hum);
      Serial.println(" %");
    }
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  my_queue = xQueueCreate(5, sizeof(struct readings));
  xTaskCreate(read_data, "read", 128, NULL, 1, NULL);
  xTaskCreate(display_data, "display", 128, NULL, 1, NULL);
}
void loop() {}
```

## Dokumentasi

1. Percobaan 4A: Analog to Digital Converter (ADC)

![Percobaan 4A](Dokumentasi_Percobaan_4A.jpg)

2. Percobaan 4B: Pulse Width Modulation (PWM)

![Percobaan 4B](Dokumentasi_Percobaan_4A.jpg)
