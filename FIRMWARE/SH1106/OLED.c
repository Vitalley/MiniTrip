#include <xc.h>
//#include <stdio.h>
#include "../MiniTrip/I2C.h"
#include "../SH1106/OLED.h"
#define OLED_Adr 0x3C
#include "SH1106\Font16x8.h"
#include "SH1106\Font40x20.h"
void oledCommand(unsigned char comm, unsigned char Adr) {
	StartI2C();
	if (WriteI2C( (Adr<<1) & 0xFE)!=0) return;//Начинаем передачу команд устройству с адресом Adr.
  	WriteI2C(0x00);//Сообщаем дисплею, что следующее передаваемое значение - команда.
  	WriteI2C(comm);//Передаем команду.
  	StopI2C();//Завершаем передачу данных.
 }
void oledData(unsigned char data,unsigned char Adr) {
	StartI2C();
 	if (WriteI2C( (Adr<<1) & 0xFE)!=0) return;//Начинаем передачу команд устройству с адресом Adr.
	WriteI2C(0x40);//Сообщаем дисплею, что следующее передаваемое значение - данные, которые необходимо вывести на дисплей.
 	WriteI2C(data);//Передаем данные.
 	StopI2C();//Завершаем передачу данных.
}
void oled_DataBurst(unsigned char data[],unsigned char Adr,unsigned char size)
{
	StartI2C();
	if (WriteI2C( (Adr<<1) & 0xFE)!=0) return;//Начинаем передачу команд устройству с адресом Adr.
	WriteI2C(0x40);//Сообщаем дисплею, что следующее передаваемое значение - данные, которые необходимо вывести на дисплей.
 	
 	for (char k=0; k < size; k++)
 	{
 		WriteI2C(data[k]);//Передаем данные.
 	} 
 	StopI2C();//Завершаем передачу данных.	
}
unsigned char oledRead(unsigned char Adr)
{
	StartI2C();
 	if (WriteI2C( (Adr<<1) & 0xFE)!=0) return(0);//Начинаем передачу команд устройству с адресом Adr.
	WriteI2C(0x40);//Сообщаем дисплею, что следующее передаваемое значение - данные, которые необходимо вывести на дисплей.
 	unsigned char dat = ReadI2C();//Передаем данные.
 	StopI2C();//Завершаем передачу данных.
 	return(dat);
}
void OLED_set_x (unsigned char x,unsigned char Adr)
{
	oledCommand(0x0f & (x),Adr);//set column addr 4 lower 
	oledCommand(0x10|(x>>4),Adr);//set column addr 4 higher 
	return;
}
void OLED_set_y (unsigned char y,unsigned char Adr)
{
	oledCommand(0xB0 ^ (y & 0x0F), Adr);
}
void OLED_set_XY (unsigned char x,unsigned char y,unsigned char Adr)
{
	oledCommand(0x0f & (x),Adr);//set column addr 4 lower 
	oledCommand(0x10|(x>>4),Adr);//set column addr 4 higher 
	oledCommand(0xB0 ^ (y & 0x0F), Adr);
}
void oledClear(unsigned char Adr) //Эта функция очищает дисплей.
{
const unsigned char page = 0xB0;
	for(unsigned char y = 0; y < 8; y++) 
	{
		oledCommand(page+y, Adr);
		OLED_set_x(2, Adr);
		StartI2C();
		if (WriteI2C( (Adr<<1) & 0xFE)!=0) return;//Начинаем передачу команд устройству с адресом Adr.
		WriteI2C(0x40);//Сообщаем дисплею, что следующее передаваемое значение - данные, которые необходимо вывести на дисплей.
		for(unsigned char x = 0; x < 128; x++)
		{
		//	oledData(0x00, Adr);
			 WriteI2C(0x00);
		}
		StopI2C();//Завершаем передачу данных.
	}
}


