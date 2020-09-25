#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Frame.hpp"

struct GlobData
{
	GLuint shader;
	GLuint vp_loc;	
	GLuint mini_loc;

	glm::ivec2 mini;
	Frame frame;

	void init(int sz_min, int dl_min);
	void bind();
	void terminate();
	void drawFrame();
	
	size_t getFrameVertexCount();
};