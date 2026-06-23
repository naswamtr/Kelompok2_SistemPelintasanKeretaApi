# 🚦 Railway Crossing Gate System (IoT)

Sistem Palang Pintu Perlintasan Kereta Api berbasis IoT menggunakan **ESP32**, **FreeRTOS**, **MQTT (Shiftr.io)**, **Servo Motor**, **Sensor IR**, dan **MockAPI** untuk monitoring serta controlling secara real-time melalui dashboard web.

---

## 👥 Team

### Kelompok 2

| Nama | NPM |
|--------|------------|
| Naswa Mutiara | 23552011185 |
| Nur Anisa | 23552011171 |

##

- Kelas : TIF K 23A 
- Mata Kuliah : Sistem Mikrokontroller
- Dosen Pengampu : Muhammad Ikhwan Fathulloh, S.Kom.

- Universitas Teknologi Bandung

---

## 📌 Features

- Monitoring status palang pintu secara real-time
- Deteksi objek menggunakan Sensor IR
- Kontrol Servo Motor otomatis dan manual
- Komunikasi cloud menggunakan MQTT Broker (Shiftr.io)
- Dashboard Web Monitoring & Controlling
- Penyimpanan log aktivitas menggunakan MockAPI
- Implementasi multitasking menggunakan FreeRTOS

---

## 🛠 Hardware Components

| Component | Description |
|------------|-------------|
| ESP32 | Main Microcontroller |
| IR Sensor | Object Detection |
| Servo Motor | Gate Actuator |
| LED | Status Indicator |
| WiFi Network | Internet Connection |

---

## 🔌 GPIO Configuration

| Component | GPIO |
|------------|------|
| IR Sensor | GPIO 34 |
| Servo Motor | GPIO 18 |
| LED Indicator | GPIO 4 |

---

## ⚙️ Technologies Used

### Embedded System
- ESP32
- Arduino IDE
- FreeRTOS
- MQTT Library
- ESP32Servo

### Web Dashboard
- HTML
- CSS
- JavaScript
- MQTT.js

### Cloud Services
- Shiftr.io MQTT Broker
- MockAPI

---

## 🏗 System Architecture

```text
Web Dashboard
      │
      ▼
 MQTT Broker (Shiftr.io)
      │
      ▼
    ESP32
 ┌────┴────┐
 ▼         ▼
IR Sensor Servo Motor
     │
     ▼
   LED
      │
      ▼
   MockAPI
```

---

## 📡 MQTT Topics

### Publish Topics

| Topic | Description |
|---------|-------------|
| gate/status/mode | System Mode |
| gate/status/servo | Servo Position |
| gate/status/ir | IR Sensor Status |

### Subscribe Topics

| Topic | Description |
|---------|-------------|
| gate/cmd/mode | Change System Mode |
| gate/cmd/servo | Manual Servo Control |

---

## 🔄 System Modes

### Automatic Mode
- Sensor IR mendeteksi objek.
- Servo bergerak otomatis.
- LED menyala saat objek terdeteksi.
- Data dikirim ke MQTT Broker.
- Dashboard diperbarui secara real-time.

### Manual Mode
- Kontrol dilakukan melalui dashboard web.
- Petugas dapat membuka atau menutup palang pintu secara manual.

---

## 📋 FreeRTOS Tasks

### TaskHardware (Core 1)
- Membaca sensor IR
- Mengontrol servo motor
- Mengontrol LED
- Publish status ke MQTT

### TaskWeb (Core 0)
- Menjaga koneksi MQTT
- Subscribe perintah dari dashboard
- Sinkronisasi data cloud
- Publish status berkala

---

## 🚀 Installation

### 1. Clone Repository

```bash
git clone https://github.com/naswamtr/Kelompok2_SistemPelintasanKeretaApi
```

### 2. Install Required Libraries

- WiFi
- MQTT
- ESP32Servo

### 3. Configure Credentials

Edit bagian berikut pada source code:

```cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

const char* shiftrKey = "YOUR_SHIFTR_USERNAME";
const char* shiftrSecret = "YOUR_SHIFTR_PASSWORD";
```

### 4. Upload to ESP32

- Select ESP32 Board
- Compile
- Upload

---

## 📊 Dashboard Features

- Real-time monitoring
- Manual gate control
- Mode switching (Automatic / Manual)
- Activity log display
- MQTT communication monitoring

---

## 📜 License

This project was developed for academic purposes as part of an Internet of Things (IoT) course project.

---

⭐ Developed by Kelompok 2 – TIF K 23 A
