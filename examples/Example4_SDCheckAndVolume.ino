/*
 * Example 4: SD Card Check dengan Retry + Kontrol Volume
 * ---------------------------------
 * Menunjukkan cara memverifikasi SD Card secara andal (dengan retry,
 * karena modul kadang butuh beberapa percobaan setelah power-on),
 * serta mengatur volume secara dinamis (misal via potensiometer).
 */

#include <SoftwareSerial.h>
#include <IskakINO_SmartVoice.h>

SoftwareSerial voiceSerial(D7, D8);
IskakINO_SmartVoice voice;

const uint8_t POT_PIN = A0;
bool sdReady = false;

bool waitForSDCard(uint8_t maxRetry, uint16_t delayBetweenMs) {
  for (uint8_t i = 0; i < maxRetry; i++) {
    if (voice.isSDCardReady()) {
      return true;
    }
    delay(delayBetweenMs);
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  voiceSerial.begin(9600);

  voice.begin(voiceSerial);

  sdReady = waitForSDCard(5, 200); // coba 5x, jeda 200ms antar percobaan

  if (sdReady) {
    Serial.println(F("SD Card siap."));
    voice.playTrack(1);
  } else {
    Serial.println(F("SD Card gagal terdeteksi setelah beberapa percobaan."));
  }
}

void loop() {
  if (!sdReady) return;

  // Baca potensiometer (0-1023) lalu petakan ke rentang volume DFPlayer (0-30)
  int rawValue = analogRead(POT_PIN);
  uint8_t targetVolume = map(rawValue, 0, 1023, 0, 30);

  static uint8_t lastVolume = 255;
  if (targetVolume != lastVolume) {
    voice.setVolume(targetVolume);
    lastVolume = targetVolume;
  }

  delay(100); // hindari spam perintah volume terlalu sering
}
