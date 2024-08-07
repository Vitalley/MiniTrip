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
	//PutStr_mid(0,0,"ABCDEFGHIJKLM",0x3C);
	//PutStr_mid(0,2,"NOPQRSTUVWXYZ",0x3C);
	//PutStr_mid(0,4,"abcdefghijklm",0x3C);
	//PutStr_mid(0,6,"nopqrstuvwxyz",0x3C);
	PutStr_mid(20,3,"Mini trip",0x3C);
}
void screen1(void)
{
	scrn=1;// ������ �� 100��, ������������� ������ �������, ������� ������ �������
	unsigned int rash=(200*(unsigned long int)fuel)/speed;
	if (rash > 999) rash = 999;// ����� ����� ������� �� ������� 99�/�
	
	PutStr_mid(2,0,"Fuel const",0x3C);
	OLED_temper_big(16, 2,rash, 0x3C);
	PutStr_mid(98,2,"Ltr",0x3C);
	PutStr_mid(98,4,"100",0x3C);
	
	// ����� ������������� �� �������
	OLED_mid(0,6,impavr/impulse_per_litre,0x3C,3);OLED_dot_mid(33,6,0x3C);OLED_mid(36,6,(impavr/impulse_litre)%10,0x3C,1);
	PutChar_mid(46,6,'l',0x3C);
	
	// ������� ������ ������� �� �������
	
	OLED_mid(81,6,(10*impavr/Rdiv)/trip,0x3C,2);// ������� ������ �������
	OLED_dot_mid(103,6,0x3C);
	OLED_mid(106,6,((100*impavr/Rdiv)/trip)%10,0x3C,1);
	PutChar_mid(118,6,'l',0x3C);
}
void screen2(void)
{
	scrn=2;// ����� � ���, ����� �������������, �������� ������
	PutStr_mid(2,0,"Fuel const",0x3C);
	
	OLED_temper_big(16, 2,fuel, 0x3C);
	PutStr_mid(95,3,"l/h",0x3C);
	
	OLED_mid(0,6,impavr/impulse_per_litre,0x3C,3);OLED_dot_mid(33,6,0x3C);OLED_mid(36,6,(impavr/impulse_litre)%10,0x3C,1);
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
	if (trip > 9999)
	{
		OLED_4_big(48, 4,trip/10, 0x3C,4);
	}
	else
	{
		OLED_temper_big(48, 4,trip, 0x3C);
	}
}
void screen4(void)
{
	scrn=4;
	unsigned int loctrip = trip/10;
	if (loctrip > 9999) loctrip =9999;
	
	PutStr_mid(0,0,"Trip",0x3C);OLED_mid(82,0,loctrip,0x3C,4);// ����� ������
	
	PutStr_mid(0,2,"Time",0x3C);OLED_mid(82,2,triptime/120,0x3C,2);// ����� � ����
			 OLED_mid(106,2,(triptime>>1)%60,0x3C,2);OLED_doubledot_mid(104,2,0x3C);
	
	PutStr_mid(0,4,"Fuel",0x3C);OLED_mid(82,4,impavr/impulse_per_litre,0x3C,3);// ������������� �������
			OLED_dot_mid(114,4,0x3C); OLED_mid(117,4,(impavr/impulse_litre)%10,0x3C,1);
	
	PutStr_mid(0,6,"Speed",0x3C);OLED_mid(82,6,12*(unsigned long int)trip/triptime,0x3C,4); // ������� ��������
}
void screen5(void)
{
	scrn=5; // ����� ������
	PutStr_mid(0,0,"Odo",0x3C);
	PutStr_mid(10,2,"km",0x3C);
	PutStr_mid(0,6,"time",0x3C);
//	unsigned long int loctrip = totalodo/10;
//	if (loctrip > 99999) loctrip =99999;
	OLED_4_big(40, 0,totalodo/10, 0x3C,5);
	OLED_4_big(48, 4,totaltime/120, 0x3C,4);
	PutStr_mid(119,5,"h",0x3C);		

}
void screen6(void)
{
	scrn=6;// ���������� �������� ����
	PutStr_mid(2,0,"Battery",0x3C);
	OLED_temper_big(16, 2,volt/4, 0x3C);
	PutStr_mid(100,3,"V",0x3C);
}
void screen7(void)
{
	scrn=7;// ������� ������� � ����
	unsigned int avrrash=(100*impavr/Rdiv)/trip;
	PutStr_mid(2,0,"Fuel tank",0x3C);
	
	OLED_temper_big(16, 2,fueltank, 0x3C); //������� ������� � ����
	PutStr_mid(95,3,"ltr",0x3C);
	
	OLED_mid(0,6,impavr/impulse_per_litre,0x3C,3);//������������� �������
	OLED_dot_mid(33,6,0x3C);
	OLED_mid(36,6,(impavr/impulse_litre)%10,0x3C,1);
	PutChar_mid(46,6,'l',0x3C);
	
	OLED_mid(74,6,(100*(unsigned long int)fueltank)/avrrash,0x3C,3);// ������� ������� �� ������ �������� ������� �������
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
	unsigned long int loctrip = TO2_ODO/10;
	if (loctrip > 99999) loctrip =99999;
	OLED_4_big(40, 0,loctrip, 0x3C,5);
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
void screen13(void)
{
	scrn=13;
	oledClear(0x3C);
	PutStr_mid(10,4,"Set 80 ltr?",0x3C);
}