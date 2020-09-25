#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>

// #include <thread>
#include <chrono>

#include "Common.hpp"
#include "Shader.hpp"
#include "Tile.hpp"
#include "Window.hpp"
#include "MapData.hpp"
#include "GlobData.hpp"
#include "Camera.hpp"

class View
{
public:
    enum Mode
    {
        MAP, 
        GLOB
    };

private:
    const int size = 1201;

    Tile* tiles;
    int tile_count;
    int correct_tiles = 0;

    const int LOD_vals[12] = {1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 30};
    const int LOD_cnt = 12;
    int LOD;

    GLuint vao;
    GLuint ids_buffer;
    GLuint index_buffers[12];

    MapData map;
    GlobData glob;
    Mode mode = Mode::MAP;

    GLuint mult_loc;

public:
    void init(const std::string& dir, int sz_min, int sz_max,
                                      int dl_min, int dl_max);
    void draw();
    void terminate();

    void changeLOD(int LOD, bool print = true);
    void switchToMode(Mode mode);

    size_t getTriangleCount();
    size_t getVertexCount();
    int getLOD();

    void handleControls();
};