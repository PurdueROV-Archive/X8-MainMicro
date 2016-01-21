

typedef struct {

	Thruster t1;
	Thruster t2;
	Thruster t3;
	Thruster t4;
	Thruster t5;
	Thruster t6;
	Thruster t7;
	Thruster t8;
	// Contains 1/0 depending on whether each motor 1-8 is 'enabled' or 'not disabled.'
	bool enabled[] = {0,0,0,0,0,0,0,0};

} ThrustersContainer;