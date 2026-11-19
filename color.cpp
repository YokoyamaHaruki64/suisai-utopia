#include <iostream>
#include "color.h"
#include "define.h"

COLOR RandomColor()
{
	COLOR ret;
	ret.nRed = RandomRange(0,255);
	ret.nGreen = RandomRange(0,255);
	ret.nBlue = RandomRange(0,255);

	return ret;
}

COLOR RandomColorBullet()
{
	COLOR ret;
	ret.nRed = RandomRange(50,180);
	ret.nGreen = RandomRange(50,200);
	ret.nBlue = RandomRange(200,220);

	return ret;
}