void OLEDinit(unsigned char Adress,unsigned char x_size, unsigned char y_size) { 
//процесс инициализации честно скопирован из даташита на дисплей.
 	oledCommand(0xAE,Adress);//выключение дисплея
	oledCommand(0xD5,Adress);//Set Display Clock Divide Ratio/Oscillator Frequency
	oledCommand(0x80,Adress);//(00H - 3FH)
	oledCommand(0xA8,Adress);// Multiplex Ration Mode Set: (A8H)
	oledCommand(y_size,Adress);//00 Multiplex Ration Data Set: (00H - 3FH)
	oledCommand(0xD3,Adress);
	oledCommand(0x00,Adress);
	oledCommand(0x00,Adress);//Младшие биты адреса колонки
	oledCommand(0x10,Adress);//Старшие биты адреса колонки 
	oledCommand(0x40,Adress);//Стартовая строка: (40H - 7FH)
	oledCommand(0xB0,Adress);//Стартовая страница	
	oledCommand(0x8D,Adress);//включение емкостного умножителя
//	oledCommand(0x14,Adress);
	oledCommand(0x20,Adress);//настройка адресации
	oledCommand(0x00,Adress);// 0х00 для горизонтальной, 0х01 для вертикальной, 0х02 для постраничной адресации
	oledCommand(0xA1,Adress);//отражение по горизонтали, для отображения справа налево необходимо использовать команду 0xA0 (A0H - A1H)
	oledCommand(0xC8,Adress);//отражение по вертикали, 0xC0 для переворота изображения по вертикали.
//Одновременное использование команд 0xC8 и 0xA1 или 0xA0 и 0xC0 позволяет повернуть изображение на 180 градусов.
	oledCommand(0xDA,Adress);// Common Pads Hardware Configuration Mode Set: (DAH)
	oledCommand(0x12,Adress);//Sequential/Alternative Mode Set: (02H - 12H) 
	oledCommand(0x10,Adress);//установка контрастности дисплея
	oledCommand(0xCF,Adress);//Контрастность (00-FFh)
	oledCommand(0xD9,Adress);//Pre-charge Period Mode Set: (D9H) 
	oledCommand(0x1F,Adress);//  Dis-charge/Pre-charge Period Data Set: (00H - FFH)
	oledCommand(0xDB,Adress);// VCOM Deselect Level Mode Set: (DBH)
	oledCommand(0x40,Adress);//VCOM Deselect Level Data Set: (00H - FFH)
	oledCommand(0xA4,Adress);// Set Entire Display OFF/ON: (A4H - A5H)
	oledCommand(0xA6,Adress);//инверсия дисплея, 0xA6 для отключения инверсии, 0xA7 для включения инверсии цвета. Set Normal/Reverse Display: (A6H -A7H)
	oledCommand(0xAF,Adress);//включение дисплея
	//IdleI2C();
//	oledClear(Adress);//Используем функцию очистки дисплея.
}
void OLEDbrightness(unsigned char Adress, unsigned char Brtns)
{
	oledCommand(0x81,Adress);
	oledCommand(Brtns,Adress); //Контрастность (00-FFh)
}
void OLEDdrawbar(unsigned char Adress, unsigned char y, int val)
{
	OLED_set_y(y ,Adress);
	OLED_set_x (0, Adress);
	for(unsigned char x = 0; x < 132; x++)
	{
	if (x<val)	{oledData(0xFF,Adress);} else {oledData(0x00,Adress);}
	}
}
void OLEDdrawcol(unsigned char Adress, unsigned char x, int val)
{
	for(unsigned char y = 2; y < 7; y++)
	{
		OLED_set_x (x, Adress);
		OLED_set_y (y ,Adress);
		char S=0, C=0,str[14];
		for (char n=0;n<8;n++)
		{
			char D=n+((y-2)*8);
			if ( ((D> val) & (D<19)) | ((D< val) & (D>20)) ) S = S|(1 << n); 				
		}
		for(unsigned char d = 0; d < 14; d++)
		{
			if (y == 4)  {C = 0x18;}else{C = 0;}
			//if (d > 4 && d < 11){oledData(S|C,Adress);}else{oledData(C,Adress);}
			if (d > 4 && d < 11){str[d]=S|C;}else{str[d]=C;}
			
		}
		oled_DataBurst(str,Adress,14);
	}	
	
}
void drawPixel(unsigned char x, unsigned char y, unsigned char color)
{
//if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;



}
void PutChar_mid(unsigned char x,unsigned char y,unsigned char dig,unsigned char Adress) //
{
	unsigned char i=0,pitch=0x21, str[12]; 
	if( dig<pitch ){dig=45;}
	if( dig>=0x7F ){dig=45;}
	OLED_set_x (x, Adress);OLED_set_y (y, Adress);
	for(i=1; i<13; i++) {str[i-1]=(font_16x8_data_table[dig-pitch][i]);}
	oled_DataBurst(str, Adress, 12);
	OLED_set_x (x, Adress);OLED_set_y (y+1, Adress);	
	for(i=13; i<24; i++) {str[i-13]=(font_16x8_data_table[dig-pitch][i]);}
	oled_DataBurst(str, Adress, 12);
}
void PutStr_mid(unsigned char x, unsigned char y, const char txt[],unsigned char Adr)
{
	unsigned char i=0;
	while (txt[i]!=0 & i<32)
	{
		PutChar_mid(x+i*10,y,txt[i],Adr);
		i++;
	}
	return;
}
void PutNumber_mid(unsigned char x, unsigned char y, int val,unsigned char Adr)
{
	if((val>999)&(val<10000))
	{
		PutChar_mid(x,	y,val/1000 + 0x30,Adr);
		PutChar_mid(x+12,y,(val/100)%10 + 0x30,Adr);
		PutChar_mid(x+24,y,(val/10)%10 + 0x30,Adr);
		PutChar_mid(x+36,y,val%10+0x30,Adr);
	}
	if((val>99)&(val<1000))
	{
		PutChar_mid(x	 ,y,(val/100)%10 + 0x30,Adr);
		PutChar_mid(x+12,y,(val/10)%10 + 0x30,Adr);
		PutChar_mid(x+24,y,val%10+0x30,Adr);
		PutChar_mid(x+36,y,' ',Adr);
	}
	if((val>9)&(val<100))
	{
		PutChar_mid(x,   y,(val/10)%10 + 0x30,Adr);
		PutChar_mid(x+12,y,val%10+0x30,Adr);
		PutChar_mid(x+24,y,' ',Adr);
		PutChar_mid(x+36,y,' ',Adr);
	}
		if(val<10)
	{
		PutChar_mid(x,   y,val%10+0x30,Adr);
		PutChar_mid(x+12,y,' ',Adr);
		PutChar_mid(x+24,y,' ',Adr);
		PutChar_mid(x+36,y,' ',Adr);
	}
	return;
}

