#include <xc.h>
#include "../SH1106/OLED.h"
extern char scrn;
extern unsigned int speed, km,fuel,fueltank;
extern unsigned long int totaltrip, impavr,triptime;
void screen0(void)
{
scrn=0;
//PutStr_mid(0,0,"ABCDEFGHIJKLM",0x3C);
//PutStr_mid(0,2,"NOPQRSTUVWXYZ",0x3C);
//PutStr_mid(0,4,"abcdefghijklm",0x3C);
//PutStr_mid(0,6,"nopqrstuvwxyz",0x3C);
PutStr_mid(20,3,"Mini trip",0x3C);
}



void screen1(void)
{
scrn=1;
unsigned int rash=(200*(unsigned long int)fuel)/speed;
PutStr_mid(2,0,"Fuel const",0x3C);
if (speed > 10) OLED_temper_big(16, 2,rash, 0x3C);
PutStr_mid(95,2,"Ltr",0x3C);
PutStr_mid(95,4,"100",0x3C);
OLED_mid(0,6,impavr/5000,0x3C,3);OLED_dot_mid(33,6,0x3C);OLED_mid(36,6,(impavr/500)%10,0x3C,1);
PutChar_mid(46,6,'l',0x3C);
OLED_mid(74,6,(100*fueltank)/rash,0x3C,3);
PutStr_mid(106,6,"km",0x3C);
}


void screen2(void)
{
scrn=2;// Ћитры в час, всего израсходовано, осталось пробега
PutStr_mid(2,0,"Fuel const",0x3C);
OLED_temper_big(16, 2,fuel, 0x3C);
PutStr_mid(95,3,"lh",0x3C);
OLED_mid(0,6,impavr/5000,0x3C,3);
OLED_dot_mid(33,6,0x3C);
OLED_mid(36,6,(impavr/500)%10,0x3C,1);
PutChar_mid(46,6,'l',0x3C);
OLED_mid(74,6,totaltrip/10,0x3C,3);
PutStr_mid(106,6,"km",0x3C);
}

void screen3(void)
{
scrn=3;
PutStr_mid(0,5,"Trip",0x3C);
PutStr_mid(0,1,"Speed",0x3C);
OLED_4_big(64, 0,speed/2, 0x3C,3);
OLED_temper_big(48, 4,totaltrip, 0x3C);
}
void screen4(void)
{
scrn=4;

PutStr_mid(0,0,"Trip",0x3C);OLED_mid(84,0,totaltrip/10,0x3C,4);
PutStr_mid(0,2,"time",0x3C);OLED_mid(84,2,triptime/60,0x3C,2);
		 OLED_mid(108,2,triptime%60,0x3C,2);OLED_doubledot_mid(106,2,0x3C);
PutStr_mid(0,4,"Fuel",0x3C);OLED_mid(84,4,impavr/5000,0x3C,3);
		OLED_dot_mid(116,4,0x3C); OLED_mid(120,4,(impavr/500)%10,0x3C,1);
PutStr_mid(0,6,"Avr spd",0x3C);OLED_mid(84,6,6*totaltrip/triptime,0x3C,4);
}
void screen5(void)
	{
//PutStr_mid(0,0,"Total odo",0x3C);
////PutStr_mid(10,2,"km",0x3C);
//PutStr_mid(0,4,"",0x3C);
//PutStr_mid(0,6,"time",0x3C);
//OLED_4_big(50, 0,totaltrip/10, 0x3C,4);
//OLED_temper_big(32, 4,triptime/6, 0x3C);
//PutStr_mid(110,5,"h",0x3C);		
		
		
		
	}
void screen10(void)
{
	scrn=10;
	oledClear(0x3C);
	PutStr_mid(10,4,"Reset trip",0x3C);
}