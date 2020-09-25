#include "Tile.hpp"

namespace fs = std::filesystem;

void Tile::readFromHGT(const std::string& path, 
                       GLshort* heights, unsigned char* buffer,
                       glm::uvec2 toffset, GLuint toffset_loc,
                       GLshort& min, GLshort& max)
{
    std::ifstream file(path, std::fstream::in | std::fstream::binary);

    if (!file.is_open())
    {
        std::cout << fs::path(path).filename() << " doesn't exists!\n";
        corrupted = true;
        return;
    }

    if (!file.read(reinterpret_cast<char*>(buffer), size * size * 2))
    {
        std::cout << "Error while initializing file!\n";
        corrupted = true;
        return;
    }

    file.close();

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
        {
            int idx = i * size + j;
            // GLshort height = (buf[0] << 8 | buf[1]);
            GLshort height = (buffer[2 * idx] << 8 | buffer[2 * idx + 1]);
            heights[idx] = height;

            if (height != INT16_MIN)
            {
                max = std::max(max, height);
                min = std::min(min, height);
            }
        }   

    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
        {
            int idx = i * size + j;
            if (heights[idx] == INT16_MIN)
            {
                int sum = 0;
                int cnt = 0;

                for (int k = 0; k < 4; ++k)
                {   
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni < 0 || ni >= size || nj < 0 || nj >= size)
                        continue;

                    int nidx = ni * size + nj;
                    if (heights[nidx] != INT16_MIN)
                    {
                        sum += heights[nidx];
                        ++cnt;
                    }
                }

                heights[idx] = GLshort(sum / std::max(cnt, 1));
            }
        }

    // glfwMakeContextCurrent(Window::get());

    glGenBuffers(1, &height_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, height_buffer);
    glBufferData(GL_ARRAY_BUFFER, size * size * sizeof(GLshort), heights, GL_STATIC_DRAW);

    this->toffset = toffset;
    this->toffset_loc = toffset_loc;

    std::cout << fs::path(path).filename() << " read!\n";
}

void Tile::draw(int LOD)
{
    if (corrupted)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, height_buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_SHORT, sizeof(GLshort), (void*)0);

    glUniform2i(toffset_loc, toffset.x, toffset.y);

    glDrawElements(GL_TRIANGLES, 6 * ((size - 1) / LOD) * ((size - 1) / LOD), GL_UNSIGNED_INT, (void*)0);
}

Tile::~Tile()
{
    glDeleteBuffers(1, &height_buffer);
}