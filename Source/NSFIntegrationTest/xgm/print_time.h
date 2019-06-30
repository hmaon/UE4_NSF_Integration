#pragma once

#include <stdio.h>

static char* print_time(int time)
{
	static char buf[32];
	int h, m, s, ss = 0;

	if (time < 0)
		return "";

	ss = (time % 1000) / 10;
	time /= 1000;
	s = time % 60;
	time /= 60;
	m = time % 60;
	time /= 60;
	h = time;

	if (h)
		sprintf(buf, "%02d:%02d:%02d", h, m, s);
	else
		sprintf(buf, "%02d:%02d", m, s);

	return buf;
}
