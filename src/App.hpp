#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "Window.hpp"
#include "Common.hpp"
#include "View.hpp"
#include "FPScounter.hpp"

class App
{
private:
	View view;

	FPScounter fps;
	bool auto_lod = true;
	bool updated = false;

	void run();
	void draw();
	void handleInput();
	void printStats();
	void printHelp();
	
public: 
    void start(const std::string& dir, int sz_min, int sz_max,
    								   int dl_min, int dl_max);
};