#ifndef SENSOR_DHT11_H
#define SENSOR_DHT11_H

#include <Arduino.h>

#define DIRECTION_REGISTER DDRD
#define DATA_REGISTER PORTD
#define PIN_NUMBER 2


#define SENSOR_OK 1
#define SENSOR_UPDATE_SKIPED 2
#define SENSOR_ERROR_TIMEOUT 3
#define SENSOR_ERROR_CHECKUM 4

#ifdef F_CPU
#define DHT11_TIMEOUT (F_CPU/40000)
#else
#define DHT11_TIMEOUT 10000
#endif

class dht11 {
	public:
		
		dht11();
		uint8_t getHumidity();
		uint8_t getTemperature();
		uint8_t update();
		float heatIndex();

	private:

		unsigned long timer;
		uint8_t humidity;
		uint8_t temperature;
		uint8_t pin;

};

#endif // SENSOR_DHT11_H