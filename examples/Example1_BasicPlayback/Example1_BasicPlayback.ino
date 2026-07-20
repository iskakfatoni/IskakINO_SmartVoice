/*
 * Example 1: Basic Playback
 * ---------------------------------
 * Contoh paling dasar: inisialisasi modul, cek SD Card,
 * atur volume, lalu putar satu file dari root SD Card.
 *
 * Wiring (Arduino Uno / ESP8266 - gunakan pin digital biasa):
 *   DFPlayer RX -> pin 5 (via resistor 1K)  [NodeMCU: D8 / GPIO15]
 *   DFPlayer TX -> pin 4                    [NodeMCU: D7 / GPIO13]
 *   DFPlayer VCC -> 5V, GND -> GND
 *
 * Catatan: contoh ini pakai SoftwareSerial, jadi TIDAK untuk ESP32
 * (ESP32 core tidak menyertakan SoftwareSerial secara default).
 * Untuk ESP32, gunakan HardwareSerial seperti pada Example 2.
 *
 * File yang dibutuhkan di SD Card: 0001.mp3
 */

#include <SoftwareSerial.h>
#include <IskakINO_SmartVoice.h>

const uint8_t VOICE_RX_PIN = 4;
const uint8_t VOICE_TX_PIN = 5;

SoftwareSerial voiceSerial(VOICE_RX_PIN, VOICE_TX_PIN);
IskakINO_SmartVoice voice;

void setup() {
  Serial.begin(115200);
  voiceSerial.begin(9600);

  voice.begin(voiceSerial);

  if (voice.isSDCardReady()) {
    Serial.println(F("SD Card terdeteksi, memulai playback."));
    voice.setVolume(20);   // 0 - 30
    voice.playTrack(1);    // memutar 0001.mp3
  } else {
    Serial.println(F("SD Card tidak terdeteksi! Periksa wiring & kartu SD."));
  }
}

void loop() {
  // Kosong: playback berjalan sendiri di modul DFPlayer.
}
