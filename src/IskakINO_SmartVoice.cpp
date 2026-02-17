/*
 * IskakINO_SmartVoice.cpp
 * Implementasi protokol komunikasi DFPlayer Mini.
 * Optimized for iskakfatoni (2026-02-17)
 */

#ifndef ISKAKINO_SMARTVOICE_CPP
#define ISKAKINO_SMARTVOICE_CPP

#include "IskakINO_SmartVoice.h"

IskakINO_SmartVoice::IskakINO_SmartVoice() {}

void IskakINO_SmartVoice::begin(Stream& serial) {
    _voiceSerial = &serial;
    // Reset modul saat awal mulai
    sendRaw(0x0C, 0); 
    delay(500);
}

void IskakINO_SmartVoice::setVolume(uint8_t volume) {
    if (volume > 30) volume = 30;
    _vol = volume;
    sendRaw(0x06, _vol);
}

void IskakINO_SmartVoice::playTrack(uint16_t track) {
    sendRaw(0x03, track);
}

void IskakINO_SmartVoice::playFromFolder(uint8_t f, uint8_t t) {
    sendRaw(0x0F, f, t);
}

void IskakINO_SmartVoice::announce(uint16_t track) {
    sendRaw(0x13, track);
}

void IskakINO_SmartVoice::pause() {
    sendRaw(0x0E, 0);
}

void IskakINO_SmartVoice::resume() {
    sendRaw(0x0D, 0);
}

void IskakINO_SmartVoice::stop() {
    sendRaw(0x16, 0);
}

bool IskakINO_SmartVoice::isSDCardReady() {
    // Mengirim query status online (0x3F)
    sendRaw(0x3F, 0);
    delay(50);
    if (_voiceSerial->available()) {
        // Jika ada respon balik, berarti SD Card siap
        return true;
    }
    return false;
}

bool IskakINO_SmartVoice::isPlaying(uint8_t busyPin) {
    // Pin BUSY DFPlayer akan bernilai LOW saat memutar suara
    return (digitalRead(busyPin) == LOW);
}

// --- PRIVATE: Protokol Hex DFPlayer ---
void IskakINO_SmartVoice::sendRaw(uint8_t cmd, uint16_t arg) {
    sendRaw(cmd, (uint8_t)(arg >> 8), (uint8_t)(arg & 0xFF));
}

void IskakINO_SmartVoice::sendRaw(uint8_t cmd, uint8_t high, uint8_t low) {
    uint8_t packet[10];
    packet[0] = 0x7E;          // Start Byte
    packet[1] = 0xFF;          // Version
    packet[2] = 0x06;          // Data Length (fixed)
    packet[3] = cmd;           // Command
    packet[4] = 0x00;          // Feedback (0x00 = No, 0x01 = Yes)
    packet[5] = high;          // Parameter High Byte
    packet[6] = low;           // Parameter Low Byte
    
    // Checksum Calculation
    uint16_t checksum = -(packet[1] + packet[2] + packet[3] + packet[4] + packet[5] + packet[6]);
    packet[7] = (uint8_t)(checksum >> 8);
    packet[8] = (uint8_t)(checksum & 0xFF);
    packet[9] = 0xEF;          // End Byte

    _voiceSerial->write(packet, 10);
}
#endif
