#include "View.hpp"

void View::init(const std::string& dir, int sz_min, int sz_max,
                                 		int dl_min, int dl_max)
{
    int rows = (sz_max - sz_min);
    int cols = (dl_max - dl_min);
    tile_count = rows * cols; 

    // map
    map.init();

    // vertex array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    auto getPath = [](const std::string& dir, int sz, int dl) {
        std::string a("N");
        std::string b("E");

        if (sz < 0)
        {
            a = "S";
            sz = -sz;
        }
        if (dl < 0)
        {
            b = "W";
            dl = -dl;
        }

        std::string n1 = std::to_string(sz);
        if (sz < 10)
            n1 = "0" + n1;

        std::string n2 = std::to_string(dl);
        if (dl < 10)
            n2 = "0" + n2;
        if (dl < 100)
            n2 = "0" + n2;

        return dir + (a + n1 + b + n2) + ".hgt";
    };

    // tiles
    tiles = new Tile[tile_count];      
    GLuint toffset_loc = glGetUniformLocation(map.shader, "tOffset");
    GLshort* heights = new GLshort[size * size];
    unsigned char* buffer = new unsigned char[2 * size * size];

    GLshort min_height = INT16_MAX;
    GLshort max_height = INT16_MIN;

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            int idx = i * cols + j;
            std::string path = getPath(dir, i + sz_min, j + dl_min);

            tiles[idx].readFromHGT(path, heights, buffer,
                                   {i, j}, toffset_loc, 
                                   min_height, max_height);
            if (!tiles[idx].corrupted)
                ++correct_tiles;
        }

    delete[] heights;
    delete[] buffer;

    auto delta = (std::chrono::high_resolution_clock::now() - start);
    float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000.f;
    std::cout << tile_count << " files read in " << elapsed << "s!\n\n";

    // int max_threads = std::min(8, tile_count);
    // std::thread threads[max_threads];
    // tiles = new Tile[tile_count];
    // GLuint toffset_loc = glGetUniformLocation(map.shader, "tOffset");

    // GLshort* heights[max_threads];
    // for (int i = 0; i < max_threads; ++i)
    //     heights[i] = new GLshort[size * size];

    // unsigned char* buffers[max_threads];
    // for (int i = 0; i < max_threads; ++i)
    //     buffers[i] = new unsigned char[size * size * 2];

    // GLshort min_height = INT16_MAX;
    // GLshort max_height = INT16_MIN;

    // int t = 0;

    // std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    // for (int i = 0; i < rows; ++i)
    //     for (int j = 0; j < cols; ++j)
    //     {
    //         int idx = i * cols + j;
    //         std::string path = getPath(dir, i + sz_min, j + dl_min);

    //         if (threads[t].joinable())
    //             threads[t].join();

    //         threads[t] = std::thread(&Tile::readFromHGT, &tiles[idx], 
    //                                  path, heights[t], buffers[t],
    //                                  glm::uvec2(i, j), toffset_loc, 
    //                                  std::ref(min_height), std::ref(max_height));

    //         if (++t == max_threads)
    //             t = 0;
    //     }

    // for (int i = 0; i < max_threads; ++i)
    // {
    //     if (threads[i].joinable())
    //         threads[i].join();
    //     delete[] heights[i];
    //     delete[] buffers[i];
    // }

    // auto delta = (std::chrono::high_resolution_clock::now() - start);
    // float elapsed = std::chrono::duration<std::chrono::milliseconds>(delta).count() / 1000.f;
    // std::cout << tile_count << " files read in " << elapsed << "s!\n\n";


    Camera::min_height = 6371.f + (1.f * min_height + 3.f * max_height) / 4.f * 0.001f;

    typedef glm::mediump_i16vec2 ivec2;
    
    // ids buffer   
    ivec2* ids = new ivec2[size * size];
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            ids[i * size + j] = ivec2(i, j);

    glGenBuffers(1, &ids_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, ids_buffer);
    glBufferData(GL_ARRAY_BUFFER, size * size * sizeof(ivec2), ids, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 2, GL_SHORT, sizeof(ivec2), (void*)0);
    delete[] ids;

    // index buffer    
    glGenBuffers(LOD_cnt, index_buffers);
    GLuint* indices = new GLuint[(size - 1) * (size - 1) * 2 * 3];
    for (int t = 0; t < LOD_cnt; ++t)
    {
        int lod = LOD_vals[t];
        int lod_size = (size - 1) / lod;

        for (int i = 0; i < lod_size; ++i)
            for (int j = 0; j < lod_size; ++j)
            {
                int idx = 6 * (i * lod_size + j);
                int idx2 = i * lod * size + j * lod;

                indices[idx + 0] = idx2;
                indices[idx + 1] = idx2 + lod * size;
                indices[idx + 2] = idx2 + lod;

                indices[idx + 3] = idx2 + lod;
                indices[idx + 4] = idx2 + size * lod;
                indices[idx + 5] = idx2 + size * lod + lod;
            }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffers[t]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * lod_size * lod_size * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }
    delete[] indices;
    
    // map
    float angle = glm::radians((sz_min + sz_max) * 0.5f);
    map.col_size = glm::cos(angle);
    map.row_size = 1.0f;
    
    float row_mult = 2.0f / (rows * map.row_size);
    float col_mult = 2.0f / (cols * map.col_size);
    float mult = std::min(row_mult, col_mult);

    map.row_size *= mult;
    map.col_size *= mult;

    float dx = cols * map.col_size * 0.5f;
    float dy = rows * map.row_size * 0.5f;
    map.pos = {-1 + dx, -1 + dy};

    map.left = map.pos.x - 1;
    map.right = map.pos.x + 1;
    map.down = map.pos.y - 1;
    map.up = map.pos.y + 1;

    // glob

    float lat = glm::radians((sz_max + sz_min) * 0.5f);
    float lon = glm::radians((dl_max + dl_min) * 0.5f);

    float y = glm::sin(lat);
    float x = glm::cos(lat) * glm::sin(lon);
    float z = glm::cos(lat) * glm::cos(lon);

    glm::vec3 pos = glm::vec3(x, y, z);
    Camera::setPos(pos);

    glob.init(sz_min, dl_min);

    changeLOD(1, false);
}

