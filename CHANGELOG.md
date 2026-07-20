# Changelog

Semua perubahan penting pada `IskakINO_SmartVoice` dicatat di file ini.

## [1.0.0] - 2026-02-17

Rilis pertama, setelah code review menyeluruh terhadap draft awal.

### Fixed
- `isSDCardReady()` sekarang membaca & memvalidasi frame respons asli dari DFPlayer Mini (start byte, command byte, checksum, end byte), bukan hanya mengecek `Serial.available()`. Sebelumnya bisa memberi hasil "SD ready" palsu akibat data sisa di buffer.
- Buffer serial dibersihkan sebelum mengirim query status, agar tidak salah membaca respons lama.
- `_voiceSerial` kini di-null-check di setiap fungsi publik, mencegah crash (null pointer) bila fungsi dipanggil sebelum `begin()`.
- Menghapus include guard yang tidak perlu pada `IskakINO_SmartVoice.cpp` (guard hanya relevan untuk header).

### Added
- `readResponse()` (private): parsing frame 10-byte DFPlayer dengan timeout berbasis `millis()` (tanpa `delay()` diam) untuk sinkronisasi dan validasi checksum/end byte.
- Parameter `busyPin` opsional pada `begin()`, sehingga `isPlaying()` bisa dipanggil tanpa argumen. Overload `isPlaying(uint8_t busyPin)` tetap tersedia untuk kompatibilitas.
- Validasi parameter dasar: `playTrack()`/`announce()` mengabaikan track `0`; `playFromFolder()` mengabaikan folder `0`/`>99` atau file `0`.
- Dokumentasi README lengkap (wiring, instalasi, API reference, contoh sketch), dipindahkan & diperluas dari komentar header.
- `keywords.txt` untuk syntax highlighting di Arduino IDE.

### Changed
- Klaim "non-blocking" pada header diperjelas: satu kali `delay()` singkat di `begin()` tetap diperlukan karena keterbatasan boot fisik modul DFPlayer Mini, sedangkan seluruh fungsi lain non-blocking.
