#include "GlobData.hpp"

void GlobData::init(int sz_min, int dl_min)
{
	shader = LoadShaders("../shader/globVertex.glsl", "../shader/globFragment.glsl");
	vp_loc = glGetUniformLocation(shader, "vp");
	mini_loc = glGetUniformLocation(shader, "mini");
	mini = {sz_min, dl_min};
	Camera::init();
	frame.init();
}

void GlobData::bind()
{
	glUseProgram(shader);

	glm::mat4 vp = Camera::getVP();
	glUniformMatrix4fv(vp_loc, 1, GL_FALSE, &vp[0][0]);

	glUniform2i(mini_loc, mini.x, mini.y);
}

void GlobData::terminate()
{
	glDeleteProgram(shader);
	frame.terminate();
}

void GlobData::drawFrame()
{
	frame.draw();	
}

size_t GlobData::getFrameVertexCount()
{
	return frame.getVertexCount();
}