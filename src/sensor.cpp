#include "sensor.hpp"

Sensor::Sensor(uint8_t const t_trigPin, uint8_t const t_echoPin) : m_trigPin(t_trigPin),
                                                                    m_echoPin(t_echoPin)
{
    pinMode(m_echoPin, INPUT);
    pinMode(m_trigPin, OUTPUT);

    digitalWrite(m_trigPin, LOW);
}

    Sensor::~Sensor()
{
}

uint16_t Sensor::getDistance() const
{
    digitalWrite(m_trigPin, LOW);
    delay(2);
    
    digitalWrite(m_trigPin, HIGH);
    delay(10);
    digitalWrite(m_trigPin, LOW);

    return pulseIn(m_echoPin, HIGH) * 0.034 / 2;
}
