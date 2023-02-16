#include <xc.h>
#include "../SH1106/OLED.h"
#include "config.h"
extern char scrn;
extern unsigned int speed, km,fuel,fueltank, volt;
extern unsigned long int trip, impavr,triptime, totalodo,totaltime, TO1_ODO, TO1_TIME, TO2_ODO, TO2_TIME;
const unsigned int 	impilse_litre = impilse_per_litre / 10,
			rash_const_mul = 400;// 10000000 / impilse_per_litre;
			rash_const_div = 25;// impilse_per_litre / 1000;
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
	scrn=1;// Расход на 100км, израсходовано литров топлива, средний расход топлива
	unsigned int rash=(rash_const_mul*(unsigned long int)fuel)/speed;
	if (rash > 999) rash = 999;// Убрал выход расхода за пределы 99л/ч
	PutStr_mid(2,0,"Fuel const",0x3C);
	OLED_temper_big(16, 2,rash, 0x3C);
	PutStr_mid(98,2,"Ltr",0x3C);
	PutStr_mid(98,4,"100",0x3C);
	OLED_mid(0,6,impilse_per_litre/50000,0x3C,3);OLED_dot_mid(33,6,0x3C);OLED_mid(36,6,(impavr/impilse_litre)%10,0x3C,1);
	PutChar_mid(46,6,'l',0x3C);
	OLED_mid(81,6,(impavr/50)/trip,0x3C,2);// Средний расход топлива
	OLED_dot_mid(103,6,0x3C);
	OLED_mid(106,6,((impavr/5)/trip)%10,0x3C,1);
	PutStr_mid(118,6,"l",0x3C);
}


void screen2(void)
{
	scrn=2;// Литры в час, всего израсходовано, суточный пробег
	PutStr_mid(2,0,"Fuel const",0x3C);
	OLED_temper_big(16, 2,fuel, 0x3C);
	PutStr_mid(95,3,"l/h",0x3C);
	OLED_mid(0,6,impavr/impilse_per_litre,0x3C,3);OLED_dot_mid(33,6,0x3C);OLED_mid(36,6,(impavr/impilse_litre)%10,0x3C,1);
	PutChar_mid(46,6,'l',0x3C);
	OLED_mid(74,6,trip/10,0x3C,3);
	PutStr_mid(106,6,"km",0x3C);
}

void screen3(void)
{
	scrn=3;
	PutStr_mid(0,5,"Trip",0x3C);
	PutStr_mid(0,1,"Speed",0x3C);
	OLED_4_big(64, 0,speed/2, 0x3C,3);
	OLED_temper_big(48, 4,trip, 0x3C);
}
void screen4(void)
{
	scrn=4;
	PutStr_mid(0,0,"Trip",0x3C);OLED_mid(84,0,trip/10,0x3C,4);// Общий пробег
	
	PutStr_mid(0,2,"time",0x3C);OLED_mid(84,2,triptime/120,0x3C,2);// Время в пути
			 OLED_mid(108,2,triptime%120,0x3C,2);OLED_doubledot_mid(106,2,0x3C);
	
	PutStr_mid(0,4,"Fuel",0x3C);OLED_mid(84,4,impavr/impilse_per_litre,0x3C,3);// Израсходовано топлива
			OLED_dot_mid(116,4,0x3C); OLED_mid(120,4,(impavr/impilse_litre)%10,0x3C,1);
	
	PutStr_mid(0,6,"Speed",0x3C);OLED_mid(84,6,12*(unsigned long int)trip/triptime,0x3C,4); // Средняя скорость
}
void screen5(void)
{
	scrn=5; // Общий пробег
	PutStr_mid(0,0,"Odo",0x3C);
	PutStr_mid(10,2,"km",0x3C);
	PutStr_mid(0,6,"time",0x3C);
	OLED_4_big(40, 0,totalodo/10, 0x3C,5);
	OLED_4_big(48, 4,totaltime/120, 0x3C,4);
	PutStr_mid(119,5,"h",0x3C);		

}
void screen6(void)
{
	scrn=6;// Напряжение бортовой сети
	PutStr_mid(2,0,"Battery",0x3C);
	OLED_temper_big(16, 2,volt/4, 0x3C);
	PutStr_mid(100,3,"V",0x3C);
//	OLED_mid(0,6,impavr/50000,0x3C,3);
//	OLED_dot_mid(33,6,0x3C);
//	OLED_mid(36,6,(impavr/5000)%10,0x3C,1);
//	PutChar_mid(46,6,'l',0x3C);
//	OLED_mid(74,6,trip/10,0x3C,3);
//	PutStr_mid(106,6,"km",0x3C);
}
void screen7(void)
{
	scrn=7;// Уровень топлива в баке
	unsigned int avrrash=(impavr/rash_const_div)/trip;
	PutStr_mid(2,0,"Fuel tank",0x3C);
	OLED_temper_big(16, 2,fueltank, 0x3C); //Остаток топлива в баке
	PutStr_mid(95,3,"ltr",0x3C);
	OLED_mid(0,6,impavr/impilse_per_litre,0x3C,3);//Израсходовано топлива
	OLED_dot_mid(33,6,0x3C);
	OLED_mid(36,6,(impavr/impilse_litre)%10,0x3C,1);
	PutChar_mid(46,6,'l',0x3C);
	OLED_mid(74,6,(10*(unsigned long int)fueltank)/avrrash,0x3C,3);// Остаток пробега на основе среднего расхода топлива
	PutStr_mid(106,6,"km",0x3C);
}

void screen8(void)
{
	scrn=8;
	PutStr_mid(0,0,"TO1",0x3C);
	PutStr_mid(10,2,"km",0x3C);
	PutStr_mid(0,6,"time",0x3C);
	OLED_4_big(40, 0,TO1_ODO/10, 0x3C,5);
	OLED_4_big(60, 4,TO1_TIME/120, 0x3C,3);
	PutStr_mid(118,5,"h",0x3C);
}
void screen9(void)
{
	scrn=9;
	PutStr_mid(0,0,"TO2",0x3C);
	PutStr_mid(10,2,"km",0x3C);
	PutStr_mid(0,6,"time",0x3C);
	OLED_4_big(40, 0,TO2_ODO/10, 0x3C,5);
	OLED_4_big(60, 4,TO2_TIME/120, 0x3C,3);
	PutStr_mid(118,5,"h",0x3C);
}
void screen10(void)
{
	scrn=10;
	oledClear(0x3C);
	PutStr_mid(10,4,"Reset trip?",0x3C);
}
void screen11(void)
{
	scrn=11;
	oledClear(0x3C);
	PutStr_mid(10,4,"Reset TO 1?",0x3C);
}
void screen12(void)
{
	scrn=12;
	oledClear(0x3C);
	PutStr_mid(10,4,"Reset TO 2?",0x3C);
}