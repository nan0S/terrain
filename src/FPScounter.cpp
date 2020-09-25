#include "FPScounter.hpp"

void FPScounter::start()
{
	frame_counter = 0;
	last = glfwGetTime();
}

bool FPScounter::update()
{
	double now = glfwGetTime();
	++frame_counter;

	if (now - last >= 1)
	{
		frame_rate = frame_counter;
		frame_counter = 0;
		last = now;
		return true;
	}
	return false;
}

int FPScounter::getFrameRate()
{
	return frame_rate;
}