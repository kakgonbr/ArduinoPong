#ifndef SENSOR_HEADER
#define SENSOR_HEADER
#include <stdint.h>
#include "Arduino.h"

class Sensor {
    private:    uint8_t const m_trigPin;
    private:    uint8_t const m_echoPin;

    public:     explicit Sensor(uint8_t const t_trigPin, uint8_t const t_echoPin);
    public:     virtual ~Sensor();
    
    public:     uint32_t getDistance() const;
};

#endif