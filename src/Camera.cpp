#include "Camera.hpp"

glm::mat4 Camera::projection = glm::mat4();
glm::mat4 Camera::view = glm::mat4();

glm::vec3 Camera::position = glm::vec3(0, 0, 1);

float Camera::height = 6372.f;
float Camera::vert_speed = 0.1f;
float Camera::min_height = 6371.f;
const float scale = 0.001f;

glm::vec3 Camera::direction = glm::vec3(0, 0, -1);
glm::vec3 Camera::right = glm::vec3(-1, 0, 0);
glm::vec3 Camera::up = glm::vec3(0, 1, 0);

float Camera::vertical_angle = 0;
float Camera::horizontal_angle = 0;

double Camera::last_xpos = 0;
double Camera::last_ypos = 0;

float Camera::fov = 60;
float Camera::speed = 12.0f;
float Camera::mouse_speed = 0.005f;

void Camera::init()
{
	glfwSetScrollCallback(Window::get(), scrollHandler);
}

void Camera::record()
{
	glfwGetCursorPos(Window::get(), &last_xpos, &last_ypos);
}

void Camera::compute()
{
	static double last_time = glfwGetTime();
	double current_time = glfwGetTime();
	float delta_time = float(current_time - last_time);
	last_time = current_time;

	double xpos, ypos;
	glfwGetCursorPos(Window::get(), &xpos, &ypos);	

	float dh = mouse_speed * float(last_xpos - xpos);
	float dv = mouse_speed * float(last_ypos - ypos);

	horizontal_angle += dh;
	vertical_angle += dv;

	vertical_angle = glm::clamp(vertical_angle, -float(M_PI / 2.2f), float(M_PI / 2.2f));

	last_xpos = xpos;
	last_ypos = ypos;

	if (Window::pressed(GLFW_KEY_U))
		vert_speed *= 1.05f;
	if (Window::pressed(GLFW_KEY_J))
		vert_speed *= 0.95f;

	glm::vec3 delta_pos(0);
	if (Window::pressed(GLFW_KEY_W) || Window::pressed(GLFW_KEY_UP))
        delta_pos += direction;
    if (Window::pressed(GLFW_KEY_S) || Window::pressed(GLFW_KEY_DOWN))
        delta_pos -= direction;
    if (Window::pressed(GLFW_KEY_D) || Window::pressed(GLFW_KEY_RIGHT))
        delta_pos += right;
    if (Window::pressed(GLFW_KEY_A) || Window::pressed(GLFW_KEY_LEFT))
        delta_pos -= right;
    if (Window::pressed(GLFW_KEY_SPACE))
    	height *= std::min(1.1f, 1.f + delta_time / 200 * vert_speed);
    if (Window::pressed(GLFW_KEY_LEFT_ALT))
    	height *= std::max(0.9f, 1.f - delta_time / 200 * vert_speed);

	const static float radius = 6371;
	const float min_dist = (height - radius) * scale * 0.2f;
	const float max_dist = glm::sqrt(height * height - radius * radius) * scale + 0.09f;

   	height = glm::max(min_height, height);
    position += delta_pos * delta_time * speed * scale;
	glm::vec3 npos = glm::normalize(position);
	position = npos  * height * scale;    

	up = npos;
	right = glm::normalize(glm::cross(direction, up));

	glm::mat4 m(1.f);
	glm::mat4 r = glm::rotate(m, dh, position);
	up = glm::vec3(r * glm::rotate(m, vertical_angle, right) * glm::vec4(up, 0.f));
	right = glm::vec3(r * glm::vec4(right, 0.f));
	direction = glm::cross(up, right);

    projection = glm::perspective(glm::radians(fov), 
    							  Window::getAspectRatio(), 
    							  min_dist,
    							  max_dist);
    view = glm::lookAt(
    	position, 
    	position + direction, 
    	up
    );
}

glm::mat4 Camera::getVP() { return projection * view; }

void Camera::setPos(glm::vec3 pos)
{
	position = glm::normalize(pos) * height * scale;
   	up = glm::normalize(position);
	if (up.y == 0)
		direction = glm::vec3(0, 1, 0);
	else
		direction = -glm::normalize(glm::vec3(up.x, (-up.x * up.x - up.z * up.z) / up.y, up.z));
	right = glm::cross(direction, up);
}

void Camera::scrollHandler(GLFWwindow* window, double xoff, double yoff)
{
	if (yoff > 0) speed *= 1.05f;
	else 		  speed *= 0.95f;
}

glm::vec3 Camera::getPos()
{
	return position;
}
