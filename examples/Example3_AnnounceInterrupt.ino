/*
 * Example 3: Announce (Interupsi Suara)
 * ---------------------------------
 * Menunjukkan fitur announce(): memutar suara selingan
 * (misal pengumuman waktu) di tengah lagu yang sedang diputar,
 * lalu otomatis kembali ke lagu tersebut setelah selesai.
 *
 * Cocok dipadukan dengan IskakINO_FastNTP untuk pengumuman jam,
 * misalnya "Sekarang jam 7" setiap jam berganti.
 *
 * File yang dibutuhkan di SD Card:
 *   0001.mp3 -> lagu latar / musik utama
 *   0002.mp3 -> file pengumuman ("Sekarang jam tujuh")
 */

#include <SoftwareSerial.h>
#include <IskakINO_SmartVoice.h>

SoftwareSerial voiceSerial(D7, D8);
IskakINO_SmartVoice voice;

unsigned long lastAnnounce = 0;
const unsigned long ANNOUNCE_INTERVAL = 60UL * 60UL * 1000UL; // tiap 1 jam (contoh)

void setup() {
  Serial.begin(115200);
  voiceSerial.begin(9600);

  voice.begin(voiceSerial);

  if (voice.isSDCardReady()) {
    voice.setVolume(20);
    voice.playTrack(1); // mulai lagu latar
  }

  lastAnnounce = millis();
}

void loop() {
  // Setiap ANNOUNCE_INTERVAL, selingi lagu dengan pengumuman lalu lanjut lagi.
  if (millis() - lastAnnounce >= ANNOUNCE_INTERVAL) {
    voice.announce(2); // DFPlayer otomatis kembali ke track 1 setelah ini selesai
    lastAnnounce = millis();
  }
}
