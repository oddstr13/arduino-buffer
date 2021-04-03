#ifndef Buffer_h
#define Buffer_h

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

        void setBuffer(uint8_t *buffer, uint16_t newsize) {
            size = newsize;
            buf = buffer;
        }

        void reset() {
            ptr = 0;
        }
        
        void add(char value) { // byte, char, unsigned char
            buf[ptr++] = value;
        }

        // Add \0 terminated string, excluding \0
        void add(const char *value) {
            int i = 0;
            while (value[i]) {
                if (ptr < size) {
                    buf[ptr++] = value[i++];
                }
            }
        }

        void add(String value) {
            add(value.c_str());
        }

        void addNumber(double value, uint8_t precission=2, bool strip=true) {
            // TODO: replace most of this code with String(), but keep strip functionality
            _floatbuf = String(value, precission);

            if (precission and strip) {
                for (uint8_t i=_floatbuf.length(); i; i--) {
                    char c = _floatbuf.charAt(i);
                    if (not c) {
                        // Skip null bytes, just in case we should have started at length()-1
                        // TODO: verify if this is needed, and remove this check if it isn't.
                    } else if (c == '0') {
                        _floatbuf.remove(i);
                    } else if (c == '.') {
                        _floatbuf.remove(i);
                        break;
                    } else {
                        break;
                    }
                }
            }
            _floatbuf.toLowerCase(); // NaN -> nan
            add(_floatbuf);
        }

        void addNumber(unsigned long value, uint8_t base=DEC) {
            add(String(value, base).c_str());
        }

        void addNumber(long value, uint8_t base=DEC) {
            add(String(value, base).c_str());
        }

        void add(const char *value, uint16_t len) {
            // Add char array to the output data buffer (databuf) 
            for (uint16_t i=0; i<len; i++) {
                if (ptr < size) {
                    buf[ptr++] = value[i];
                }
            }
        }

        void addBytes(unsigned long value) { // long, unsigned long
            buf[ptr++] = value >> 24 & 0xff;
            buf[ptr++] = value >> 16 & 0xff;
            buf[ptr++] = value >> 8 & 0xff;
            buf[ptr++] = value & 0xff;
        }

        void addBytes(uint16_t value) { // word, int, unsigned int
            buf[ptr++] = value >> 8 & 0xff;
            buf[ptr++] = value & 0xff;
        }

        void addBytes(uint8_t value) { // byte, char, unsigned char
            buf[ptr++] = value;
        }

    private:
        String _floatbuf;
};

#endif