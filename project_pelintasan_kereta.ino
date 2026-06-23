#include <WiFi.h>
#include <MQTT.h>
#include <ESP32Servo.h>

// --- Kredensial Wi-Fi & Shiftr.io ---
const char* ssid = "AndroidAP";
const char* password = "icha1212";

// ISI SESUAI URL SHIFTR.IO KAMU:
const char* shiftrKey = "typhoonfighter543";       // Ini Username
const char* shiftrSecret = "rYYZjGURxHcPjJ4x";     // Ini Password

WiFiClient net;
MQTTClient client;
Servo servo;

// --- Definisi GPIO ---
const int irPin = 34;       
const int servoPin = 18;   
const int ledHijau = 4;        

// --- Variabel Global (Shared Resources) ---
volatile int statusIR = HIGH;
volatile int modeOtomatis = 1; 
volatile int posisiServo = 90;
unsigned long lastPublish = 0;

TaskHandle_t TaskHardwareHandle = NULL;
TaskHandle_t TaskWebHandle = NULL;

// --- Callback saat ada perintah masuk dari Shiftr.io (Website) ---
void messageReceived(String &topic, String &payload) {
  Serial.println("Pesan masuk: " + topic + " - " + payload);

  if (topic == "gate/cmd/mode") {
    modeOtomatis = !modeOtomatis;
    client.publish("gate/status/mode", String(modeOtomatis));
  }
  else if (topic == "gate/cmd/servo" && modeOtomatis == 0) { // Hanya jika mode manual
    posisiServo = payload.toInt();
    servo.write(posisiServo);
    if (posisiServo == 0) digitalWrite(ledHijau, HIGH);
    else digitalWrite(ledHijau, LOW);
    
    client.publish("gate/status/servo", String(posisiServo));
  }
}

// --- RTOS Task 1: Operasi Hardware (Core 1) ---
void TaskHardware(void * pvParameters) {
  for(;;) {
    if (modeOtomatis == 1) {
      int currentIR = digitalRead(irPin); 

      if (currentIR != statusIR) { // Hanya update jika ada perubahan deteksi
        statusIR = currentIR;
        if (statusIR == LOW) { 
          posisiServo = 0;
          servo.write(posisiServo);
          digitalWrite(ledHijau, HIGH);
          client.publish("gate/status/ir", "0");
          client.publish("gate/status/servo", "0");
          vTaskDelay(pdMS_TO_TICKS(1500)); 
        } 
        else {
          posisiServo = 90;
          servo.write(posisiServo);
          digitalWrite(ledHijau, LOW);
          client.publish("gate/status/ir", "1");
          client.publish("gate/status/servo", "90");
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50)); 
  }
}

// --- RTOS Task 2: Koneksi Jaringan MQTT Cloud (Core 0) ---
void TaskWeb(void * pvParameters) {
  for(;;) {
    client.loop();
    vTaskDelay(pdMS_TO_TICKS(10)); // Mencegah crash watchdog

    if (!client.connected()) {
      Serial.print("\nMenghubungkan ke MQTT shiftr.io...");
      while (!client.connect("esp32_gate", shiftrKey, shiftrSecret)) {
        Serial.print(".");
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
      Serial.println("\nMQTT Terhubung!");
      client.subscribe("gate/cmd/#"); // Menguping perintah dari web
      
      // Publish status awal setelah reconnect
      client.publish("gate/status/mode", String(modeOtomatis));
      client.publish("gate/status/servo", String(posisiServo));
      client.publish("gate/status/ir", String(statusIR));
    }

    // Publish berkala setiap 3 detik untuk sinkronisasi data
    if (millis() - lastPublish > 3000) {
      lastPublish = millis();
      client.publish("gate/status/mode", String(modeOtomatis));
      client.publish("gate/status/servo", String(posisiServo));
      client.publish("gate/status/ir", String(digitalRead(irPin)));
    }
  }
}

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);

  pinMode(irPin, INPUT);
  pinMode(ledHijau, OUTPUT);

  // Konek Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Terhubung!");

  // Inisialisasi MQTT client ke broker shiftr.io
  client.begin("typhoonfighter543.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  // Buat Task RTOS
  xTaskCreatePinnedToCore(TaskHardware, "Sistem_Hardware", 4096, NULL, 2, &TaskHardwareHandle, 1);
  xTaskCreatePinnedToCore(TaskWeb, "Sistem_MQTT_Cloud", 8192, NULL, 1, &TaskWebHandle, 0);
}

void loop() {
  // Kosong karena dijalankan oleh Scheduler FreeRTOS
}