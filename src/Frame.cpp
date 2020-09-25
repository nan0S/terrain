#include "Frame.hpp"

void Frame::init()
{
	// vao and shader
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	shader = LoadShaders("../shader/frameVertex.glsl", "../shader/frameFragment.glsl");
	vp_loc = glGetUniformLocation(shader, "vp");

	// offsets
	glm::vec2* offsets = new glm::vec2[181 * 361];
	float lat_step = M_PI / 180.f;
	float lon_step = 2 * M_PI / 360.f;

	for (int i = 0; i <= 180; ++i)
	{
		float lat = M_PI_2 - i * lat_step;
		for (int j = 0; j <= 360; ++j)
		{
			float lon = j * lon_step; 
			int idx = i * 361 + j;
			offsets[idx] = {lat, lon};
		}
	}

	glGenBuffers(1, &offset_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, offset_buffer);
	glBufferData(GL_ARRAY_BUFFER, 181 * 361 * sizeof(glm::vec2), offsets, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	delete[] offsets;

	// indices
	std::vector<GLushort> indices;
	for (int i = 0; i < 180; ++i)
	{
		GLushort k1 = i * 361;
		GLushort k2 = k1 + 361;

		for (int j = 0; j < 360; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k1 + 1);
			}

			indices.push_back(k1);
			indices.push_back(k2);
		}
	}
	index_count = indices.size();

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
}	

void Frame::draw()
{
	glBindVertexArray(vao);
	glUseProgram(shader);

	glm::mat4 vp = Camera::getVP();
	glUniformMatrix4fv(vp_loc, 1, GL_FALSE, &vp[0][0]);

	glDrawElements(GL_LINES, index_count, GL_UNSIGNED_SHORT, (void*)0);

	glBindVertexArray(0);
}

void Frame::terminate()
{
	glDeleteBuffers(1, &offset_buffer);
	glDeleteBuffers(1, &index_buffer);
	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &vao);
}

size_t Frame::getVertexCount()
{
	return index_count;
}