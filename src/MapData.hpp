#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Common.hpp"
#include "Window.hpp"

struct MapData
{
	GLuint shader;

	GLuint rows_loc;
    GLuint cols_loc;
    float row_size;
    float col_size;

    float left, right;
    float up, down;

    float zoom = 1;
    GLuint zoom_loc;

    glm::vec2 pos = {0, 0};
    float move_speed = 1;
    GLuint pos_loc;

    GLuint mult_loc;

    void init();
    void bind();
    void terminate();
    void clipPos();
};