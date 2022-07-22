#include <xc.h>

char GetButton(void)
{
static unsigned char btncnt, key;
if (!RA1)
{
	if (btncnt > 150)
	{
		key = 2;
	}
	else if(++btncnt > 10)
	{
		key = 1;
	}
}
else
{
	if (key == 1)
	{
		key = 0;return 1;
	}
	else if (key == 2)
	{
		key = 0;return 2;
	}
	else btncnt=0;
}
return 0;
}