#pragma once

#include <time.h>

class Timer{
clock_t start;

public:
	Timer();
	void reset();
	void setInSeconds(double seconds);
	double ms();
	double seconds();
	double minutes();
	bool cycle(double seconds);
	operator double() { return seconds(); }
};

