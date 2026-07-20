# IskakINO_SmartVoice

Library untuk mengontrol modul **DFPlayer Mini** (MP3 Player) secara mudah dan stabil pada **ESP32 / ESP8266**, bagian dari ekosistem **IskakINO**.

## Fitur

- Kontrol MP3 via SD Card (mode Root & Folder).
- Fitur **Announce**: memutar suara selingan (misal notifikasi "Jam 7") lalu otomatis kembali ke lagu yang sedang diputar sebelumnya.
- Cek status SD Card (`isSDCardReady()`) dengan membaca & memvalidasi respons asli dari modul, bukan sekadar cek data tersedia.
- Cek status pemutaran secara realtime lewat pin BUSY (`isPlaying()`).
- Aman digunakan bersama `IskakINO_LiquidCrystal_I2C` dan `IskakINO_FastNTP`.

## Wiring

| DFPlayer Mini Pin | ESP8266 (NodeMCU/D1) | ESP32     | Keterangan |
|---|---|---|---|
| VCC (Pin 1)  | 5V (Vin)             | 5V (Vin) | Wajib 5V (jangan 3.3V) |
| RX  (Pin 2)  | D8 (via Resistor 1K) | GPIO 17  | Lewat resistor 1K |
| TX  (Pin 3)  | D7                   | GPIO 16  | Langsung |
| SPK1 (Pin 6) | Speaker (+)          | Speaker (+) | Max 3 Watt |
| GND (Pin 7)  | GND                  | GND      | Common ground |
| SPK2 (Pin 8) | Speaker (-)          | Speaker (-) | |
| BUSY (Pin 16)| D6 (opsional)        | GPIO 4   | LOW saat bunyi |

> **Catatan:** gunakan resistor 1K Ohm pada jalur RX DFPlayer untuk menghilangkan suara "kresek" akibat interferensi sinyal serial.

## Instalasi

1. Buka Arduino IDE → **Sketch > Include Library > Manage Libraries**.
2. Cari `IskakINO_SmartVoice`, klik **Install**.
3. Atau, download ZIP dari [GitHub](https://github.com/iskakfatoni/IskakINO_SmartVoice) dan **Sketch > Include Library > Add .ZIP Library**.

## Contoh Penggunaan

```cpp
#include <SoftwareSerial.h>
#include <IskakINO_SmartVoice.h>

SoftwareSerial voiceSerial(D7, D8); // RX, TX (ESP8266)
IskakINO_SmartVoice voice;

void setup() {
  voiceSerial.begin(9600);
  voice.begin(voiceSerial, D6); // Stream, busyPin (opsional)

  if (voice.isSDCardReady()) {
    voice.setVolume(20);
    voice.playTrack(1); // 0001.mp3 di root SD Card
  }
}

void loop() {
  if (!voice.isPlaying()) {
    // lagu selesai, lakukan sesuatu
  }
}
```

## API Reference

| Fungsi | Deskripsi |
|---|---|
| `begin(Stream& serial, uint8_t busyPin = 255, uint16_t bootDelayMs = 500)` | Inisialisasi modul. `busyPin` opsional agar `isPlaying()` tanpa argumen bisa dipakai. |
| `setVolume(uint8_t volume)` | Atur volume, 0–30. |
| `playTrack(uint16_t track)` | Putar file `NNNN.mp3` di root SD Card. |
| `playFromFolder(uint8_t folder, uint8_t track)` | Putar file dari folder `01`–`99`, file `001`–`255`. |
| `pause()` / `resume()` / `stop()` | Kontrol pemutaran dasar. |
| `announce(uint16_t track)` | Putar suara selingan lalu kembali otomatis ke lagu sebelumnya. |
| `isSDCardReady(uint16_t timeoutMs = 100)` | Cek fisik SD Card via respons modul. |
| `isPlaying()` | Cek status BUSY pin yang sudah diset di `begin()`. |
| `isPlaying(uint8_t busyPin)` | Cek status BUSY pin secara eksplisit. |

## Lisensi

MIT — lihat [LICENSE](LICENSE).
