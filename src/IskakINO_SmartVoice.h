/*
 * =================================================================
 * Library: IskakINO_SmartVoice
 * Deskripsi: Library khusus pengontrol modul DFPlayer Mini (MP3 Player)
 * yang dioptimalkan untuk performa non-blocking dan 
 * kompatibilitas penuh dengan sistem IskakINO.
 * * Fitur:
 * - Kontrol MP3 via SD Card (Root & Folder mode).
 * - Fitur Announce (Interupsi suara tanpa memutus lagu utama).
 * - Cek status SD Card & status pemutaran secara realtime.
 * - Aman digunakan bersama IskakINO_LiquidCrystal_I2C & FastNTP.
 * * Author: iskakfatoni
 * Date: 2026-02-17
 * =================================================================
 * * [ WIRING GUIDE ]
 * * DFPlayer Mini Pin  ->  ESP8266 (NodeMCU/D1) -> ESP32        -> Keterangan
 * -------------------------------------------------------------------------
 * VCC (Pin 1)        ->  5V (Vin)             -> 5V (Vin)    -> Wajib 5V (Jangan 3.3V)
 * RX  (Pin 2)        ->  D8 (via Resistor 1K) -> GPIO 17     -> Lewat resistor 1K
 * TX  (Pin 3)        ->  D7                   -> GPIO 16     -> Langsung
 * SPK1(Pin 6)        ->  Speaker (+)          -> Speaker (+) -> Max 3 Watt
 * GND (Pin 7)        ->  GND                  -> GND         -> Common Ground
 * SPK2(Pin 8)        ->  Speaker (-)          -> Speaker (-)
 * BUSY(Pin 16)       ->  D6 (Optional)        -> GPIO 4      -> Low saat bunyi
 * * *Catatan: Gunakan Resistor 1K Ohm pada jalur RX DFPlayer untuk menghilangkan 
 * suara 'kresek' atau noise akibat interferensi sinyal serial.
 * =================================================================
 */

#ifndef ISKAKINO_SMARTVOICE_H
#define ISKAKINO_SMARTVOICE_H

#include <Arduino.h>

class IskakINO_SmartVoice {
  private:
    Stream* _voiceSerial;
    uint8_t _vol = 20;
    
    // Protokol komunikasi internal DFPlayer Mini
    void sendRaw(uint8_t cmd, uint16_t arg);
    void sendRaw(uint8_t cmd, uint8_t high, uint8_t low);

  public:
    IskakINO_SmartVoice();
    
    /**
     * @brief Inisialisasi modul suara
     * @param serial Gunakan SoftwareSerial (ESP8266) atau HardwareSerial (ESP32)
     */
    void begin(Stream& serial);
    
    // --- Kontrol Dasar ---
    void setVolume(uint8_t volume);            // Range 0 - 30
    void playTrack(uint16_t track);            // File 0001.mp3 di ROOT SD Card
    void playFromFolder(uint8_t f, uint8_t t);  // Folder 01-99, File 001-255
    void pause();
    void resume();
    void stop();
    
    // --- Fitur Lanjut ---
    /**
     * @brief Announce: Memutar suara selingan (misal: "Jam 7"), 
     * lalu otomatis kembali ke musik yang diputar sebelumnya.
     */
    void announce(uint16_t track); 
    
    /**
     * @brief Verifikasi fisik SD Card
     * @return true jika SD card terbaca dan siap diputar
     */
    bool isSDCardReady();
    
    /**
     * @brief Cek status aktif modul via pin BUSY (hardware feedback)
     * @param busyPin Hubungkan ke pin BUSY DFPlayer (Active Low)
     */
    bool isPlaying(uint8_t busyPin);
};

// Instance global IskakVoice
//extern IskakINO_SmartVoice IskakVoice;

#endif
