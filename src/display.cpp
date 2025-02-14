#include "display.hpp"
#include "config.hpp"

Adafruit_SSD1306 Display::s_display{WIDTH, HEIGHT, &Wire, RESET};

boolean Display::setup()
{
    return s_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void Display::showText()
{
    s_display.clearDisplay();

	s_display.setTextSize(1);
	s_display.setTextColor(WHITE); 
	s_display.setCursor(0, 0);
	s_display.cp437(true);	

    for (int16_t i = 0; i < 256; i++)
	{
		if (i == '\n')
			Display::s_display.write(' ');
		else
			Display::s_display.write(i);
	}

    delay(2000); 
}

void Display::showUnsigned(uint16_t t_posX, uint16_t t_posY, uint32_t t_num)
{
	s_display.setTextSize(1);
	s_display.setTextColor(WHITE); 
	s_display.setCursor(t_posX, t_posY);
	s_display.cp437(true);

	s_display.print(t_num, 10);
}

void Display::drawPaddle(uint16_t t_posX, uint16_t t_posY)
{
	// Serial.println(t_posY);
	s_display.drawLine(t_posX, t_posY, t_posX, t_posY + Config::PADDLE_HEIGHT, WHITE);
}

void Display::drawRect(uint16_t t_posX, uint16_t t_posY, uint16_t t_width, uint16_t t_height)
{
	s_display.drawRect(t_posX, t_posY, t_width, t_height, WHITE);
}

void Display::drawHorizontalLine(uint16_t t_posY)
{
	s_display.drawLine(0, t_posY, WIDTH + 1, t_posY, WHITE);
}

void Display::drawPixel(uint16_t t_posX, uint16_t t_posY)
{
	s_display.drawPixel(t_posX, t_posY, WHITE);
}