void OLED_Char_big(unsigned char x,unsigned char y, char dig, unsigned char Adr) //
{
	unsigned char i,wide=18,offcet=0,str[20];				
	if (dig>10) dig=10;
	OLED_set_XY (x, y, Adr);
	for(i=offcet; i<wide; i++) {str[i-offcet]=(Font40x20[(dig*72)+i]);}
	oled_DataBurst(str, Adr, wide);
	
	OLED_set_XY (x, y+1, Adr);
	for(i=offcet+18; i<wide+18; i++){str[i-(offcet+18)]=(Font40x20[(dig*72)+i]);}
	oled_DataBurst(str, Adr, wide);	
	
	OLED_set_XY (x, y+2, Adr);	
	for(i=offcet+36; i<wide+36; i++){str[i-(offcet+36)]=(Font40x20[(dig*72)+i]);}
	oled_DataBurst(str, Adr, wide);
	
	OLED_set_XY (x, y+3, Adr);
	for(i=offcet+54; i<wide+54;i++){str[i-(offcet+54)]=(Font40x20[(dig*72)+i]);}	
	oled_DataBurst(str, Adr, wide);
}
void OLED_Dot_big(unsigned char x,unsigned char y, unsigned char Adr)
{
	unsigned char i=0,str[6];
	OLED_set_XY (x, y+3, Adr);
	for(i=0; i<2;i++){str[i]=0x00;}
	for(i=2; i<7;i++){str[i]=0x1F;}	
	oled_DataBurst(str, Adr, 6);
}
void OLED_temper_big(unsigned char x, unsigned char y,int val, unsigned char Adr)
{
	if((val>999)&(val<10000))
	{
	OLED_Char_big(x,y,val/1000, Adr);
	}
	if (val<0)
	{
	OLED_Char_big(x,y,11, Adr);val=-val;	
	}	
	OLED_Char_big(x+18,y,(val/100)%10, Adr);
	OLED_Char_big(x+36,y,(val/10)%10, Adr);
	OLED_Dot_big(x+52, y, Adr);
	OLED_Char_big(x+61,y,val%10, Adr);
	return;
}
void OLED_4_big(unsigned char x, unsigned char y,unsigned int val, unsigned char Adr, unsigned char c)
{
	switch (c)
	{
	case 5: OLED_Char_big(x,y,(val/10000)%10, Adr);x=x+18;
	case 4: OLED_Char_big(x,y,(val/1000)%10, Adr);x=x+18;
	case 3: OLED_Char_big(x,y,(val/100)%10, Adr);x=x+18;
	case 2: OLED_Char_big(x,y,(val/10)%10, Adr);x=x+18;
	case 1: OLED_Char_big(x,y,val%10, Adr);
	default: return;
	}
}
void OLED_mid(unsigned char x, unsigned char y, int val,unsigned char Adr, unsigned char c)
{
	switch (c)
	{
	case 5:	PutChar_mid(x,y,(val/10000)%10 + 0x30,Adr); x=x+11;
	case 4:	PutChar_mid(x,y,(val/1000)%10 + 0x30,Adr); x=x+11;
	case 3:	PutChar_mid(x,y,(val/100)%10 + 0x30,Adr); x=x+11;
	case 2:	PutChar_mid(x,y,(val/10)%10 + 0x30,Adr); x=x+11;
	case 1:	PutChar_mid(x,y,val%10+0x30,Adr);
	default: return;
	}
}
void OLED_dot_mid(unsigned char x, unsigned char y, unsigned char Adr)
{
	OLED_set_XY (x, y+1, Adr);
	oledData(0x00,Adr);
	oledData(0b01011000,Adr);
	oledData(0b00111000,Adr);
	oledData(0x00,Adr);
}
void OLED_doubledot_mid(unsigned char x, unsigned char y, unsigned char Adr)
{
	OLED_set_XY (x, y+1, Adr);
	oledData(0x00,Adr);
	oledData(0b01100011,Adr);
	oledData(0b01100011,Adr);
	oledData(0x00,Adr);
}