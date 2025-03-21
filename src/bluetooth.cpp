#include "bluetooth.hpp"

#ifdef USE_HARDWARE_SERIAL
HardwareSerial& Bluetooth::s_bluetoothSerial = Serial;
#else
SoftwareSerial Bluetooth::s_bluetoothSerial(Config::BLUETOOTH_TX_ARDUINO_RX, Config::BLUETOOTH_RX_ARDUINO_TX);
#endif


int8_t Bluetooth::s_buffer[Config::BLUETOOTH_BUFFER];
int8_t Bluetooth::s_readIndex = 0;
bool Bluetooth::s_readAvailable = 0;

int Bluetooth::available()
{
    return s_bluetoothSerial.available();
}

int Bluetooth::read()
{
    return s_bluetoothSerial.read();
}

void Bluetooth::write(int8_t t_byte)
{
    s_bluetoothSerial.write(t_byte);
}

void Bluetooth::readToBuffer()
{
    while (available() && s_readIndex < Config::BLUETOOTH_BUFFER - 1) {
        int byteRead = read();
        if (byteRead == -1) {
            break;
        }
        
        if (byteRead == '\n') {
            s_buffer[s_readIndex] = 0;
            s_readAvailable = 1;
            break;
        }

        s_buffer[s_readIndex] = static_cast<int8_t>(byteRead);
        s_readIndex++;

    }
}


void Bluetooth::writeFromBuffer()
{
    uint8_t i = 0;
    if (s_buffer[i] == 0)
        return;

    while (s_buffer[i] != 0 && i < Config::BLUETOOTH_BUFFER) {
        s_bluetoothSerial.write(s_buffer[i]);
        ++i;
    }
    // s_readIndex = 0;
    // s_readAvailable = 0;
}


void Bluetooth::resetBuffer()
{
    s_readIndex = 0;
    s_readAvailable = 0;
}

void Bluetooth::begin(uint16_t baud)
{
    s_bluetoothSerial.begin(baud);
}

bool Bluetooth::readAvailable()
{
    return s_readAvailable;
}
