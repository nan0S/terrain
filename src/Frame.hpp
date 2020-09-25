#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "Camera.hpp"
#include "Shader.hpp"

struct Frame
{
	GLuint shader;
	GLuint vao;
	GLuint vp_loc;

	int index_count;
	GLuint offset_buffer;
	GLuint index_buffer;

	void init();
	void draw();
	void terminate();

	size_t getVertexCount();
};