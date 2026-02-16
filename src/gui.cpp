#include "data.hpp"
#include "gui.hpp"
#include <fstream>

using json = nlohmann::json;

Button current_screen_buttons[50] = {};

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
    std::cout << "Buttons:" << std::endl;
    uint i = 0;
    for (const auto& json_button : data.items()) {
        Button button;
        button.setPosition(sf::Vector2f(json_button.value()["x"].get<float>(), json_button.value()["y"].get<float>()));
        //button.setSize(sf::Vector2f(json_button.value()["size_x"].get<float>(), json_button.value()["size_y"].get<float>()));
        button.setText(json_button.value()["text"].get<std::string>());
        current_screen_buttons[i] = button;
        i++;
    }
}


