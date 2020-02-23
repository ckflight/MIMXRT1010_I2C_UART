
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1011.h"
#include "fsl_debug_console.h"

#include "CK_I2C.h"
#include "CK_TIME.h"

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    CK_TIME_Init();

    PRINTF("I2C_L3GD20H_Init\n");

	CK_I2C_Init(LPI2C1);

	uint8_t device_id = 0;
	CK_I2C_Read(LPI2C1, 0x6B, 0x0F, &device_id, 1);
	if(device_id == 0xD7){

		PRINTF("Device Id: "); PRINTF("%x", device_id);

		CK_I2C_Write(LPI2C1, 0x6B, 0x20, 0xFF); // 800 Hz
		CK_I2C_Write(LPI2C1, 0x6B, 0x23, 0x20); // 2000 deg/sec, 70mdps/digit

	}

	uint8_t i2c_read[6];
	int16_t gyro_raw_x = 0;
	int16_t gyro_raw_y = 0;
	int16_t gyro_raw_z = 0;

	float gyro_x = 0.0f;
	float gyro_y = 0.0f;
	float gyro_z = 0.0f;

    while(1){

    	uint32_t time1 = CK_TIME_GetMicroSec();

    	CK_I2C_Read(LPI2C1, 0x6B, 0x28 | 0x80, i2c_read, 6);

    	gyro_raw_x = i2c_read[1] << 8 | i2c_read[0];
    	gyro_raw_y = i2c_read[3] << 8 | i2c_read[2];
    	gyro_raw_z = i2c_read[5] << 8 | i2c_read[4];

    	gyro_x = gyro_raw_x * 0.07f;
    	gyro_y = gyro_raw_y * 0.07f;
    	gyro_z = gyro_raw_z * 0.07f;


    	// Implement own print metods later. This takes 4ms
    	PRINTF("   Gx: ");   PRINTF("%.2f", gyro_x);
    	PRINTF("   Gy: ");   PRINTF("%.2f", gyro_y);
    	PRINTF("   Gz: ");   PRINTF("%.2f", gyro_z);
    	PRINTF("   Time: "); PRINTF("%d", time2);
    	PRINTF("\n");

    	uint32_t time2 = CK_TIME_GetMicroSec() - time1;


    	CK_TIME_DelayMicroSec(1250);


    }

}













