#include <dht11.hpp>


dht11::dht11() {
	
	//this->pin = pin;
	this->humidity = 0;
	this->temperature = 0;
	this->timer = 0;

}

uint8_t dht11::getHumidity() {
	return this->humidity;
}

uint8_t dht11::getTemperature() {
	return this->temperature;
}

uint8_t dht11::update() {

	if ( millis() - this->timer < 1000 ) {
		return SENSOR_UPDATE_SKIPED;
	}

	unsigned long counter = DHT11_TIMEOUT * 2;

	uint8_t bytes[5] = {0};

	// Nastaví pin na output
	//pinMode( this->pin, OUTPUT );
	DIRECTION_REGISTER |= 1 << PIN_NUMBER;

	// Nastaví pin na LOW
	// digitalWrite( this->pin, LOW );
	DATA_REGISTER &= ~(1 << PIN_NUMBER);
	
	// Nějaká alternativa?
	delay( 18 );

	// Nastaví pin a HIGH
	// digitalWrite( this->pin, HIGH );
	DATA_REGISTER |= 1 << PIN_NUMBER;

	delayMicroseconds( 40 );

	// pinMode( this->pin, INPUT );
	DIRECTION_REGISTER &= ~(1 << PIN_NUMBER );

	// while( digitalRead( this->pin ) == LOW ) {
	while( DATA_REGISTER & ~PIN_MASK ) {
		if ( --counter == 0 ) {
			return 3;
		}
	}
	
	counter=DHT11_TIMEOUT;
	// while( digitalRead( this->pin) == HIGH ) {
	while( DATA_REGISTER & PIN_MASK ) {
		if ( --counter == 0) {
			return 4;
		}
	}

	unsigned long start = 0;
	uint8_t bits_counter = 7;
	uint8_t bytes_counter = 0;

	for( uint8_t i = 0; i < 40; i++ ) {

		counter = DHT11_TIMEOUT;
		// while( digitalRead(this->pin) == LOW ) {
		while (DATA_REGISTER & ~PIN_MASK) {
			if ( --counter == 0 ) {
				return 5;
			}
		}

		counter = DHT11_TIMEOUT;
		start = micros();
		// while( digitalRead(this->pin) == HIGH ) {
		while( DATA_REGISTER & PIN_MASK ) {
			if ( --counter == 0 ) {
				return 6;
			}
		}

		if ( micros() - start > 40 ) {
			bytes[ bytes_counter ] |= (1 << bits_counter);
		}

		if ( bits_counter == 0) {
			bits_counter = 7;
			bytes_counter++;
		} else {
			bits_counter--;
		}
		
	}

	if ( bytes[0] + bytes[2] == bytes[4] ) {
		this->humidity = bytes[0];
		this->temperature = bytes[2];
	} else {
		this->humidity = 0;
		this->temperature = 0;
		return SENSOR_ERROR_CHECKUM;
	}

	// pinMode( this->pin, OUTPUT );
	DIRECTION_REGISTER |= PIN_MASK;
	
	// digitalWrite( this->pin, HIGH );
	DATA_REGISTER |= PIN_MASK;

	this->timer = millis();
	return SENSOR_OK;
}

/**
 * Vypočítá hodnotu pocitové teploty na základě naměřené teploty a vlhkosti vzduchu.
 * \return Pocitová teplota ve °C.
 * \see http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
*/
float dht11::heatIndex() {
	// Vzorec počítá s teplotou ve °F.
	float fahrenheit = 9 * this->temperature / 5 + 32;
	
	// Výsledná pocitová teplota
	float hi = 0.0;

	// Pro teploty nižší než 80°F se používá jednodušší vzorec.
	if ( fahrenheit < 80 ) {
		hi = 0.5 * (fahrenheit + 61.0 + ((fahrenheit-68.0)*1.2) + (this->humidity*0.094)); 
	} else {
		hi = 	- 42.379 
				+ 2.04901523 	* fahrenheit
				+ 10.14333127 	* this->humidity 
				- 0.22475541	* fahrenheit * this->humidity
				- 0.00683783	* fahrenheit * fahrenheit
				- 0.05481717	* this->humidity   * this->humidity 
				+ 0.00122874	* fahrenheit * fahrenheit  * this->humidity 
				+ 0.00085282	* fahrenheit * this->humidity    * this->humidity 
				- 0.00000199	* fahrenheit * fahrenheit  * this->humidity * this->humidity;

		// Pro extrémní úrovně vlhkosti a teploty je třeba přičíst korekci.
		// Je otázka, jestli tato situace může vůbec někdy nastat, protože
		// dle datasheetu k čidlu je nejnižší měřitelná relativní vlhkost
		// kolem 20%. Nicméně pro úplnost část uvádím.
		if ( this->humidity < 13 && fahrenheit > 80 && fahrenheit < 112 ) {
			hi += ((this->humidity-85)/10) * ((87-fahrenheit)/5);
		}
	}

	// Vrací se teplota opět ve stupních celsia
	return 5*(hi-32)/9;
}