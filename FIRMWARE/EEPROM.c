#include <xc.h>
#include "../SH1106/OLED.h"
#include "EEPROM.h"
extern unsigned long int trip,triptime,impavr, totalodo, totaltime, TO1_ODO, TO1_TIME, TO2_ODO, TO2_TIME;
eeprom unsigned char EE_TRIP[5], EE_TRIPTIME[5], EE_TRIPAVR[5], EE_TOTALODO[5],EE_TOTALTIME[5], EE_TO1_ODO[5],
			EE_TO1_TIME[5], EE_TO2_ODO[5], EE_TO2_TIME[5];

/*
    unsigned char EEPROM_ReadByte(unsigned char eeprom_Address)
    {
        while(RD || WR);      // check the WR&RD bit to see if a RD/WR is in progress
      EEADR=eeprom_Address;   // Write the address to EEADR.
                              // Make sure that the address is not larger than the memory size 
        RD = 1;               // Set the WR bit to trigger the eeprom read operation.
        delay_us(50);
        return(EEDATA);       // Return the data read form eeprom.
     }

    void EEPROM_WriteNBytes(unsigned char EepromAddr, unsigned char *RamAddr, char NoOfBytes)
     {
      while(NoOfBytes !=  0)
       {
    	EEPROM_WriteByte(EepromAddr,*RamAddr); //Write a byte from RAM to EEPROM
    	    EepromAddr++;				   //Incerement the Eeprom Address
    		RamAddr++;				   //Increment the RAM Address
    		NoOfBytes--;			   //Decrement NoOfBytes after writing each Byte
    	   }
     }






    void EEPROM_ReadNBytes(unsigned char EepromAddr, unsigned char *RamAddr, char NoOfBytes)
     {
       while(NoOfBytes !=  0)
    	{
    	 *RamAddr = EEPROM_ReadByte(EepromAddr);//Read a byte from EEPROM to RAM
    	    EepromAddr++;				    //Incerement the Eeprom Address
    	 	RamAddr++;			          //Increment the RAM Address
    	 	NoOfBytes--;			    //Decrement NoOfBytes after Reading each Byte
     
    	 }
     }





void EEPROM_ReadString(unsigned char eeprom_address, unsigned char * destination_address)
  {
   char eeprom_data;
 
     do
 	 {
	  eeprom_data = EEPROM_ReadByte(eeprom_address); //Read a byte from EEPROM to RAM
 	    *destination_address = eeprom_data;	       //Copy the data into String Buffer
 	     destination_address++;				 //Incerement the RAM Address
 	 	 eeprom_address++;				 //Increment the Eeprom Address
 		}while(eeprom_data!=0);
  }
  
      void EEPROM_WriteString(unsigned char eeprom_address, unsigned char * source_address)
      {
        do
     	{
     	  EEPROM_WriteByte(eeprom_address,*source_address); //Write a byte from RAM to EEPROM
     	    source_address++;				          //Incerement the RAM Address
     		eeprom_address++;					    //Increment the Eeprom Address
     	  }while(*(source_address-1)!=0);
      }

void EEPROM_Erase()
   {
	 unsigned char eeprom_address;
 
	 for(eeprom_address=0;eeprom_address<255;eeprom_address++)
	    {
		 EEPROM_WriteByte(eeprom_address,0xff); // Write Each memory location with OxFF
	      }
   }
 */  
unsigned long int erd(unsigned char eeadr)
{
	unsigned long int rtt=0xFFFFFFFF;
	if (eeprom_read(eeadr+4) == 0x35)//Сигнатура целостности
	{
		for (char i=4;i>0;i--)
		{
			rtt=rtt<<8;
			rtt=rtt+eeprom_read(eeadr+i-1);		
		}
	}
	else
	{
		ewr(rtt,eeadr);
	}
	return ~rtt;
} 
void ewr(unsigned long int dt,unsigned char eeadr)
{
	dt=~dt;
	INTCONbits.GIE = 0;
	eeprom_write(eeadr+4,0xFF);//Сигнатура нарушения целостности
	INTCONbits.GIE = 1;
	for (char i=0;i<4;i++)
	{
		INTCONbits.GIE = 0;
		eeprom_write(eeadr+i,(dt & 0xFF));
		INTCONbits.GIE = 1;
		dt=dt>>8;
	}
	INTCONbits.GIE = 0;
	eeprom_write(eeadr+4,0x35);//Сигнатура целостности
	INTCONbits.GIE = 1;
}  
void EEREADDATA(void)
{
	CLRWDT();
	if (eeprom_read(254) == 0x3A)//Сигнатура инициализированной памяти
	
	{
		trip = erd((unsigned char)&EE_TRIP);
		triptime = erd((unsigned char)&EE_TRIPTIME);
		impavr = erd((unsigned char)&EE_TRIPAVR);
		totalodo = erd((unsigned char)&EE_TOTALODO);
		totaltime = erd((unsigned char)&EE_TOTALTIME);
		TO1_ODO = erd((unsigned char)&EE_TO1_ODO);
		TO1_TIME = erd((unsigned char)&EE_TO1_TIME);
		TO2_ODO = erd((unsigned char)&EE_TO2_ODO);
		TO2_TIME = erd((unsigned char)&EE_TO2_TIME);
	}
	else
	{
		eeprom_write(254,0x3A);
		ewr(0x00000000,(unsigned char)&EE_TRIP);
		ewr(0x00000000,(unsigned char)&EE_TRIPTIME);
		ewr(0x00000000,(unsigned char)&EE_TRIPAVR);
		ewr(0x00000000,(unsigned char)&EE_TOTALODO);
		ewr(0x00000000,(unsigned char)&EE_TOTALTIME);
		ewr(0x00000000,(unsigned char)&EE_TO1_ODO);
		ewr(0x00000000,(unsigned char)&EE_TO1_TIME);
		ewr(0x00000000,(unsigned char)&EE_TO2_ODO);
		ewr(0x00000000,(unsigned char)&EE_TO2_TIME);
	
	}
	
}
void EEWRITEFATA(void)
{	ewr(trip,(unsigned char)&EE_TRIP);
	ewr(triptime,(unsigned char)&EE_TRIPTIME);
	ewr(impavr,(unsigned char)&EE_TRIPAVR);
	ewr(totalodo,(unsigned char)&EE_TOTALODO);
	ewr(totaltime,(unsigned char)&EE_TOTALTIME);
	ewr(TO1_ODO,(unsigned char)&EE_TO1_ODO);
	ewr(TO1_TIME,(unsigned char)&EE_TO1_TIME);
	ewr(TO2_ODO,(unsigned char)&EE_TO2_ODO);
	ewr(TO2_TIME,(unsigned char)&EE_TO2_TIME);
}  
void EETRIPRESET(void)
{
	CLRWDT();
	ewr(0x00000000,(unsigned char)&EE_TRIP);
	ewr(0x00000000,(unsigned char)&EE_TRIPTIME);
	ewr(0x00000000,(unsigned char)&EE_TRIPAVR);
	_delay(40000);
	asm("RESET");
}
void EETO1RESET(void)
{
	CLRWDT();
	ewr(0x00000000,(unsigned char)&EE_TO1_ODO);
	ewr(0x00000000,(unsigned char)&EE_TO1_TIME);
	asm("RESET");
}
void EETO2RESET(void)
{
	CLRWDT();
	ewr(0x00000000,(unsigned char)&EE_TO2_ODO);
	ewr(0x00000000,(unsigned char)&EE_TO2_TIME);
	asm("RESET");
}   