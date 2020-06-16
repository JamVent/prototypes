/*
 * Project photonjamsim
 * Description:
 * Author:
 * Date:
 */

#include "I2cIntHandler.h"
#include "I2cSlaveCtl.h"
#include "I2cJamsimConfig.h"
#include "I2cPresSensor.h"
#include "I2cPsysSensor.h"
#include "I2cAdcSensor.h"
#include "JamventTime.h"
#include "JamsimDbgPrint.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);


// gpio defs  (these will change with our proto board.)
#define I2C_RPI_SCL   A1
#define I2C_RPI_SDA   A4
#define I2C_CPLD_SCL  D3
#define I2C_CPLD_SDA  D2

#define GPIO_VALVEA D0
#define GPIO_VALVEB D1
#define GPIO_VALVEC D2
#define GPIO_VALVED D3

// stay away from these addresses
#define I2C_PRES_ADDR 0x76  // check these against the specs...
#define I2C_PSYS_ADDR 0x77

// our simulation addresses...
#define I2C_PHOTON_CFG_ADDR 0x60
#define I2C_PRES_SIM_ADDR   0x61
#define I2C_PSYS_SIM_ADDR   0x62
#define I2C_ADC_SIM_ADDR    0x63




static I2cIntHandler *i2cIntHandler = nullptr;
static I2cSlaveCtl *i2cRpiSlaveCtl = nullptr;
static I2cSlaveCtl *i2cCpldSlaveCtl = nullptr;
static I2cJamsimConfig *photonConfig;
static double timeToWake = 0;
static double timeToPrint = 0;
static JamventTime jamTime;
static JamsimDbgPrint dbgPrint;


int counter = 0;

void initialize() {
	static bool isInitialized = false;
	if (isInitialized)
		return;

	i2cIntHandler = new I2cIntHandler();
	i2cIntHandler->setDbgPrint(&dbgPrint);
	i2cRpiSlaveCtl = new I2cSlaveCtl(I2C_RPI_SCL, I2C_RPI_SDA);
	i2cRpiSlaveCtl->setDbgPrint(&dbgPrint);
	photonConfig = new I2cJamsimConfig(I2C_PHOTON_CFG_ADDR);
	// TBD create three more sensors


	i2cCpldSlaveCtl = new I2cSlaveCtl(I2C_CPLD_SCL, I2C_CPLD_SDA);
	i2cCpldSlaveCtl->setDbgPrint(&dbgPrint);
	// TBD create three cpld sensors...

	// wire it all up.
	i2cIntHandler->registerI2cSlaveCtl(i2cRpiSlaveCtl);
	#if 0
	i2cIntHandler->registerI2cSlaveCtl(i2cCpldSlaveCtl);
	#endif
	i2cRpiSlaveCtl->registerI2cDevice(photonConfig);
	i2cIntHandler->attachAllInterrupts();
	isInitialized=true;

}

void setup() {
	initialize();
}	


void loop() {

	if (i2cIntHandler == nullptr)
		return;

	i2cIntHandler->microsNow();			// keep this refreshed so we have an accurate micros count...
	double now = jamTime.now();
	//i2cSlaveCtl->sampleIO();
	if (now >= timeToWake) {
		// tbd... call the jamsim simulation.
	}
	if (now >= timeToPrint) {
		if (dbgPrint.hasData()) {
			Serial.print(dbgPrint.getPrintBuffer());
			dbgPrint.clear();
		}
		timeToPrint += 1.0;
	}
	#if 0
	static unsigned cnt = 0;
	cnt++;
	//Serial.print("setting A0 high\n");
	digitalWrite(D7, (cnt&1));
	delay(1000);
	//digitalWrite(A0,HIGH);
	#endif
}
