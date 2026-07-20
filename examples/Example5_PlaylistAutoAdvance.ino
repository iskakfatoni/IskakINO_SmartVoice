/*
 * Example 5: Playlist Otomatis dengan Pin BUSY
 * ---------------------------------
 * Menunjukkan penggunaan pin BUSY (via isPlaying()) untuk mendeteksi
 * kapan sebuah track selesai diputar, lalu otomatis lanjut ke
 * track berikutnya (playlist sederhana), tanpa perlu delay() blocking.
 *
 * Wiring tambahan:
 *   DFPlayer BUSY -> D6 (ESP8266) / GPIO 4 (ESP32)
 */

#include <SoftwareSerial.h>
#include <IskakINO_SmartVoice.h>

SoftwareSerial voiceSerial(D7, D8);
IskakINO_SmartVoice voice;

const uint8_t BUSY_PIN = D6;
const uint16_t playlist[] = {1, 2, 3, 4}; // 0001.mp3 - 0004.mp3
const uint8_t playlistLen = sizeof(playlist) / sizeof(playlist[0]);

uint8_t currentIndex = 0;
bool wasPlaying = false;

void playCurrentTrack() {
  voice.playTrack(playlist[currentIndex]);
  Serial.print(F("Memutar track: "));
  Serial.println(playlist[currentIndex]);
}

void setup() {
  Serial.begin(115200);
  voiceSerial.begin(9600);

  // busyPin diset lewat begin() agar isPlaying() tanpa argumen bisa dipakai
  voice.begin(voiceSerial, BUSY_PIN);
  voice.setVolume(20);

  if (voice.isSDCardReady()) {
    playCurrentTrack();
  }
}

void loop() {
  bool isPlayingNow = voice.isPlaying();

  // Deteksi transisi dari "sedang main" -> "berhenti" = track selesai
  if (wasPlaying && !isPlayingNow) {
    currentIndex = (currentIndex + 1) % playlistLen;
    delay(300); // jeda singkat antar track, agar tidak salah baca status BUSY
    playCurrentTrack();
  }

  wasPlaying = isPlayingNow;
}
