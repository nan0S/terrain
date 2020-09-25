#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <iostream>
#include <vector>

struct Color
{
    GLfloat r, g, b, a = 1.0f;

    static Color RED;
    static Color BLUE;
    static Color GREEN;
    static Color YELLOW;
    static Color PURPLE;
    static Color ORANGE;
    static Color BLACK;
    static Color NONE;
    static Color RAINBOW;
    static Color DARK_PURPLE;
    static Color DARK_YELLOW;
    static Color WHITE;
    static Color PINK;
    static Color LIGHT_BLUE;

    static std::vector<Color> colors;

    static Color getRandom();
    bool operator== (const Color &color);
};

class Random
{
private:
    static std::mt19937 gen;

public:
    template<typename T>
    static T random(T min = 0, T max = 1);
    static GLfloat randnorm();
};

class Math
{
public:
    template<typename T>
    static T constrain(T val, T val_min, T val_max, T targ_min, T targ_max)
    {
        T val_len = val_max - val_min;
        T ratio = (val - val_min) / val_len;
        return targ_min + (targ_max - targ_min) * ratio;
    }

    template<typename T>
    static T clip(T val, T min, T max)
    {
        return std::min(std::max(min, val), max);
    }
};


// std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
// {
//     out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
//     return out;
// }

// std::ostream& operator<<(std::ostream& out, const glm::vec2& v)
// {
//     out << "(" << v.x << ", " << v.y << ")";
//     return out;
// }

// std::ostream& operator<<(std::ostream& out, const glm::vec4& v)
// {
//     out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
//     return out;
// }

// std::ostream& operator<<(std::ostream& out, const glm::mat4& m)
// {
//     for (int i = 0; i < 4; ++i)
//     {
//         for (int j = 0; j < 4; ++j)
//             out << m[j][i] << " ";
//         out << "\n";
//     }
//     return out;
// }