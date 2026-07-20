/*
 * =================================================================
 * Library: IskakINO_SmartVoice
 * Deskripsi: Library khusus pengontrol modul DFPlayer Mini (MP3 Player)
 * yang dioptimalkan untuk kemudahan penggunaan dan
 * kompatibilitas penuh dengan sistem IskakINO.
 *
 * Fitur:
 * - Kontrol MP3 via SD Card (Root & Folder mode).
 * - Fitur Announce (Interupsi suara tanpa memutus lagu utama).
 * - Cek status SD Card (dengan parsing respons asli, tanpa delay() panjang).
 * - Cek status pemutaran secara realtime via pin BUSY.
 * - Aman digunakan bersama IskakINO_LiquidCrystal_I2C & FastNTP.
 *
 * Catatan performa: begin() melakukan satu kali delay() singkat karena
 * modul DFPlayer Mini secara fisik butuh waktu boot setelah power-on;
 * ini adalah keterbatasan hardware, bukan kekurangan library. Semua
 * fungsi lain (playTrack, setVolume, dst) bersifat non-blocking.
 *
 * Author: iskakfatoni
 * Date: 2026-02-17
 * =================================================================
 *
 * [ WIRING GUIDE ]
 *
 * DFPlayer Mini Pin  ->  ESP8266 (NodeMCU/D1) -> ESP32        -> Keterangan
 * -------------------------------------------------------------------------
 * VCC (Pin 1)        ->  5V (Vin)             -> 5V (Vin)    -> Wajib 5V (Jangan 3.3V)
 * RX  (Pin 2)        ->  D8 (via Resistor 1K) -> GPIO 17     -> Lewat resistor 1K
 * TX  (Pin 3)        ->  D7                   -> GPIO 16     -> Langsung
 * SPK1(Pin 6)        ->  Speaker (+)          -> Speaker (+) -> Max 3 Watt
 * GND (Pin 7)        ->  GND                  -> GND         -> Common Ground
 * SPK2(Pin 8)        ->  Speaker (-)          -> Speaker (-)
 * BUSY(Pin 16)       ->  D6 (Optional)        -> GPIO 4      -> Low saat bunyi
 *
 * *Catatan: Gunakan Resistor 1K Ohm pada jalur RX DFPlayer untuk menghilangkan
 * suara 'kresek' atau noise akibat interferensi sinyal serial.
 * =================================================================
 */

#ifndef ISKAKINO_SMARTVOICE_H
#define ISKAKINO_SMARTVOICE_H

#include <Arduino.h>

class IskakINO_SmartVoice {
  private:
    Stream* _voiceSerial = nullptr;
    uint8_t _vol = 20;
    uint8_t _busyPin = 255;   // 255 = belum dikonfigurasi

    // Protokol komunikasi internal DFPlayer Mini
    void sendRaw(uint8_t cmd, uint16_t arg);
    void sendRaw(uint8_t cmd, uint8_t high, uint8_t low);

    // Membaca & memvalidasi satu frame respons dari DFPlayer.
    // Non-blocking terhadap CPU (tidak delay() diam), dibatasi timeoutMs.
    // Mengembalikan true jika frame valid diterima sebelum timeout.
    bool readResponse(uint8_t &cmdOut, uint16_t &paramOut, uint16_t timeoutMs = 100);

  public:
    IskakINO_SmartVoice();

    /**
     * @brief Inisialisasi modul suara
     * @param serial Gunakan SoftwareSerial (ESP8266) atau HardwareSerial (ESP32)
     * @param busyPin (opsional) Pin BUSY DFPlayer, agar isPlaying() bisa dipanggil
     *                tanpa parameter. Isi 255 (default) jika tidak dipakai.
     * @param bootDelayMs (opsional) Waktu tunggu boot modul, default 500ms.
     */
    void begin(Stream& serial, uint8_t busyPin = 255, uint16_t bootDelayMs = 500);

    // --- Kontrol Dasar ---
    void setVolume(uint8_t volume);             // Range 0 - 30
    void playTrack(uint16_t track);              // File 0001.mp3 di ROOT SD Card
    void playFromFolder(uint8_t f, uint8_t t);   // Folder 01-99, File 001-255
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
     * @brief Verifikasi fisik SD Card dengan membaca & memvalidasi
     * respons asli dari modul (bukan sekadar cek Serial.available()).
     * @param timeoutMs Batas waktu tunggu respons (default 100ms).
     * @return true jika SD card terbaca dan siap diputar
     */
    bool isSDCardReady(uint16_t timeoutMs = 100);

    /**
     * @brief Cek status aktif modul via pin BUSY (hardware feedback),
     * menggunakan pin yang sudah diset lewat begin().
     * @return true jika sedang memutar suara
     */
    bool isPlaying();

    /**
     * @brief Cek status aktif modul via pin BUSY (hardware feedback)
     * @param busyPin Hubungkan ke pin BUSY DFPlayer (Active Low)
     */
    bool isPlaying(uint8_t busyPin);
};

// Instance global IskakVoice
//extern IskakINO_SmartVoice IskakVoice;

#endif
