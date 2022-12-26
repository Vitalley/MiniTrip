#include <xc.h>

void oledCommand(unsigned char comm, unsigned char Adr);
void oledData(unsigned char data,unsigned char Adr);
void oled_DataBurst(unsigned char data[],unsigned char Adr,unsigned char size);
void oledClear(unsigned char Adr);
void OLEDinit(unsigned char Adress,unsigned char x_size, unsigned char y_size);
void OLEDbrightness(unsigned char Adress, unsigned char Brtns);
void OLED_set_x (unsigned char x,unsigned char Adr);
void OLED_set_XY (unsigned char x,unsigned char y,unsigned char Adr);
void OLEDdrawbar(unsigned char Adress, unsigned char y, int val);
void OLEDdrawcol(unsigned char Adress, unsigned char x, int val);
void PutChar_mid(unsigned char x,unsigned char y,unsigned char dig,unsigned char Adress);
void PutStr_mid(unsigned char x, unsigned char y, const char txt[],unsigned char Adress);
void PutNumber_mid(unsigned char x, unsigned char y, int val,unsigned char Adr);
void OLED_temper_big(unsigned char x, unsigned char y,int val, unsigned char Adr);
void OLED_4_big(unsigned char x, unsigned char y,unsigned int val, unsigned char Adr, unsigned char c);
void OLED_mid(unsigned char x, unsigned char y, int val,unsigned char Adr, unsigned char c);
void OLED_dot_mid(unsigned char x, unsigned char y, unsigned char Adr);
void OLED_doubledot_mid(unsigned char x, unsigned char y, unsigned char Adr);
