#ifndef CONFIG_HEADER
#define CONFIG_HEADER
#include "display.hpp"
#include "pair.hpp"

#define USE_HARDWARE_SERIAL

#ifdef USE_HARDWARE_SERIAL
    #define BLUETOOTH_BUFFER_SIZE 63
    #define PLAYER_NAME_BUFFER_SIZE 31
#else
    #define BLUETOOTH_BUFFER_SIZE 31
    #define PLAYER_NAME_BUFFER_SIZE 15
#endif
class Config {
    public:     static constexpr uint16_t const LEFT_PADDLE_X = 10;
    public:     static constexpr uint16_t const RIGHT_PADDLE_X = Display::WIDTH - 10;
    public:     static constexpr uint16_t const SAFE_OFFSET = 20;
    public:     static constexpr uint16_t const DISTANCE_LOWER = 0;
    public:     static constexpr uint16_t const DISTANCE_UPPER = 200;
    public:     static constexpr uint16_t const DISTANCE_TOTAL = DISTANCE_UPPER - DISTANCE_LOWER - SAFE_OFFSET * 2; 
    public:     static constexpr uint8_t const PADDLE_PADDING_UPPER = 8;
    public:     static constexpr uint8_t const PADDLE_PADDING_LOWER = 2;
    public:     static constexpr uint8_t const PADDLE_HEIGHT = 16;
    public:     static constexpr uint8_t const BLUETOOTH_TX_ARDUINO_RX = 2;
    public:     static constexpr uint8_t const BLUETOOTH_RX_ARDUINO_TX = 3;
    // public:     static constexpr uint8_t const BLUETOOTH_BUFFER = 15;
    public:     static constexpr uint8_t const BLUETOOTH_BUFFER = BLUETOOTH_BUFFER_SIZE;
    public:     static constexpr uint8_t const PLAYER_NAME_BUFFER = PLAYER_NAME_BUFFER_SIZE;
    public:     static const Pair<uint8_t, uint8_t> BALL_RATE;
};

#endif