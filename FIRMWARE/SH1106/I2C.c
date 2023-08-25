#include <xc.h>
#include "I2C.h"
#include "config.h"

//Initialize I2C in master mode, Sets the required baudrate
void I2CInit(void)
{
//   TRISBbits.TRISB1 = 1;    // SDA Data pin direction
//   TRISBbits.TRISB4 = 1;    // SCL Clock pin direction

//   SSP1ADD  =  ((32000000/4000)/100) - 1;  //((_XTAL_FREQ/4000)/I2C_SPEED) - 1
//
//   SSP1STAT = 0x80;   /* Slew rate disabled */
//   SSP1CON1 = 0x28;   /* SSPEN = 1, I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
//   while ( ( SSP1CON2 & 0x1F ) || ( SSP1STATbits.R_nW ) );
//   SSP1STAT = 0x00;  //Resetting SSPSTAT register
   
        SSP1STAT = 0x80;
        SSP1CON1 = 0x08;
        SSP1CON2 = 0x00;
        //SSP1ADD = 0x4f;//0x4F;
        SSP1ADD  =  ((_XTAL_FREQ/4000)/I2C_SPEED) - 1;
        SSP1CON1bits.SSPEN = 1;
        
        SSP1STAT = 0x00;

}
//Send a start condition on I2C Bus
void StartI2C(void)
{
  SSP1CON2bits.SEN = 1;            // initiate bus start condition 
  while(SSP1CON2bits.SEN);        /* wait for start condition to finish */
}

//Send a stop condition on I2C Bus
void StopI2C(void)
{
  SSP1CON2bits.PEN = 1;            // initiate bus stop condition
  while(SSP1CON2bits.PEN);  
}

//Sends a repeated start condition on I2C Bus
void I2CRestart(void)
{
  SSP1CON2bits.RSEN = 1;           // initiate bus restart condition
  while(SSP1CON2bits.RSEN);     /* wait for condition to finish */
}

//Generates acknowledge for a transfer
void I2CNck(void)
{
   SSP1CON2bits.ACKDT = 0;     // 0 means ACK
   SSP1CON2bits.ACKEN = 1;     // Send ACKDT value
   while(SSP1CON2bits.ACKEN);    /* wait for ack data to send on bus */
}

//Generates Not-acknowledge for a transfer
void I2CAck(void)
{
   SSP1CON2bits.ACKDT = 1;     // 0 means ACK
   SSP1CON2bits.ACKEN = 1;     // Send ACKDT value
   while(SSP1CON2bits.ACKEN);    /* wait for ack data to send on bus */
}

//wait for transfer to finish
void I2CWait(void)
{
   while ((SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) );
   /* wait for any pending transfer */
}
void IdleI2C( void )
{
  while ( ( SSP1CON2 & 0x1F ) || ( SSP1STATbits.R_nW ) )
     continue;
}
 
//Send 8-bit data on I2C bus - 8-bit data to be sent on bus data can be either address/data byte
signed char WriteI2C(unsigned char dat)
{
   SSP1BUF = dat;    /* Move data to SSPBUF */
     if (SSP1CON1bits.WCOL)      // test if write collision occurred
   return ( -1 );              // if WCOL bit is set return negative #
   while(SSP1STATbits.BF);       /* wait till complete data is sent from buffer */
   IdleI2C();       /* wait for any pending transfer */
   if ( SSP1CON2bits.ACKSTAT ) // test for ACK condition received
    	 return ( -2 );			// return NACK
	else 

//   if ( PIR1bits.SSP1IF ) // test for ACK condition received
//   	 return ( -2 );
//   else 
	return ( 0 );              //return ACK
}

//Read 8-bit data from I2C bus
unsigned char ReadI2C(void)
{
/* Reception works if transfer is initiated in read mode */
   SSP1CON2bits.RCEN = 1;        /* Enable data reception */
  while ( !SSP1STATbits.BF );      // wait until byte received  
  return ( SSP1BUF );              // return with read byte 
}

