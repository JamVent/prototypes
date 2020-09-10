/**
 * @brief Stand-alone executable to test I2C comms on Pi to sensors
 */

#include <string.h>
#include <pigpio.h> // Library used for I2C on Pi
#include <iostream>
#include <vector>

#include "I2cMS5607.h"
#include "I2cMS5803.h"
#include "I2cAdcWithO2.h"
#include "Valves.h"
#include "Speaker.h"

using namespace std;

/* I2C addresses of sensors */
#define PRES_I2C_DEV    0x76
#define PSYS_I2C_DEV    0x77
#define ADC_I2C_DEV		0x1D

int testPresDevice() {
	int rc; 	// return codes
	int delay; 	// time we need to wait for pressure/temp reading
	
	// test MS5803 / PRES sensor
	cout << endl << "=== PRES / MS5803 test ===" << endl;
	
	I2cMS5803 presSensor(1, PRES_I2C_DEV);
	rc = presSensor.open();
	if (rc != 0) {
		cout << "presSensor.open failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = presSensor.reset();
	if (rc != 0) {
		cout << "presSensor.reset() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = presSensor.readProm();
	if (rc != 0) {
		cout << "presSensor.readProm() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}

	std::vector<uint16_t> promData;
	presSensor.getPromData(promData);
	cout << "promData = " << endl;
	for (auto d : promData) 
		cout << "  " << d << endl;

	delay = presSensor.startTemperatureCv(I2cMSxxx::ADC_4096);
	if (delay < 0) {
		cout << "presSensor.startTemperatureCv() failed, rc=" << delay << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "presSensor.startTemperatureCv() = " << delay << endl;
	
	rc = presSensor.gpioDelay(delay * 1.5); 	// 50% margin just to be sure
	cout << "waited " << rc << endl;
	
	rc = presSensor.readAdc();
	if (rc != 0) {
		cout << "presSensor.readAdc() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	cout << "temperature is " << presSensor.getLastTemperature() << " C" << endl;
	
	delay = presSensor.startPressureCv(I2cMSxxx::ADC_4096);
	if (delay < 0) {
		cout << "presSensor.startPressureCv() failed, rc=" << delay << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "presSensor.startPressureCv() = " << delay << endl;
	
	rc = presSensor.gpioDelay(delay * 1.5); 	// 50% margin just to be sure
	cout << "waited " << rc << endl;
	
	rc = presSensor.readAdc();
	if (rc != 0) {
		cout << "presSensor.readAdc() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	cout << "pressure is " << presSensor.getLastPressure() << " bar" << endl;
	
	return (0);
}

int testPsysDevice() {
	int rc;	// return codes
	int delay;	// time we need to wait for pressure/temp reading
	
	// test MS5607 / PSYS sensor
	cout << endl << "=== PSYS / MS5607 test ===" << endl;
	
	I2cMS5607 psysSensor(1, PSYS_I2C_DEV);
	rc = psysSensor.open();
	if (rc != 0) {
		cout << "psysSensor.open failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = psysSensor.reset();
	if (rc != 0) {
		cout << "psysSensor.reset() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = psysSensor.readProm();
	if (rc != 0) {
		cout << "psysSensor.readProm() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}

	std::vector<uint16_t> promData;
	psysSensor.getPromData(promData);
	cout << "promData = " << endl;
	for (auto d : promData) 
		cout << "  " << d << endl;

	delay = psysSensor.startTemperatureCv(I2cMSxxx::ADC_4096);
	if (delay < 0) {
		cout << "psysSensor.startTemperatureCv() failed, rc=" << delay << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "psysSensor.startTemperatureCv() = " << delay << endl;
	
	rc = psysSensor.gpioDelay(delay * 1.5);	// 50% margin just to be sure
	cout << "waited " << rc << endl;
	
	rc = psysSensor.readAdc();
	if (rc != 0) {
		cout << "psysSensor.readAdc() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	cout << "temperature is " << psysSensor.getLastTemperature() << " C" << endl;
	
	delay = psysSensor.startPressureCv(I2cMSxxx::ADC_4096);
	if (delay < 0) {
		cout << "psysSensor.startPressureCv() failed, rc=" << delay << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "psysSensor.startPressureCv() = " << delay << endl;
	
	rc = psysSensor.gpioDelay(delay * 1.5);	// 50% margin just to be sure
	cout << "waited " << rc << endl;
	
	rc = psysSensor.readAdc();
	if (rc != 0) {
		cout << "psysSensor.readAdc() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	cout << "pressure is " << psysSensor.getLastPressure() << " bar" << endl;
	
	return (0);
}

int testO2(void) {
	int rc;
	float o2;
	int16_t temperature;
	
	cout << endl << "=== O2 / ADC128D818 test ===" << endl;
	
	I2cAdcWithO2 adc(1, ADC_I2C_DEV, 2.56, 3E-3, 7E-3, 101);
	rc = adc.open();
	if (rc != 0) {
		cout << "adc.open() failed, rc=" << rc << "," << adc.getErrorText(rc) << endl;
		return 1;
	}
	
	bool enableChannels[8] = { false, false, true, false, false, false, false, false };
	// have only ADC channel 2 enabled, use internal 2.56V reference, and enable the internal temp sensor instead of ch.7
	rc = adc.init(false, enableChannels, true);
	if (rc < 0) {
		cout << "adc.init() failed, rc=" << rc << "," << adc.getErrorText(rc) << endl;
		return (1);
	}
	
	// give enough time (plus 50% overhead) to get a full set of readings
	adc.gpioDelay(rc * 1.5);
	
	// read both the 'interpreted' O2 level from the sensor, and the raw voltage at the ADC pin
	o2 = adc.readO2(2);
	if (o2 < 0) {
		cout << "adc.readO2() failed, rc=" << o2 << "," << adc.getErrorText((int)o2) << endl;
		return 1;
	}
	cout << "O2 level is " << o2 << "%" << endl;
	
	o2 = adc.readVoltage(2);
	if (o2 < 0) {
		cout << "adc.readVoltage() failed, rc=" << o2 << "," << adc.getErrorText((int)o2) << endl;
		return 1;
	}
	cout << "O2 channel voltage is " << o2 << endl;
	
	// read off the internal temperature sensor as well
	rc = adc.readTemp(&temperature);
	if (rc != 0) {
		cout << "adc.readTemp() failed, rc=" << rc << "," << adc.getErrorText(rc) << endl;
		return 1;
	}
	cout << "temperature is " << (temperature / 2) << " C" << endl;
	
	return 0;
}

int testValves(void) {
	int rc;
	
	cout << endl << "=== Valve test ===" << endl;
	
	// initialize
	rc = Valves::init();
	if (rc != 0) {
		cout << "Valves::init() failed, rc=" << rc << endl; //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	
	// turn on valve A for one second
	cout << "Valve A open" << endl;
	rc = Valves::setState(Valves::VALVE_A_O2, true);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl; //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	gpioDelay(1E6);
	cout << "Valve A closed" << endl;
	rc = Valves::setState(Valves::VALVE_A_O2, false);
	if (rc != 0) {
		cout << "Valves::setstate failed, rc=" << rc << endl;  //"," << Valves::geterrortext(rc) << endl;
		return 1;
	}
	
	// turn on valve B for one second
	cout << "Valve B open" << endl;
	rc = Valves::setState(Valves::VALVE_B_AIR, true);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl;  //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	gpioDelay(1E6);
	cout << "Valve B closed" << endl;
	rc = Valves::setState(Valves::VALVE_B_AIR, false);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl;  //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	
	// turn on valve C for one second
	cout << "Valve C open" << endl;
	rc = Valves::setState(Valves::VALVE_C_INHALE, true);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl;  //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	gpioDelay(1E6);
	cout << "Valve C closed" << endl;
	rc = Valves::setState(Valves::VALVE_C_INHALE, false);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl;  //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	
	// turn on valve B for one second
	cout << "Valve D open" << endl;
	rc = Valves::setState(Valves::VALVE_D_EXHALE, true);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl;  //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	gpioDelay(1E6);
	cout << "Valve D closed" << endl;
	rc = Valves::setState(Valves::VALVE_D_EXHALE, false);
	if (rc != 0) {
		cout << "Valves::setState failed, rc=" << rc << endl;  //"," << Valves::getErrorText(rc) << endl;
		return 1;
	}
	
	return 0;
}

int testSpeaker(void) {
	int rc;
	
	cout << endl << "=== Speaker test ===" << endl;
	
	// initialize
	rc = Speaker::init();
	if (rc != 0) {
		cout << "speaker::init() failed, rc=" << rc << endl;  //"," << speaker::getErrorText(rc) << endl;
		return 1;
	}
	
	// play the IEC 60601-1-8 general med-priority C4-C4-C4 alarm
	cout << "C4-C4-C4" << endl;
	for(int i = 0 ; i < 3 ; i++) {
		Speaker::playNote(262);
		gpioDelay(500E3);
		Speaker::stopNote();
		gpioDelay(500E3);
	}
	
	// play the IEC 60601-1-8 "ventilate" med-priority C4-A4-F4 alarm
	cout << "C4-A4-F4" << endl;
	Speaker::playNote(262);
	gpioDelay(1E6);
	Speaker::playNote(440);
	gpioDelay(1E6);
	Speaker::playNote(349);
	gpioDelay(1E6);
	Speaker::stopNote();
	
	return 0;
}

int main(int argc, char *argv[])
{
	int rc;
	
	cout << "initializing gpio" << endl;
	rc = gpioInitialise();
	if (rc == PI_INIT_FAILED) {
		cout << "gpioInitialise Failed" << endl;
		return (1);
	}
	cout << "pigpio version " << rc << endl;

	testSpeaker();
	testValves();
	testPsysDevice();
	testPresDevice();
	testO2();
	
	gpioTerminate();
	
	return 0;
}