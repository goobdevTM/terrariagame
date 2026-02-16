#include "data.hpp"

bool mouse_down = false;
bool mouse_just_clicked = false;
sf::Vector2f mouse_pos = {0.f,0.f};
sf::RenderWindow window;
std::string tex_path = "../assets/textures/";
std::string font_path = "../assets/fonts/";

void start() {
    //WINDOW SETUP
    std::cout << "Creating Window \n";
    window = sf::RenderWindow (sf::VideoMode({1920, 1080}), "Game");
    window.setVerticalSyncEnabled(true);
}

void process() {
    mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
    mouse_pos = sf::Vector2f(float(mouse_pos.x) / (float(window.getSize().x) / 1920.0f), float(mouse_pos.y) / (float(window.getSize().y) / 1080.0f));

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