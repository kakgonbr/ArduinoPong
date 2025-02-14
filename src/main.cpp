#include <Arduino.h>
#include "display.hpp"
#include "sensor.hpp"
#include "config.hpp"
#include "pair.hpp"
#include "game.hpp"

// Global variable? All day everyday
Sensor leftSensor(11, 10);
Sensor rightSensor(13, 12);
Pair<uint16_t, uint16_t> leftPaddle;
Pair<uint16_t, uint16_t> rightPaddle;

uint32_t leftDistance;
uint32_t rightDistance;

void setup()
{
	Serial.begin(9600);

	if (!Display::setup())
	{
		Serial.println("UNABLE TO SETUP DISPLAY");
		while (1)
			;
	}
	Display::s_display.display();
	delay(2000);

	// Game::setPaddles(&leftPaddle, &rightPaddle);

	Game::throwBall(static_cast<uint8_t>(random(4)));
}

void loop()
{
	Game::tick(&leftPaddle, &rightPaddle);
	// Serial.print("Left: ");
	// Serial.println(leftSensor.getDistance());
	// Serial.print("Right: ");
	// Serial.println(rightSensor.getDistance());
	
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
	leftPaddle = {Config::LEFT_PADDLE_X, static_cast<uint16_t>((static_cast<float>(leftDistance - Config::SAFE_OFFSET) / Config::DISTANCE_TOTAL) * (Display::HEIGHT - Config::PADDLE_PADDING_UPPER - Config::PADDLE_PADDING_LOWER - Config::PADDLE_HEIGHT) + Config::PADDLE_PADDING_UPPER)};

	rightPaddle = {Config::RIGHT_PADDLE_X, static_cast<uint16_t>((static_cast<float>(rightDistance - Config::SAFE_OFFSET) / Config::DISTANCE_TOTAL) * (Display::HEIGHT - Config::PADDLE_PADDING_UPPER - Config::PADDLE_PADDING_LOWER - Config::PADDLE_HEIGHT) + Config::PADDLE_PADDING_UPPER)};

	// ball
	Display::drawPixel(Game::getBall().left, Game::getBall().right);

	Display::drawPaddle(leftPaddle.left, leftPaddle.right);
	Display::drawPaddle(rightPaddle.left, rightPaddle.right);
	
	Display::showUnsigned(10, 0, Game::getScore().left);
	Display::showUnsigned(120, 0, Game::getScore().right);

	// Display::drawRect(0, Config::PADDLE_PADDING_UPPER, Display::WIDTH + 1, Display::HEIGHT - Config::PADDLE_PADDING_UPPER - Config::PADDLE_PADDING_LOWER + 1);

	Display::drawHorizontalLine(Config::PADDLE_PADDING_UPPER);
	Display::drawHorizontalLine(Display::HEIGHT - Config::PADDLE_PADDING_LOWER + 1);

	Display::s_display.display();
	// delay(20);
}
