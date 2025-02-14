#ifndef CONFIG_HEADER
#define CONFIG_HEADER
#include "display.hpp"
#include "pair.hpp"

class Config {
    public:     static constexpr uint16_t const LEFT_PADDLE_X = 10;
    public:     static constexpr uint16_t const RIGHT_PADDLE_X = Display::WIDTH - 10;
    public:     static constexpr uint32_t const SAFE_OFFSET = 20;
    public:     static constexpr uint32_t const DISTANCE_LOWER = 0;
    public:     static constexpr uint32_t const DISTANCE_UPPER = 400;
    public:     static constexpr uint32_t const DISTANCE_TOTAL = DISTANCE_UPPER - DISTANCE_LOWER - SAFE_OFFSET * 2; 
    public:     static constexpr uint16_t const PADDLE_PADDING_UPPER = 8;
    public:     static constexpr uint16_t const PADDLE_PADDING_LOWER = 2;
    public:     static constexpr uint16_t const PADDLE_HEIGHT = 16;
    public:     static const Pair<uint16_t, uint16_t> BALL_RATE;
};

#endif