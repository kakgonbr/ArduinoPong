#include <Arduino.h>
#include "display.hpp"
#include "sensor.hpp"
#include "config.hpp"
#include "pair.hpp"
#include "game.hpp"

Sensor leftSensor(11, 10);
Sensor rightSensor(13, 12);
Pair<uint8_t, uint8_t> leftPaddle;
Pair<uint8_t, uint8_t> rightPaddle;

uint16_t leftDistance;
uint16_t rightDistance;

void gameLoop() 
{
	Game::tick(leftPaddle, rightPaddle);

	leftDistance = leftSensor.getDistance();
	rightDistance = rightSensor.getDistance();

	if (leftDistance < Config::SAFE_OFFSET)
	{
		leftDistance = Config::SAFE_OFFSET;
	}

	if (leftDistance > Config::DISTANCE_UPPER - Config::SAFE_OFFSET)
	{
		leftDistance = Config::DISTANCE_UPPER - Config::SAFE_OFFSET;
	}

	if (rightDistance < Config::SAFE_OFFSET)
	{
		rightDistance = Config::SAFE_OFFSET;
	}

	if (rightDistance > Config::DISTANCE_UPPER - Config::SAFE_OFFSET)
	{
		rightDistance = Config::DISTANCE_UPPER - Config::SAFE_OFFSET;
	}

	// possible values after adjustment: [SAFE_OFFSET, DISTANCE_UPPER - SAFE_OFFSET]

	Display::s_display.clearDisplay();

	// ultrasonic distance range: 1-400, add safety offset
	leftPaddle = {Config::LEFT_PADDLE_X, static_cast<uint8_t>((static_cast<float>(leftDistance - Config::SAFE_OFFSET) / Config::DISTANCE_TOTAL) * (Display::HEIGHT - Config::PADDLE_PADDING_UPPER - Config::PADDLE_PADDING_LOWER - Config::PADDLE_HEIGHT) + Config::PADDLE_PADDING_UPPER)};

	rightPaddle = {Config::RIGHT_PADDLE_X, static_cast<uint8_t>((static_cast<float>(rightDistance - Config::SAFE_OFFSET) / Config::DISTANCE_TOTAL) * (Display::HEIGHT - Config::PADDLE_PADDING_UPPER - Config::PADDLE_PADDING_LOWER - Config::PADDLE_HEIGHT) + Config::PADDLE_PADDING_UPPER)};

	// ball
	Display::drawPixel(static_cast<uint16_t>(Game::getBall().left),
					   static_cast<uint16_t>(Game::getBall().right));

	Display::drawPaddle(static_cast<uint16_t>(leftPaddle.left),
						static_cast<uint16_t>(leftPaddle.right));
	Display::drawPaddle(static_cast<uint16_t>(rightPaddle.left),
						static_cast<uint16_t>(rightPaddle.right));

	Display::showUnsigned(10, 0, Game::getScore().left);
	Display::showUnsigned(110, 0, Game::getScore().right);

	// Display::drawRect(0, Config::PADDLE_PADDING_UPPER, Display::WIDTH + 1, Display::HEIGHT - Config::PADDLE_PADDING_UPPER - Config::PADDLE_PADDING_LOWER + 1);

	Display::drawHorizontalLine(Config::PADDLE_PADDING_UPPER);
	Display::drawHorizontalLine(Display::HEIGHT - Config::PADDLE_PADDING_LOWER + 1);

	Display::s_display.display();
}

void preGame() 
{
	Display::s_display.clearDisplay();
	Display::s_display.setCursor(Display::WIDTH / 2 - 5*5, Display::HEIGHT / 2 - 10);
	Display::s_display.write("Pong!");
	Display::s_display.display();
	Game::preGame();
	Game::throwBall(static_cast<uint8_t>(random(4)));
	Game::ended = 0;
}

void setup()
{
	// Serial.begin(9600);
	Bluetooth::begin(9600);
	if (!Display::setup())
	{
		Serial.println("FAILED TO SETUP DISPLAY");

		pinMode(13, OUTPUT);
		digitalWrite(13, HIGH);
		while (1);
	}

	Display::s_display.display();
	delay(1000);
	Display::s_display.setTextSize(1);
	Display::s_display.setTextColor(WHITE); 
	Display::s_display.cp437(true);	
}

void loop()
{
	preGame();
	while (!Game::ended) {
		gameLoop();
	}
}

