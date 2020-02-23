
#include "CK_I2C.h"

#include "fsl_iomuxc.h"
#include "fsl_clock.h"

void CK_I2C_Init(LPI2C_Type* I2Cx){

	if(I2Cx == LPI2C1){

		// I2C1 Configuration

		// GPIO_01 is configured as LPI2C1_SDA
		//Software Input On Field: Force input path of pad GPIO_01
		IOMUXC_SetPinMux(IOMUXC_GPIO_01_LPI2C1_SDA, 1U);

		// GPIO_02 is configured as LPI2C1_SCL
		// Software Input On Field: Force input path of pad GPIO_02
		IOMUXC_SetPinMux(IOMUXC_GPIO_02_LPI2C1_SCL, 1U);

		// GPIO_01 PAD functional properties :
		// Slew Rate Field: Slow Slew Rate
		// Drive Strength Field: R0/4
		// Speed Field: fast(150MHz)
		// Open Drain Enable Field: Open Drain Enabled
		// Pull / Keep Enable Field: Pull/Keeper Enabled
		// Pull / Keep Select Field: Keeper
		// Pull Up / Down Config. Field: 22K Ohm Pull Up
		// Hyst. Enable Field: Hysteresis Disabled */
		IOMUXC_SetPinConfig(IOMUXC_GPIO_01_LPI2C1_SDA, 0xD8A0U);

		// GPIO_02 PAD functional properties :
		// Slew Rate Field: Slow Slew Rate
		// Drive Strength Field: R0/4
		// Speed Field: fast(150MHz)
		// Open Drain Enable Field: Open Drain Enabled
		// Pull / Keep Enable Field: Pull/Keeper Enabled
		// Pull / Keep Select Field: Keeper
		// Pull Up / Down Config. Field: 22K Ohm Pull Up
		// Hyst. Enable Field: Hysteresis Disabled */
		IOMUXC_SetPinConfig(IOMUXC_GPIO_02_LPI2C1_SCL, 0xD8A0U);

		// Clock setting for LPI2C
		// Select USB1 PLL (480 MHz) as master lpi2c clock source
		CLOCK_SetMux(kCLOCK_Lpi2cMux, 0U);

		uint32_t LPI2C_CLOCK_SOURCE_DIVIDER = 5U;

		// Clock divider for master lpi2c clock source
		CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);

		// masterConfig.debugEnable = false;
		// masterConfig.ignoreAck = false;
		// masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
		// masterConfig.baudRate_Hz = 100000U;
		// masterConfig.busIdleTimeout_ns = 0;
		// masterConfig.pinLowTimeout_ns = 0;
		// masterConfig.sdaGlitchFilterWidth_ns = 0;
		// masterConfig.sclGlitchFilterWidth_ns = 0;

		lpi2c_master_config_t masterConfig;

		LPI2C_MasterGetDefaultConfig(&masterConfig);

		// Change default baudrate from 100KHz to 400KHz
		masterConfig.baudRate_Hz = 400000;

		uint32_t LPI2C_CLOCK_FREQUENCY = ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U));

		// Initialize the LPI2C master peripheral
		LPI2C_MasterInit(LPI2C1, &masterConfig, LPI2C_CLOCK_FREQUENCY);


	}


}

status_t CK_I2C_Write(LPI2C_Type* I2Cx, uint8_t slaveAddress, uint8_t reg, uint8_t data){

	status_t reVal;

	// Send master blocking data to slave
	if(kStatus_Success == LPI2C_MasterStart(LPI2C1, slaveAddress, kLPI2C_Write)){

		reVal = LPI2C_MasterSend(LPI2C1, &reg, 1);

		reVal = LPI2C_MasterSend(LPI2C1, &data, 1);

		reVal = LPI2C_MasterStop(LPI2C1);

	}

	return reVal;

}

status_t CK_I2C_WriteMulti(LPI2C_Type* I2Cx, uint8_t slaveAddress, uint8_t reg, uint8_t* writeBuffer, uint16_t writeSize){

	status_t reVal;

	// Send master blocking data to slave
	if(kStatus_Success == LPI2C_MasterStart(LPI2C1, slaveAddress, kLPI2C_Write)){

		reVal = LPI2C_MasterSend(LPI2C1, &reg, 1);
		if (reVal != kStatus_Success){
			return -1;
		}

		reVal = LPI2C_MasterSend(LPI2C1, writeBuffer, writeSize);
		if (reVal != kStatus_Success){
			return -1;
		}

		reVal = LPI2C_MasterStop(LPI2C1);
		if (reVal != kStatus_Success){
			return -1;
		}

	}

	return reVal;

}

status_t CK_I2C_Read(LPI2C_Type* I2Cx, uint8_t slaveAddress, uint8_t reg, uint8_t* readBuffer, uint16_t readSize){

	status_t reVal;

	// Send master blocking data to slave
	if(kStatus_Success == LPI2C_MasterStart(LPI2C1, slaveAddress, kLPI2C_Write)){

		reVal = LPI2C_MasterSend(LPI2C1, &reg, 1);
		if (reVal != kStatus_Success){
			return -1;
		}

		reVal = LPI2C_MasterRepeatedStart(LPI2C1, slaveAddress, kLPI2C_Read);
		if (reVal != kStatus_Success){
			return -1;
		}

		reVal = LPI2C_MasterReceive(LPI2C1, readBuffer, readSize);
		if (reVal != kStatus_Success){
			return -1;
		}

		reVal = LPI2C_MasterStop(LPI2C1);
		if (reVal != kStatus_Success){
			return -1;
		}

	}

	return reVal;

}

