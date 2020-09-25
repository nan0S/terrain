#include "Common.hpp"

Color Color::RED = {1.0f, 0.0f, 0.0f};
Color Color::BLUE = {0.0f, 0.0f, 1.0f};
Color Color::GREEN = {0.0f, 1.0f, 0.0f};
Color Color::YELLOW = {1.0f, 1.0f, 0.2f};
Color Color::PURPLE = {0.9f, 0.0f, 0.8f};
Color Color::ORANGE = {1.0f, 0.5f, 0.0f};
Color Color::BLACK = {0.0f, 0.0f, 0.0f};
Color Color::NONE = {-1.0f, -1.0f, -1.0f};
Color Color::RAINBOW = {83.1f / 255, 68.6f / 255, 21.6f / 255};
Color Color::DARK_PURPLE = {128.0f / 255, 0.0f, 128.0f / 255};
Color Color::DARK_YELLOW = {150.0f / 255, 150.0f / 255, 0.0f};
Color Color::WHITE = {1.0f, 1.0f, 1.0f};
Color Color::PINK = {255 / 255.0f, 105 / 255.0f, 180 / 255.0f};
Color Color::LIGHT_BLUE = {135 / 255.0f, 206 / 255.0f, 250 / 255.0f};

std::mt19937 Random::gen{std::random_device{}()};

std::vector<Color> Color::colors = {
    BLUE, 
    GREEN, 
    YELLOW, 
    PURPLE, 
    ORANGE,  
    RAINBOW,
    DARK_PURPLE,
    DARK_YELLOW, 
    WHITE
};

Color Color::getRandom()
{
    return colors[Random::random<int>(0, colors.size() - 1)];
}

bool Color::operator==(const Color& color)
{
    return r == color.r && g == color.g && b == color.b;
}

template<typename T>
T Random::random(T min, T max)
{
    using dist = std::conditional_t<
        std::is_integral<T>::value, 
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>
    >;
    return dist{min, max}(gen);
}

GLfloat Random::randnorm()
{
    return random(-1.0f, 1.0f);
}
