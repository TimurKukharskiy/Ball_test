class gravity_ball
{
	float x,y,xx,yy,yy2;
public:
	gravity_ball();
	float get_x();
	float get_y();
	void next();
	void reset();
	void inc_x();
	void dec_x();
	void inc_y();
	void dec_y();
};