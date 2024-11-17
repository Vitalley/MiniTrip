#include <xc.h>
//#include <stdio.h>
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
unsigned int  fuel, speed, speedcnt, fueltank, volt, volt_array[array_size_voltage+2], fuel_array[array_size_fuel+2];
unsigned long int impavr, trip, triptime, totalodo, totaltime, TO1_ODO, TO1_TIME, TO2_ODO, TO2_TIME;
bit turn = 0, motoron = 0, saveEEDATA = 0, bri;
static unsigned int TMR1_OLD_DATA = 0;
char scrn, mscr, mkey;
void init(void)
{
    LATA = 0x00;
    LATB = 0x00;
    TRISA = 0b00011111;
    TRISB = 0b01111111;
    ANSELB = 0x00;
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
     //T1GSS T1G_pin; TMR1GE disabled; T1GTM disabled; T1GPOL low; T1GGO done; T1GSPM disabled; 
    T1GCON = 0x00;
    TMR1H = 0x00;
    TMR1L = 0x00;
    PIR1bits.TMR1IF = 0;  
    T1CON = 0b10000011; 
    T1GCON = 0;
    // Set TMR2 to the options selected in the User Interface
    PR2 = 0x03;
    TMR2 = 0x00;
    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR2IF = 0;
    // Enabling TMR2 interrupt.
    PIE1bits.TMR2IE = 1;
    // T2CKPS 1:64; T2OUTPS 1:1; TMR2ON on; 
    T2CON = 0x07;
 
    
    ADCON1 = 0xF0;
    ADRESL = 0x00;
    ADRESH = 0x00;
    ADCON0 = 0x01;


    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    
    
    EEREADDATA();
    CLRWDT();
    _delay(1999);
    OLEDinit(OLED_Adr,127,62);    
    OLEDbrightness(OLED_Adr,0xF0); 
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
 	oledClear(OLED_Adr);
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
		if (bri !=0 ) {OLEDbrightness(OLED_Adr,0x00); bri = 0;}
	}	 
	else 
	{
		if (bri != 1 ) {OLEDbrightness(OLED_Adr,0xFF); bri = 1;}
		
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
			if (scrn != 1)oledClear(OLED_Adr);
			screen1();
				
		}
		
		else
		
		{
			if (motoron)
			{
		 	 	if (scrn != 2)oledClear(OLED_Adr);
				screen2();
			 		
			}
		 	else
			{
		 		if (scrn != 4)oledClear(OLED_Adr);
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
		if (scrn != 3) oledClear(OLED_Adr);
		screen3();
		break;
// Общая сводка о пройденом пути	
	case 2:
		if (!turn)
		{ 
			if (!motoron) {mscr++; break;}
		}	
		if (scrn != 4) oledClear(OLED_Adr);
		screen4();
		break;
// Бортовое напряжение	
	case 3:
		if (volt<40) {mscr++; break;}
		if (scrn != 6) oledClear(OLED_Adr);
		screen6();
		break;
//	Уровень топлива в баке и остаток пути	
	case 4:
		if (volt<10 || fueltank==0) {mscr++; break;}
		if (scrn != 7) oledClear(OLED_Adr);
		screen7();
		break;
// TO1	
	case 6:
		if (turn) {mscr++; break;}
		if (scrn != 8) oledClear(OLED_Adr);
		screen8();
		break;
// TO2	
	case 7:
		if (motoron) {mscr++; break;}
		if (scrn != 9) oledClear(OLED_Adr);
		screen9();
		break;
// Общий пробег
	case 5:
		if (motoron) {mscr++; break;}
		if (scrn != 5) oledClear(OLED_Adr);
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
    if (++Count7n2Seconds >= 563)//563 при 25000 имп/литр
    {
// Код не вместился в память
//	char TMPH, TMPL;
//	do
//	{
//		TMPH = TMR1H;
//		TMPL = TMR1L;
//	}
//	while (TMPH == TMR1H);
//	unsigned int TMR_DATA = TMPH <<8 + TMPL;
	unsigned int TMR_DATA = TMR1;
	unsigned int FUEL = TMR_DATA - TMR1_OLD_DATA;
	TMR1_OLD_DATA = TMR_DATA;
	fuel = FUEL;
	impavr+=FUEL;
	Count7n2Seconds=0;
    }
// Счётчик скорости движения
    if (++CountSPD >= 698)
    {
	CountSPD = 0; 
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
void interrupt isr(void)
{
   if(INTCONbits.INTF)
    {
	    INTCONbits.INTF = 0;
	    INTCONbits.INTE = 0;
	    T2CONbits.TMR2ON = 1;
	       	if (OPTION_REGbits.INTEDG) 
		        {
			        TripCounter();
			        OPTION_REGbits.INTEDG = 0;

			     }
	        else     
		        {
			        OPTION_REGbits.INTEDG = 1;
	        	}
	}
    else 
    if(PIR1bits.TMR2IF)
    {
    		PIR1bits.TMR2IF = 0;
    		T2CONbits.TMR2ON = 0;
    		INTCONbits.INTE = 1;
			TMR2 = 0;
    }
    else 
	if(INTCONbits.TMR0IF)
	{
		TMR0 = 176;
		INTCONbits.TMR0IF = 0;
		TMR0_ISR();
	}


}