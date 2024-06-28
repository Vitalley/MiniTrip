#include <xc.h>
#include <stdio.h>
#include "../SH1106/OLED.h"
#include "screens.h"
#include "button.h"
#include "config.h"
#include "screen.h"
#include "I2C.h"
#include "EEPROM.h"
//#include "median.c"
/*
fuel -  расход топлива 0.1литр в час
speed - скорость км/ч*2
impavr - израсходовано топлива за поездку 25000/литр
triptime - время в пути Сек
fueltank - количество топлива в баке *10 л
*/
unsigned int imp, fuel, speed, speedcnt, fueltank, volt, volt_array[array_size_voltage+2], fuel_array[array_size_fuel+2];
unsigned long int impavr, trip, triptime, totalodo, totaltime, TO1_ODO, TO1_TIME, TO2_ODO, TO2_TIME;
bit turn = 0, motoron = 0, saveEEDATA = 0, bri;
char scrn, mscr, mkey;
void init(void)
{
    LATA = 0x00;
    LATB = 0x00;
    TRISA = 0b00011111;
    TRISB = 0b00111111;
    ANSELB = 0xC0;
    ANSELA = 0b0001101;
    WPUB = 0x00;
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;
    APFCON0 = 0x00;
    APFCON1 = 0x00;
//	CPSCON0 = 0x00;

    OSCCON = 0x70;
    BORCON = 0x00;
//     OSCTUNE = 0x00;
     WDTCON = 0x16;
    while(PLLR == 0)
    {
	    CLRWDT();
    }
    I2CInit();    
    OPTION_REG = (unsigned char)((OPTION_REG & 0xC0) | (0xD7 & 0x3F)); 
    TMR0 = 176;   
    
    
    ADCON1 = 0xF0;
    ADRESL = 0x00;
    ADRESH = 0x00;
    ADCON0 = 0x01;
//    PIE1bits.ADIE = 1;  
    IOCBF = 0b00000000;//interrupt on change for group IOCBF - flag
    IOCBN = 0b00000001;//interrupt on change for group IOCBN - negative
    IOCBP = 0b00001000;//interrupt on change for group IOCBP - positive

    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    
    
    EEREADDATA();
    CLRWDT();
    OLEDinit(0x3C,127,62);    
    OLEDbrightness(0x3c,0xF0); 
    bri = 1;
    
    
    // Enable IOCI interrupt 
    INTCONbits.IOCIE = 1; 
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}
unsigned int GET_ADC(char channel)
{
	__delay_us(100);
	ADCON0bits.CHS = channel;
	__delay_us(100);
	ADCON0bits.GO_nDONE = 1;
	while (ADCON0bits.GO_nDONE){}
	return ((unsigned int)((ADRESH << 8) + ADRESL));
}
unsigned int SMA_filter(unsigned int a,unsigned  int* Massive, unsigned char array_size)
{
	char n=Massive[array_size];
	if (n>=(array_size)) n=0;	
	Massive[n]=a;
	Massive[array_size]=n+1;
	if (Massive[array_size+1] < array_size-1) {Massive[array_size+1]++;return (a);}
	a=0;
	for(char j=0;j<array_size;j++) 
	{
		a=a+Massive[j];
	}
	return (unsigned int)a/array_size;
}
unsigned int map(unsigned int x, unsigned int in_min, unsigned int in_max, unsigned int out_min, unsigned int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void main(void)
{
	static char seladc;
	init();
	fueltank = 100;
    _delay(999);CLRWDT();
 	oledClear(0x3C);
 	screen0();
 	for (int bra=0;bra<5600;bra++)
	{
		_delay(1999);CLRWDT();	
	}

 	mscr = 0;speed=0;fuel=0;seladc=0;


while(1)
{
	CLRWDT();
switch (seladc)
{
case 0:	if (GET_ADC(0) > 200) 
	{
		if (bri !=0 ) {OLEDbrightness(0x3c,0x00); bri = 0;}
	}	 
	else 
	{
		if (bri != 1 ) {OLEDbrightness(0x3c,0xFF); bri = 1;}
		
	}
	seladc++;	
	break;
case 1:	volt = SMA_filter(GET_ADC(3), volt_array, array_size_voltage);
	seladc++;
	break;
case 2:	fueltank = SMA_filter(GET_ADC(2), fuel_array, array_size_fuel);
	//fueltank = Wquick_select_median(fuel_array, array_size_fuel);
	fueltank = fueltank - ft_sdd;
	//fueltank = 183;
	fueltank = (fueltank * (unsigned long int)ft_mult)/256;
	if (fueltank < 1000) fueltank = 1000 - fueltank; else fueltank = 0;
	if (fueltank > 999) {fueltank = 999;}
	seladc=0;
	break;
default: seladc=0;
}
	if (saveEEDATA) {EEWRITEFATA(); saveEEDATA=0;}
	if (speed>10) turn=1; else turn=0;
	if (fuel>1) motoron=1; else motoron=0;
CLRWDT();
	switch (mscr)
	{
	case 0: 	
	
		if (turn)
		{
			if (scrn != 1)oledClear(0x3C);
			screen1();
				
		}
		
		else
		
		{
			if (motoron)
			{
		 	 	if (scrn != 2)oledClear(0x3C);
				screen2();
			 		
			}
		 	else
			{
		 		if (scrn != 4)oledClear(0x3C);
				screen4();
			}
		
		}
		break;
// Текущая скорость движения	
	case 1:	
		if (!turn) 	
		{ 
			if (!motoron) {mscr++; break;}
		}
		if (scrn != 3) oledClear(0x3C);
		screen3();
		break;
// Общая сводка о пройденом пути	
	case 2:
		if (!turn)
		{ 
			if (!motoron) {mscr++; break;}
		}	
		if (scrn != 4) oledClear(0x3C);
		screen4();
		break;
// Бортовое напряжение	
	case 3:
		if (volt<40) {mscr++; break;}
		if (scrn != 6) oledClear(0x3C);
		screen6();
		break;
//	Уровень топлива в баке и остаток пути	
	case 4:
		if (volt<10 || fueltank==0) {mscr++; break;}
		if (scrn != 7) oledClear(0x3C);
		screen7();
		break;
// TO1	
	case 6:
		if (turn) {mscr++; break;}
		if (scrn != 8) oledClear(0x3C);
		screen8();
		break;
// TO2	
	case 7:
		if (motoron) {mscr++; break;}
		if (scrn != 9) oledClear(0x3C);
		screen9();
		break;
// Общий пробег
	case 5:
		if (motoron) {mscr++; break;}
		if (scrn != 5) oledClear(0x3C);
		screen5();
		break;	
	default:
		scrn = 0;
	}
	 for (int bra=0;bra<24;bra++)
		{
		_delay(30000);CLRWDT();
		mkey = GetButton();
	
		if (mkey == 2)
		{ 
		if (mscr == 6)
		{
			screen11();
			for (unsigned char bra=0;bra<250;bra++)
			{
				mkey = GetButton();
				_delay(40000);
				if (mkey == 1) {EETO1RESET();}
			}
		}
		else if (mscr == 7) 
		{
			screen12();
			for (unsigned char bra=0;bra<250;bra++)
			{
				mkey = GetButton();
				_delay(40000);
				if (mkey == 1) {EETO2RESET();}
			}		
		}
		else 
		{
			screen10();
			for (unsigned char bra=0;bra<250;bra++)
			{
				mkey = GetButton();
				_delay(40000);
				if (mkey == 1) {EETRIPRESET();}
			}
			
			
		}
		}
		else if (mkey == 1) mscr++;
		
		if (mscr > 7) mscr=0;	
		}
}
}


void TMR0_ISR(void)
{
    static unsigned int CountSeconds = 0, Count7n2Seconds = 0, CountSPD = 0;
    static unsigned char CountTripTime;
    // callback function - called every 391th pass
    //	 Секундный счётчик
    if (++CountSeconds >= 391)
    {
	CountSeconds=0;
	// 30 Секундный счётчик
	if (++CountTripTime >= 30)
    	{
		if (fuel>1 || speed>1)
		{
			triptime++;
			totaltime++;
			TO1_TIME++;
			TO2_TIME++;
		}
		CountTripTime=0;
		saveEEDATA=1;
    	}
	
    }
// Счётчик для измерения расхода топлива
    if (++Count7n2Seconds >= 563/*1022*/)//563 при 25000 имп/литр
    {
	fuel = imp; imp = 0;
	Count7n2Seconds=0;
    }
// Счётчик скорости движения
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
    if (++CountKM >= 409) // 100 метровый счётчик
    {
	trip++; 
	totalodo++;
	TO1_ODO++;
	TO2_ODO++;
	CountKM = 0;
    }
}
//void ADC_ISR(void)
//{
//
//
//}
void interrupt isr(void)
{
	union
	{
	  struct
	     {
	     unsigned b0:1;
	     unsigned b1:1;
	     unsigned b2:1;
	     unsigned b3:1;
	     unsigned b4:1;
	     unsigned b5:1;
	     unsigned b6:1;
	     unsigned b7:1;
	     };
	     unsigned char reg;
	}IOCB;
    if(INTCONbits.IOCIE == 1 && INTCONbits.IOCIF == 1)
    {
	IOCB.reg = IOCBF;
	IOCBF = 0;
//	INTCONbits.IOCIF = 0;	//The IOCF Flag bit is read-only and cleared when all the interrupt-on-change flags in the IOCBF register have been cleared by software.
    	// interrupt on change for pin IOCBF3
	if(IOCB.b3 == 1)
    	{
	        impavr++;
	        imp++;   
    	}
        // interrupt on change for pin IOCBF0
    	if(IOCB.b0 == 1)
   		{
        	if (RB0 == 0) TripCounter();
/*        	if (IOCBN0) 
	        	{
		        	TripCounter();
		        	IOCBN0 = 0;
		        	IOCBP0 = 1;
		        }
        	if (IOCBF0)	     
	        	{
		        	IOCBP0 = 0;
		        	IOCBN0 = 1;
        		}
 */
    	}	
	// interrupt on change for pin IOCBF5
    	if(IOCB.b5 == 1)
    	{
        	//TripCounter();
        	//if (RB5 == 0)TripCounter();  
    	}		
    }
    else 
	if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
	{
		TMR0 = 176;
		INTCONbits.TMR0IF = 0;
		TMR0_ISR();
	}
    else
	if(INTCONbits.PEIE == 1)
	{
		if(PIE1bits.ADIE == 1 && PIR1bits.ADIF == 1)
		{
	        PIR1bits.ADIF == 0;
//		ADC_ISR();
		} 
        else 	if(PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1)
		{
	        PIR1bits.RCIF == 0;
 //		EUSART_RxDefaultInterruptHandler();
		} 
	}


}