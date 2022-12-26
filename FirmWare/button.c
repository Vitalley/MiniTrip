#include <xc.h>

char GetButton(void)
{
static unsigned char btncnt, key;
if (PORTAbits.RA1 == 0)
{
	if (btncnt > 250)
	{
		key = 2;//return 2;
	}
	else if(++btncnt > 3)
	{
		key = 1;
	}
}
else
{
	btncnt=0;
	if (key == 1)
	{
		key = 0;return 1;
	}
	else if (key == 2)
	{
		key = 0;return 2;
	}

}
return 0;
}