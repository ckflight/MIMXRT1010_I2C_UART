
#ifndef CK_I2C_H_
#define CK_I2C_H_

#include "fsl_lpi2c.h"

void CK_I2C_Init(LPI2C_Type* I2Cx);

status_t CK_I2C_Read(LPI2C_Type* I2Cx, uint8_t slaveAddress, uint8_t reg, uint8_t* readBuffer, uint16_t readSize);

status_t CK_I2C_Write(LPI2C_Type* I2Cx, uint8_t slaveAddress, uint8_t reg, uint8_t data);

status_t CK_I2C_WriteMulti(LPI2C_Type* I2Cx, uint8_t slaveAddress, uint8_t reg, uint8_t* writeBuffer, uint16_t writeSize);


#endif /* CK_I2C_H_ */
