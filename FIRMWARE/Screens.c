#include <xc.h>
#include "../SH1106/OLED.h"
#include "config.h"
extern char scrn;
extern unsigned int speed, km,fuel,fueltank, volt;
extern unsigned long int trip, impavr,triptime, totalodo,totaltime, TO1_ODO, TO1_TIME, TO2_ODO, TO2_TIME;
const unsigned int 	impulse_litre = impulse_per_litre / 10;
const unsigned  int 	Rdiv = impulse_per_litre / 100;
void screen0(void)
{
	scrn=0;
	//PutStr_mid(0,0,"ABCDEFGHIJKLM",OLED_Adr);
	//PutStr_mid(0,2,"NOPQRSTUVWXYZ",OLED_Adr);
	//PutStr_mid(0,4,"abcdefghijklm",OLED_Adr);
	//PutStr_mid(0,6,"nopqrstuvwxyz",OLED_Adr);
	PutStr_mid(20,3,"Mini trip",OLED_Adr);
}
void screen1(void)
{
	scrn=1;// Расход на 100км, израсходовано литров топлива, средний расход топлива
	unsigned int rash=(200*(unsigned long int)fuel)/speed;
	if (rash > 999) rash = 999;// Убрал выход расхода за пределы 99л/ч
	
	PutStr_mid(2,0,"Fuel const",OLED_Adr);
	OLED_temper_big(16, 2,rash, OLED_Adr);
	PutStr_mid(98,2,"Ltr",OLED_Adr);
	PutStr_mid(98,4,"100",OLED_Adr);
	
	// Всего израсходовано на поездку
	OLED_mid(0,6,impavr/impulse_per_litre,OLED_Adr,3);OLED_dot_mid(33,6,OLED_Adr);OLED_mid(36,6,(impavr/impulse_litre)%10,OLED_Adr,1);
	PutChar_mid(46,6,'l',OLED_Adr);
	
	// Средний расход топлива за поездку
	
	OLED_mid(81,6,(10*impavr/Rdiv)/trip,OLED_Adr,2);// Средний расход топлива
	OLED_dot_mid(103,6,OLED_Adr);
	OLED_mid(106,6,((100*impavr/Rdiv)/trip)%10,OLED_Adr,1);
	PutChar_mid(118,6,'l',OLED_Adr);
}
void screen2(void)
{
	scrn=2;// Литры в час, всего израсходовано, суточный пробег
	PutStr_mid(2,0,"Fuel const",OLED_Adr);
	
	OLED_temper_big(16, 2,fuel, OLED_Adr);
	PutStr_mid(95,3,"l/h",OLED_Adr);
	
	OLED_mid(0,6,impavr/impulse_per_litre,OLED_Adr,3);OLED_dot_mid(33,6,OLED_Adr);OLED_mid(36,6,(impavr/impulse_litre)%10,OLED_Adr,1);
	PutChar_mid(46,6,'l',OLED_Adr);
	
	OLED_mid(74,6,trip/10,OLED_Adr,3);
	PutStr_mid(106,6,"km",OLED_Adr);
}
void screen3(void)
{
	scrn=3;
	PutStr_mid(0,5,"Trip",OLED_Adr);
	PutStr_mid(0,1,"Speed",OLED_Adr);
	OLED_4_big(64, 0,speed/2, OLED_Adr,3);
	if (trip > 9999)
	{
		OLED_4_big(48, 4,trip/10, OLED_Adr,4);
	}
	else
	{
		OLED_temper_big(48, 4,trip, OLED_Adr);
	}
}
void screen4(void)
{
	scrn=4;
	unsigned int loctrip = trip/10;
	if (loctrip > 9999) loctrip =9999;
	
	PutStr_mid(0,0,"Trip",OLED_Adr);OLED_mid(82,0,loctrip,OLED_Adr,4);// Общий пробег
	
	PutStr_mid(0,2,"Time",OLED_Adr);OLED_mid(82,2,triptime/120,OLED_Adr,2);// Время в пути
			 OLED_mid(106,2,(triptime>>1)%60,OLED_Adr,2);OLED_doubledot_mid(104,2,OLED_Adr);
	
	PutStr_mid(0,4,"Fuel",OLED_Adr);OLED_mid(82,4,impavr/impulse_per_litre,OLED_Adr,3);// Израсходовано топлива
			OLED_dot_mid(114,4,OLED_Adr); OLED_mid(117,4,(impavr/impulse_litre)%10,OLED_Adr,1);
	
	PutStr_mid(0,6,"Speed",OLED_Adr);OLED_mid(82,6,12*(unsigned long int)trip/triptime,OLED_Adr,4); // Средняя скорость
}
void screen5(void)
{
	scrn=5; // Общий пробег
	PutStr_mid(0,0,"Odo",OLED_Adr);
	PutStr_mid(10,2,"km",OLED_Adr);
	PutStr_mid(0,6,"time",OLED_Adr);
//	unsigned long int loctrip = totalodo/10;
//	if (loctrip > 99999) loctrip =99999;
	OLED_4_big(40, 0,totalodo/10, OLED_Adr,5);
	OLED_4_big(48, 4,totaltime/120, OLED_Adr,4);
	PutStr_mid(119,5,"h",OLED_Adr);		

}
void screen6(void)
{
	scrn=6;// Напряжение бортовой сети
	PutStr_mid(2,0,"Battery",OLED_Adr);
	OLED_temper_big(16, 2,volt/4, OLED_Adr);
	PutStr_mid(100,3,"V",OLED_Adr);
}
void screen7(void)
{
	scrn=7;// Уровень топлива в баке
	unsigned int avrrash=(100*impavr/Rdiv)/trip;
	PutStr_mid(2,0,"Fuel tank",OLED_Adr);
	
	OLED_temper_big(16, 2,fueltank, OLED_Adr); //Остаток топлива в баке
	PutStr_mid(95,3,"ltr",OLED_Adr);
	
	OLED_mid(0,6,impavr/impulse_per_litre,OLED_Adr,3);//Израсходовано топлива
	OLED_dot_mid(33,6,OLED_Adr);
	OLED_mid(36,6,(impavr/impulse_litre)%10,OLED_Adr,1);
	PutChar_mid(46,6,'l',OLED_Adr);
	
	OLED_mid(74,6,(100*(unsigned long int)fueltank)/avrrash,OLED_Adr,3);// Остаток пробега на основе среднего расхода топлива
	PutStr_mid(106,6,"km",OLED_Adr);
}

