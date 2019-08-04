
#include "StdAfx.h"
#include <time.h>
#include "Timer.h"

Timer::Timer(){ start = clock(); }

void Timer::reset(){ start = clock(); }

void Timer::setInSeconds(double seconds){ start = clock() - seconds * ((double) CLOCKS_PER_SEC); }

double Timer::ms(){ return (clock() - start) * 1000.0 / ((double) CLOCKS_PER_SEC); }

double Timer::seconds(){ return (clock() - start) / ((double) CLOCKS_PER_SEC); }

double Timer::minutes(){ return (clock() - start) / ((double) CLOCKS_PER_SEC) / 60.0; }

bool Timer::cycle(double s){
	if( seconds() < s )
		return false;
	reset();
	return true;
}