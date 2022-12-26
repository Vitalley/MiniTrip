#include <xc.h>
void I2CInit(void);
void StartI2C(void);                 // Send start bit on i2c
void I2CRestart(void);
signed char WriteI2C(unsigned char dat);              // Read from 0xC
unsigned char ReadI2C(void);
void I2CAck(void);
void StopI2C(void);    // Send stop bit on i2c
void IdleI2C( void );