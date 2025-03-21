#ifndef BLUETOOTH_HEADER
#define BLUETOOTH_HEADER

#include <SoftwareSerial.h>
#include "config.hpp"

class Bluetooth {
    #ifdef USE_HARDWARE_SERIAL
    private:    static HardwareSerial& s_bluetoothSerial;
    #else
    private:    static SoftwareSerial s_bluetoothSerial;
    #endif
    public:     static int8_t s_buffer[Config::BLUETOOTH_BUFFER];
    private:    static int8_t s_readIndex;
    private:    static bool s_readAvailable;

    public:     static int available();
    public:     static int read();
    public:     static void write(int8_t t_byte);
    public:     static void readToBuffer();
    public:     static void writeFromBuffer();
    public:     static void resetBuffer();
    public:     static void begin(uint16_t baud);
    public:     static bool readAvailable();
};

#endif