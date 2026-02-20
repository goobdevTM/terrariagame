#include "data.hpp"
#include "gui.hpp"
#include <fstream>
#include <vector>

sf::Font font = sf::Font("../assets/fonts/Ithaca.ttf");

using json = nlohmann::json;

Button current_json_buttons[max_button_array_size] = {};

void load_from_json(std::string path) {
    // Open the JSON file
    std::ifstream f(path);

    if (!f.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
    }

    // Parse the file content directly into a json object
    json data = json::parse(f);

    // Close the file (optional, as the ifstream destructor will close it)
    f.close();


    // Iterate over an array
    uint i = 0;
    for (const auto& json_button : data.items()) {
        Button button;
        button.setScale(sf::Vector2f(json_button.value()["scale"].get<float>(), json_button.value()["scale"].get<float>()), true);
        button.setSize(sf::Vector2f(json_button.value()["size_x"].get<float>(), json_button.value()["size_y"].get<float>()));
        button.setText(json_button.value()["text"].get<std::string>());
        button.setPosition(sf::Vector2f(json_button.value()["x"].get<float>(), json_button.value()["y"].get<float>()), true);
        
        button.setColor(hex_to_color("#2b2fb0ff"));
        button.hover_color = hex_to_color("#2e3fc0ff");
        button.pressed_color = hex_to_color("#231f8aff");
        button.hover_color = sf::Color::White;

        current_json_buttons[i] = button;
        i++;
    }
}


