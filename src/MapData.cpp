#include "MapData.hpp"

void MapData::init()
{
	shader = LoadShaders("../shader/mapVertex.glsl", "../shader/mapFragment.glsl");
   	rows_loc = glGetUniformLocation(shader, "rowSize");
    cols_loc = glGetUniformLocation(shader, "colSize");
    zoom_loc = glGetUniformLocation(shader, "zoom");
    pos_loc = glGetUniformLocation(shader, "pos");
    mult_loc = glGetUniformLocation(shader, "mult");
}

void MapData::bind()
{
	glUseProgram(shader);

    glUniform1f(rows_loc, row_size);
    glUniform1f(cols_loc, col_size);
    glUniform1f(zoom_loc, zoom);
    glUniform2f(pos_loc, pos.x, pos.y);
    glUniform2f(mult_loc, 1000.f / Window::getWidth(), 1000.0f / Window::getHeight());
}

void MapData::terminate()
{
	glDeleteProgram(shader);
}

void MapData::clipPos()
{
    float inv = 1 / zoom;
    pos.x = Math::clip(pos.x, left + inv, right - inv);
    pos.y = Math::clip(pos.y, down + inv, up - inv);
}