#include <xc.h>
#include <stdio.h>
#include "../SH1106/OLED.h"
#include "screens.h"
#include "button.h"
#include "I2C.h"
#define _XTAL_FREQ 320000000
#define _clock_freq 400000
/*
fuel -  расход топлива 0.1литр в час
speed - скорость км/ч*2
impavr - израсходовано топлива за поездку 5000/литр
triptime - время в пути Сек
fueltank - количество топлива в баке *10 л
*/
unsigned int imp, fuel, speed, speedcnt, km, fueltank;
unsigned long int impavr, spdavr, timearv, totaltrip, triptime;
bit turn, motoron, saveEEDATA=0;
char scrn;
void init(void)
{
    LATA = 0x00;
    LATB = 0x00;
    TRISA = 0b00101111;
    TRISB = 0b00111111;
    ANSELB = 0x00;//0xE0;
    ANSELA = 0x01;//0x1F;
    WPUB = 0x00;
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;
    APFCON0 = 0x00;
    APFCON1 = 0x00;


    // SCS FOSC; SPLLEN disabled; IRCF 8MHz_HF; 
    OSCCON = 0x70;
    BORCON = 0x00;
//     OSCTUNE = 0x00;
     WDTCON = 0x16;
    while(PLLR == 0)
    {
    }
    I2CInit();    
    OPTION_REG = (unsigned char)((OPTION_REG & 0xC0) | (0xD7 & 0x3F)); 
    TMR0 = 176;    
    
    ADCON1 = 0x70;
    ADRESL = 0x00;
    ADRESH = 0x00;
    ADCON0 = 0x01;
    PIE1bits.ADIE = 1;  
    
    //interrupt on change for group IOCBF - flag
    IOCBFbits.IOCBF0 = 0;
    //interrupt on change for group IOCBF - flag
    IOCBFbits.IOCBF3 = 0;
    //interrupt on change for group IOCBN - negative
    IOCBNbits.IOCBN0 = 1;
    //interrupt on change for group IOCBN - negative
    IOCBNbits.IOCBN3 = 1;
    //interrupt on change for group IOCBP - positive
    IOCBPbits.IOCBP0 = 0;
    //interrupt on change for group IOCBP - positive
    IOCBPbits.IOCBP3 = 0;
    
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    
    
    EEREADDATA();
      OLEDinit(0x3C,127,63);    
    
    
    
    // Enable IOCI interrupt 
    INTCONbits.IOCIE = 1; 
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

 	//OpenI2C(/*MASTER,SLEW_OFF*/)

}
void main(void)
{
	init();
	fueltank = 100;
     OLEDbrightness(0x3c,0xFF);
    _delay(999);
 	//IdleI2C();
 	screen0();
 		for (int bra=0;bra<5600;bra++)
	{
	_delay(1999);	
	}

 	

while(1)
	{
	 for (int bra=0;bra<560;bra++)
	{
		_delay(2990);
		char mscr;
		char mkey = GetButton();
		if (mkey == 2) 
		{
			screen10();
			//_delay(2990);
			EETRIPRESET();
			//_delay(2990);
		}
		if (mkey ==1) mscr++;
		if (mscr > 2) mscr=0;	
	}
	if (saveEEDATA) {EEWRITEFATA(); saveEEDATA=0;}

 	
 if (fuel>0)
 	{
 	if (speed>10)
 		{
		if (scrn != 1)oledClear(0x3C);
		screen1();
		
 		}
 	else
 		{
 		if (scrn != 2)oledClear(0x3C);
		screen2();
 		
 		
 		}
  
 	}
 	
 else
 	{
  	if (speed>10)
 		{
 		if (scrn != 3)oledClear(0x3C);
 		
 		screen3();
 		
 		
 		
 		}
 	else
 		{
 		if (scrn != 4)oledClear(0x3C);
 		screen4();
 		
 		
 		}	
 		
 	}
	


	}
}


void TMR0_ISR(void)
{
    static unsigned int CountSeconds = 0, Count7n2Seconds = 0, CountSPD = 0;
    static unsigned char CountTripTime;

    // Clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    TMR0 = 176;

    // callback function - called every 391th pass
    if (++CountSeconds >= 391)
    {
	timearv++;
	CountSeconds=0;
	if (++CountTripTime >= 60)
    	{
		triptime++;
		CountTripTime=0;
		saveEEDATA=1;
	
    	}
	
    }
    if (++Count7n2Seconds >= 2815)
    {
	fuel = imp; imp = 0;
	Count7n2Seconds=0;
    }
    if (++CountSPD >= 698)
    {
	CountSPD=0; 
	speed = speedcnt;
	speedcnt = 0;
    }



}
void TripCounter(void)
{
    static volatile unsigned int CountKM = 0;
    speedcnt++;
//    LATAbits.LATA0 = PORTAbits.RA0^1;
    if (++CountKM >= 400)
    {
	totaltrip++; // 100 метровый счётчик
	CountKM=0;
    }
}
//void ADC_ISR(void)
//{
//
//
//}
void interrupt isr(void)
{
    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
    {
        TMR0_ISR();
    }

    if(INTCONbits.IOCIE == 1 && INTCONbits.IOCIF == 1)
    {
        // interrupt on change for pin IOCBF0
    	if(IOCBFbits.IOCBF0 == 1)
   	{
        	IOCBFbits.IOCBF0 = 0;
        	
        	if (RB0 == 0) 
        	{//static char d;d++; if (d >= 10)
        	//{d=0;
	        	 imp++; impavr++;
        	//} 
        	}
        	//if (RB0 == 0) TripCounter(); 
    	}	
	// interrupt on change for pin IOCBF3
    	if(IOCBFbits.IOCBF3 == 1)
    	{
        	IOCBFbits.IOCBF3 = 0;
        	if (RB3 == 0) TripCounter();
        	//if (impavr%50 == 1)TripCounter();
        	//TripCounter();  
    	}	
    }
    else if(INTCONbits.PEIE == 1)
    {
        if(PIE1bits.ADIE == 1 && PIR1bits.ADIF == 1)
        {
//            ADC_ISR();
        } 
        else if(PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1)
        {
 //           EUSART_RxDefaultInterruptHandler();
        } 
    }


}