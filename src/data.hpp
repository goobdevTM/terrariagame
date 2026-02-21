#ifndef DATA_HPP
#define DATA_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <algorithm>

//Basic data structures, variables and functions

extern bool mouse_down;
extern bool mouse_just_clicked;
extern sf::Vector2f mouse_pos;
extern sf::RenderWindow window;
extern std::string tex_path;
extern std::string font_path;
extern sf::Vector2f camera_pos;
const int max_button_array_size = 10000;

void process();
void start();
sf::Color hex_to_color(std::string hex);
extern uint _rand_calls;
int randi_range(int min, int max);
    

struct AABB
{
    float minX, minY, maxX, maxY;

    bool intersects(AABB other)
    {
        return (minX <= other.maxX && maxX >= other.minX) &&
               (minY <= other.maxY && maxY >= other.minY);
    }
    bool intersects_point(sf::Vector2f point)
    {
        return (point.x >= minX && point.x <= maxX) && (point.y >= minY && point.y <= maxY);
    }
};

#endif //DATA_HPP