void screen8(void)
{
	scrn=8;
	PutStr_mid(0,0,"TO1",OLED_Adr);
	PutStr_mid(10,2,"km",OLED_Adr);
	PutStr_mid(0,6,"time",OLED_Adr);
	OLED_4_big(40, 0,TO1_ODO/10, OLED_Adr,5);
	OLED_4_big(60, 4,TO1_TIME/120, OLED_Adr,3);
	PutStr_mid(118,5,"h",OLED_Adr);
}
void screen9(void)
{
	scrn=9;
	PutStr_mid(0,0,"TO2",OLED_Adr);
	PutStr_mid(10,2,"km",OLED_Adr);
	PutStr_mid(0,6,"time",OLED_Adr);
	unsigned long int loctrip = TO2_ODO/10;
	if (loctrip > 99999) loctrip =99999;
	OLED_4_big(40, 0,loctrip, OLED_Adr,5);
	OLED_4_big(60, 4,TO2_TIME/120, OLED_Adr,3);
	PutStr_mid(118,5,"h",OLED_Adr);
}
void screen10(void)
{
	scrn=10;
	oledClear(OLED_Adr);
	PutStr_mid(10,4,"Reset trip?",OLED_Adr);
}
void screen11(void)
{
	scrn=11;
	oledClear(OLED_Adr);
	PutStr_mid(10,4,"Reset TO 1?",OLED_Adr);
}
void screen12(void)
{
	scrn=12;
	oledClear(OLED_Adr);
	PutStr_mid(10,4,"Reset TO 2?",OLED_Adr);
}
void screen13(void)
{
	scrn=13;
	oledClear(OLED_Adr);
	PutStr_mid(10,4,"Set 80 ltr?",OLED_Adr);
}