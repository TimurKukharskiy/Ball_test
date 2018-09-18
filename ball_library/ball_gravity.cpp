#include "ball_library_h.h"
#include <Windows.h>
gravity_ball::gravity_ball()
{
	x=100;y=485;xx=3.5;yy=-9;yy2=yy;
}
float gravity_ball::get_x()
{
	return x;
}
float gravity_ball::get_y()
{
	return y;
}
void gravity_ball::next()
{
	x+=xx;y+=yy2;yy2+=0.098f;
	if (y>485)y=485;
}
void gravity_ball::reset()
{
	if(y==485){x=100;yy2=yy;Sleep(1000);}
}
void gravity_ball::inc_x()
{
	xx+=.3f;
}
void gravity_ball::dec_x()
{
	xx-=.3f;
}
void gravity_ball::inc_y()
{
	yy+=.3f;
}
void gravity_ball::dec_y()
{
	yy-=.3f;
}
