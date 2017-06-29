#include <stdint.h>
#include <iso646.h>
#include <Arduino.h>

const char *BASE16 = "0123456789abcdef";

//template<uint16_t SIZE>
class Buffer {
    public:
        uint16_t ptr;
        uint16_t size;
        uint8_t *buf = NULL;
        /*
        void reset();
        void addString(const char *value);
        void addString(String value);
        //void addString(const __FlashStringHelper *ifsh);
        void addFloat(double value, uint8_t precission, bool strip);
        void addCharArray(char *value, uint8_t len);
        void addLong(unsigned long value);
        void addWord(uint16_t value);
        void addByte(uint8_t value);*/

        void setBuffer(uint8_t *buffer, uint16_t newsize) {
            size = newsize;
            buf = buffer;
        }

        void reset() {
            ptr = 0;
        }

        // Add \0 terminated string, excluding \0
        void addString(const char *value) {
            int i = 0;
            while (value[i]) {
                if (ptr < size) {
                    buf[ptr++] = value[i++];
                }
            }
        }

        void addString(String value) {
            addString(value.c_str());
        }

        void addFloat(double value, uint8_t precission=2, bool strip=true) {
            #ifdef AVR
            dtostrf(value, 1, precission, _floatbuf);
            #else
            snprintf(_floatbuf, 16, "%.*f", precission, value);
            #endif

            if (precission and strip) {
                uint8_t e = 0;
                for (uint8_t i=0;i<16;i++) {
                    if (!_floatbuf[i]) {
                        e = i-1;
                        break;
                    }
                }
                for (uint8_t i=e; i; i--) {
                    if (_floatbuf[i] == '0') {
                        _floatbuf[i] = 0;
                    } else if (_floatbuf[i] == '.') {
                        _floatbuf[i] = 0;
                        break;
                    } else {
                        break;
                    }
                }
            }
            for (uint8_t i=0; i<16;i++) {
                if (_floatbuf[i] == 0) {
                    break;
                }
                if (_floatbuf[i] >= 'A' and _floatbuf[i] <= 'Z') {
                    _floatbuf[i] += 32; // str.tolower()
                }
            }
            addString(_floatbuf);
        }

        void addNumber(unsigned long long value) {
            snprintf(_floatbuf, 16, "%u", value);
            addString(_floatbuf);
        }

        void addSignedNumber(long long value) {
            snprintf(_floatbuf, 16, "%d", value);
            addString(_floatbuf);
        }

        void addCharArray(char *value, uint8_t len) {
            // Add char array to the output data buffer (databuf) 
            for (uint8_t i=0; i<len; i++) {
                if (ptr < size) {
                buf[ptr++] = value[i];
                }
            }
        }

        void addLong(unsigned long value) { // long, unsigned long
            buf[ptr++] = value >> 24 & 0xff;
            buf[ptr++] = value >> 16 & 0xff;
            buf[ptr++] = value >> 8 & 0xff;
            buf[ptr++] = value & 0xff;
        }

        void addWord(uint16_t value) { // word, int, unsigned int
            buf[ptr++] = value >> 8 & 0xff;
            buf[ptr++] = value & 0xff;
        }

        void addByte(uint8_t value) { // byte, char, unsigned char
            buf[ptr++] = value;
        }

    private:
        char _floatbuf[16] = "";
};