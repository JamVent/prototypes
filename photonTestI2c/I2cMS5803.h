#ifndef __I2CMS5803_H__
#define __I2CMS5803_H__

class I2cMS5803 : public I2cMSxxx
{
public:
	I2cMS5803() = delete;
	I2cMS5803(unsigned bus, unsigned address)
		: I2cMSxxx(bus, address) {
		memset(_prom, 0, sizeof(_prom));
	}

	/**
	 * @brief convert the temperature value.
	 * 
	 * @param rawValue 
	 * @return uint32_t -- returns the temperature value in degrees C
	 */
	uint32_t  convertTemperature(uint32_t rawValue) {
		uint32_t C1 = _prom[1];
		uint32_t C2 = _prom[2];
		uint32_t C3 = _prom[3];
		uint32_t C4 = _prom[4];
		uint32_t C5 = _prom[5];
		uint32_t C6 = _prom[6];
		uint32_t C7 = _prom[7];
	    int32_t temp_calc;
	    int32_t dT;

		dT = rawValue - ((int32_t)C5 << 8);
		temp_calc = (((int64_t)dT * C6) >> 23) + 2000;

		//Now we have our first order Temperature, let's calculate the second order.
        int64_t T2, OFF2, SENS2;

		if (temp_calc < 2000) {    // If temp_calc is below 20.0C
			T2 = 3 * (((int64_t)dT * dT) >> 33);
			OFF2 = 3 * ((temp_calc - 2000) * (temp_calc - 2000)) / 2;
			SENS2 = 5 * ((temp_calc - 2000) * (temp_calc - 2000)) / 8;

			if(temp_calc < -1500) {       // If temp_calc is below -15.0C
				OFF2  = OFF2 + 7 * ((temp_calc + 1500) * (temp_calc + 1500));
				SENS2 = SENS2 + 4 * ((temp_calc + 1500) * (temp_calc + 1500));
			}
		}
		else {      // If temp_calc is above 20.0C
			//T2 = 7 * ((uint64_t)dT * dT)/pow(2,37);
			T2 = 7 * ((uint64_t)dT * dT) >> 37;
			OFF2 = ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
			SENS2 = 0;
		}
		// Now bring it all together to apply offsets

		temp_calc = temp_calc - T2;
		_OFF  = ((int64_t)C2 << 16) + (((C4 * (int64_t)dT)) >> 7);
		_SENS = ((int64_t)C1 << 15) + (((C3 * (int64_t)dT)) >> 8);
		_OFF  = _OFF - OFF2;
		_SENS = _SENS - SENS2;

		_temperature = temp_calc;
		return (_temperature);
	}

	/**
	 * @brief convert the pressure value.
	 * 
	 * @param rawValue -- raw pressure value to convert.
	 * @return uint32_t -- converted pressure value 
	 */
	uint32_t convertPressure(uint32_t rawValue) {
		_pressure = (((_SENS * rawValue) / 2097152 ) - _OFF) / 32768;
		return (_pressure);
	}


protected:
	/**
	 * @brief Get the Precision value
	 * 
	 * @param precision -- precision to look up the delay for
	 * @return unsigned -- delay in us...
	 */
	virtual  int getPrecisionDelay(unsigned precision) {
		int delay;
		switch (precision) 		{
			case ADC_256 : delay =   600; break;
			case ADC_512 : delay =  1170; break;
			case ADC_1024: delay =  2280; break;
			case ADC_2048: delay =  4540; break;
			case ADC_4096: delay =  9040; break;
			default: delay = -1;		// unknown precision...
		}
		return (delay);
	}
private:


};

#endif