void View::draw()
{       
    if (mode == Mode::GLOB)
        glob.drawFrame();

    glBindVertexArray(vao);  

    if (mode == Mode::MAP)
        map.bind();
    else
        glob.bind();

    for (int i = 0; i < tile_count; ++i)
        tiles[i].draw(LOD);

    glBindVertexArray(0);
}

void View::terminate()
{
    delete[] tiles;

    glDeleteBuffers(1, &ids_buffer);
    glDeleteBuffers(10, index_buffers);

    map.terminate();
    glob.terminate();

    glDeleteVertexArrays(1, &vao);
}

void View::changeLOD(int LOD, bool print)
{
    if (LOD < 1 || LOD > LOD_cnt)
        return;

    if (this->LOD != LOD)
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffers[LOD - 1]);

	   if (print) {
		   if (LOD <= 9)
		   	std::cout << "\nLevel of Detail: " << 10 - LOD << "\n\n";
		   else
		   {
			   if (this->LOD < LOD)
				   std::cout << "Decreased Level of Detail\n\n";
			   else
				   std::cout << "Increased Level of Detail\n\n";
		   }
	   }
	   this->LOD = LOD;
    }
}

int View::getLOD()
{
    return LOD;
}

size_t View::getTriangleCount()
{
    size_t lod = LOD_vals[LOD - 1];
    return ((size - 1) / lod) * ((size - 1) / lod) * 2 * correct_tiles;
}

size_t View::getVertexCount()
{
    size_t res = getTriangleCount() * 3;
    if (mode == Mode::GLOB)
        res += glob.getFrameVertexCount();
    return res;
}

void View::handleControls()
{
    static double last_time = glfwGetTime();
    double now = glfwGetTime();
    double delta_time = now - last_time;
    last_time = now;

    if (mode == Mode::MAP)
    {
        if (Window::pressed(GLFW_KEY_Z))
            map.zoom *= std::min(1.1, 1.01 + delta_time);
        if (Window::pressed(GLFW_KEY_X))
            map.zoom *= std::max(0.9, 0.99 - delta_time);

        map.zoom = std::max(1.0f, map.zoom);

        float delta = map.move_speed / map.zoom * delta_time;

        if (Window::pressed(GLFW_KEY_A) || Window::pressed(GLFW_KEY_LEFT))
            map.pos.x -= delta;
        if (Window::pressed(GLFW_KEY_D) || Window::pressed(GLFW_KEY_RIGHT))
            map.pos.x += delta;
        if (Window::pressed(GLFW_KEY_W) || Window::pressed(GLFW_KEY_UP))
            map.pos.y += delta;
        if (Window::pressed(GLFW_KEY_S) || Window::pressed(GLFW_KEY_DOWN))
            map.pos.y -= delta;        

        map.clipPos();
    }
    else
        Camera::compute();
}

void View::switchToMode(Mode mode)
{
    if (this->mode == Mode::MAP && mode == Mode::GLOB)
        Camera::record();

    if (this->mode != mode)
    {
        if (mode == Mode::MAP)
            std::cout << "\nSwitching to map mode\n\n";
        else
            std::cout << "\nSwitching to 3D mode\n\n";
    }

    this->mode = mode;
}
