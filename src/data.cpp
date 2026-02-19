#include "data.hpp"
#include "delta.hpp"
#include "input.hpp"
#include <cmath>

bool mouse_down = false;
bool mouse_just_clicked = false;
sf::Vector2f mouse_pos = {0.f,0.f};
sf::RenderWindow window;
std::string tex_path = "../assets/textures/";
std::string font_path = "../assets/fonts/";
sf::Vector2f camera_pos = {0.f,0.f};
sf::Vector2f input_dir = {0.f,0.f};
const float camera_speed = 1024.f;
sf::Vector2f target_camera_pos = {0.f,0.f};

void start() {
    //WINDOW SETUP
    std::cout << "Creating Window \n";
    window = sf::RenderWindow (sf::VideoMode({1920, 1080}), "Game");
    //window.setVerticalSyncEnabled(true);
}

void process() {
    mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
    mouse_pos = sf::Vector2f(float(mouse_pos.x) / (float(window.getSize().x) / 1920.0f), float(mouse_pos.y) / (float(window.getSize().y) / 1080.0f));

    input_dir = {float(right_pressed) - float(left_pressed), float(down_pressed) - float(up_pressed)};
    target_camera_pos += {input_dir.x * camera_speed * delta, input_dir.y * camera_speed * delta};
    camera_pos = {float(std::lerp(camera_pos.x, target_camera_pos.x, delta * 20.0)), float(std::lerp(camera_pos.y, target_camera_pos.y, delta * 20.0))};
}

sf::Color hex_to_color(std::string hex) {
    int r = std::stoi(hex.substr(1,2), nullptr, 16);
    int g = std::stoi(hex.substr(3,2), nullptr, 16);
    int b = std::stoi(hex.substr(5,2), nullptr, 16);
    int a = 255;
    if (hex.length() > 6) {
        a = std::stoi(hex.substr(7,2), nullptr, 16);
    }
    return sf::Color(r,g,b,a);
}