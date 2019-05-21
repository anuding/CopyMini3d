#pragma once
#include <time.h>

struct Timer
{
	clock_t nowtime=0;
	clock_t tick() {
		clock_t delta = clock() - nowtime;
		nowtime = clock();
		return delta; 
	}
};