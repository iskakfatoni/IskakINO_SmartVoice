/*
 * Example 2: Folder Playback
 * ---------------------------------
 * Menunjukkan cara memutar file yang diorganisir dalam folder
 * di SD Card, misalnya untuk memisahkan kategori suara:
 * 01/ = Musik, 02/ = Notifikasi, 03/ = Alarm, dst.
 *
 * Struktur SD Card yang dibutuhkan:
 *   /01/001.mp3
 *   /01/002.mp3
 *   /02/001.mp3
 *
 * Wiring (ESP32):
 *   DFPlayer RX -> GPIO 17
 *   DFPlayer TX -> GPIO 16
 */

#include <IskakINO_SmartVoice.h>

HardwareSerial voiceSerial(2); // UART2 pada ESP32
IskakINO_SmartVoice voice;

const uint8_t FOLDER_MUSIK       = 1;
const uint8_t FOLDER_NOTIFIKASI  = 2;

void setup() {
  Serial.begin(115200);
  voiceSerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  voice.begin(voiceSerial);
  voice.setVolume(22);

  if (voice.isSDCardReady()) {
    // Putar file /01/001.mp3
    voice.playFromFolder(FOLDER_MUSIK, 1);
  } else {
    Serial.println("SD Card tidak terdeteksi!");
  }
}

void loop() {
  // Contoh: setiap tombol ditekan, putar suara notifikasi dari folder 02
  // (logika pembacaan tombol disederhanakan untuk contoh ini)
  static unsigned long lastTrigger = 0;
  bool tombolDitekan = false; // ganti dengan pembacaan pin tombol asli

  if (tombolDitekan && millis() - lastTrigger > 1000) {
    voice.playFromFolder(FOLDER_NOTIFIKASI, 1);
    lastTrigger = millis();
  }
}
