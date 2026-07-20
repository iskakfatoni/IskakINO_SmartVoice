/*
 * Example 1: Basic Playback
 * ---------------------------------
 * Contoh paling dasar: inisialisasi modul, cek SD Card,
 * atur volume, lalu putar satu file dari root SD Card.
 *
 * Wiring (ESP8266 NodeMCU):
 *   DFPlayer RX -> D8 (via resistor 1K)
 *   DFPlayer TX -> D7
 *   DFPlayer VCC -> 5V, GND -> GND
 *
 * File yang dibutuhkan di SD Card: 0001.mp3
 */

#include <SoftwareSerial.h>
#include <IskakINO_SmartVoice.h>

SoftwareSerial voiceSerial(D7, D8); // RX, TX
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
