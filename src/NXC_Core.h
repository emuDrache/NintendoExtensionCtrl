/*
*  Project     Nintendo Extension Controller Library
*  @author     David Madison
*  @link       github.com/dmadison/NintendoExtensionCtrl
*  @license    LGPLv3 - Copyright (c) 2018 David Madison
*
*  This file is part of the Nintendo Extension Controller Library.
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NXC_Core_h
#define NXC_Core_h

#include "Arduino.h"

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) || \
    defined(__MK64FX512__) || defined(__MK66FX1M0__) // Teensy 3.0/3.1-3.2/LC/3.5/3.6
#include <i2c_t3.h>
#define NXC_I2C_TYPE i2c_t3
#else
#include <Wire.h>  // All other platforms
#define NXC_I2C_TYPE TwoWire
#endif

#define NXC_I2C_DEFAULT Wire
#define NXC_SERIAL_DEFAULT Serial

enum NXC_ControllerType {
	NXC_NoController,
	NXC_UnknownController,
	NXC_Nunchuk,
	NXC_ClassicController,
	NXC_GuitarController,
	NXC_DrumController,
	NXC_DJTurntable,
};

namespace NintendoExtensionCtrl {

	class ExtensionComms {
	public:
		ExtensionComms(NXC_I2C_TYPE& i2cRef) : i2c(i2cRef) {}

		void startBus();

		boolean writePointer(byte ptr);
		boolean writeRegister(byte reg, byte value);
		boolean requestMultiple(uint8_t requestSize, uint8_t * dataOut);

		boolean readDataArray(byte ptr, uint8_t requestSize, uint8_t * dataOut);
	private:
		static const uint8_t I2C_Addr = 0x52;  // Address for all controllers
		static const long ConversionDelay = 175;  // Microseconds, ~200 on AVR

		NXC_I2C_TYPE & i2c;
	};

	// Identity
	const uint8_t IDHeaderSize = 6;
	boolean requestIdentity(NXC_I2C_TYPE& i2c, uint8_t * idData);
	NXC_ControllerType identifyController(const uint8_t * idData);
	NXC_ControllerType identifyController(NXC_I2C_TYPE& i2c = NXC_I2C_DEFAULT);

	// Utility
	boolean verifyData(const uint8_t * dataIn, uint8_t dataSize);
	void printRaw(const uint8_t * dataIn, uint8_t dataSize, uint8_t baseFormat = HEX, Stream& stream = NXC_SERIAL_DEFAULT);
	void printRaw(uint8_t dataIn, uint8_t baseFormat = HEX, Stream& stream = NXC_SERIAL_DEFAULT);
	void printRepeat(char c, uint8_t nPrint, Stream& stream = NXC_SERIAL_DEFAULT);

	class RolloverChange {
	public:
		RolloverChange(uint8_t min, uint8_t max);
		int8_t getChange(uint8_t valIn);
	private:
		int8_t rolloverOut(uint8_t c1, uint8_t c2) const;
		uint8_t halfRange() const;

		const uint8_t minValue;
		const uint8_t maxValue;

		uint8_t lastValue = 0;
	};
}

namespace NXCtrl = NintendoExtensionCtrl;  // Alias for shorter access

#endif
