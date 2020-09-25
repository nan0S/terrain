#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Window.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

class Tile
{
private:
    const int size = 1201;

    GLuint height_buffer;
    GLuint toffset_loc;
    glm::uvec2 toffset;

public:
    bool corrupted = false;

    void readFromHGT(const std::string& dir, 
                     GLshort* heights, unsigned char* buffer,
                     glm::uvec2 offset, GLuint toffset_loc, 
                     GLshort& min, GLshort& max);
    void draw(int LOD);

    ~Tile();
};