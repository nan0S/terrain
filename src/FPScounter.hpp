#include <GLFW/glfw3.h>

class FPScounter
{
private:
	double last;
	int frame_counter;
	int frame_rate = 30;
	
public:
	void start();
	bool update();
	int getFrameRate();
};