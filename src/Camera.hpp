#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

class Camera
{
private:
	static glm::mat4 projection;
	static glm::mat4 view;

	static glm::vec3 position;

	static float height;
	static float vert_speed;

	static glm::vec3 direction;
	static glm::vec3 right;
	static glm::vec3 up;

	static float horizontal_angle;
	static float vertical_angle;

	static double last_xpos;
	static double last_ypos;

	static float speed;
	static float mouse_speed;

	static float fov;

public:
	static float min_height;

	static void init();
	static void record();
	static void compute();

	static glm::mat4 getVP();
	static void setPos(glm::vec3 pos);
	static glm::vec3 getPos();

	static void scrollHandler(GLFWwindow* window, double xoff, double yoff);
};