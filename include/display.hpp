#ifndef DISPLAY_HEADER
#define DISPLAY_HEADER
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdint.h>


class Display {
    public:     static constexpr uint8_t WIDTH = 128;
    public:     static constexpr uint8_t HEIGHT = 64;
    public:     static constexpr uint8_t RESET = 64;

    public:     static Adafruit_SSD1306 s_display;

    public:     static boolean setup();
    public:     static void showText();
    public:     static void showUnsigned(uint16_t t_posX, uint16_t t_posY, uint32_t t_num);
    public:     static void drawPaddle(uint16_t t_posX, uint16_t t_posY);
    public:     static void drawRect(uint16_t t_posX, uint16_t t_posY, uint16_t t_width, uint16_t t_height);
    public:     static void drawHorizontalLine(uint16_t t_posY);
    public:     static void drawPixel(uint16_t t_posX, uint16_t t_posY);
};

#endif