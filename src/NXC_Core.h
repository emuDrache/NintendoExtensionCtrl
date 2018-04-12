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
	const uint8_t I2C_Addr = 0x52;  // Address for all controllers
	const int I2C_ConversionDelay = 175;  // Microseconds, ~200 on AVR

	// I2C Low-Level Comms
	boolean writePointer(NXC_I2C_TYPE& i2c, byte ptr);
	boolean writeRegister(NXC_I2C_TYPE& i2c, byte reg, byte value);
	boolean requestMultiple(NXC_I2C_TYPE& i2c, uint8_t requestSize, uint8_t * dataOut);

	boolean readDataArray(NXC_I2C_TYPE& i2c, byte ptr, uint8_t requestSize, uint8_t * dataOut);

	// Controller Data
	boolean initialize(NXC_I2C_TYPE& i2c = NXC_I2C_DEFAULT);

	const uint8_t IDHeaderSize = 6;
	boolean requestIdentity(NXC_I2C_TYPE& i2c, uint8_t * idData);
	NXC_ControllerType identifyController(const uint8_t * idData);
	NXC_ControllerType identifyController(NXC_I2C_TYPE& i2c = NXC_I2C_DEFAULT);

	// Utility
	boolean verifyData(const uint8_t * dataIn, uint8_t dataSize);
	void printRaw(const uint8_t * dataIn, uint8_t dataSize, Stream& stream = Serial, uint8_t baseFormat = HEX);
}

namespace NXCtrl = NintendoExtensionCtrl;  // Alias for shorter access

#endif
