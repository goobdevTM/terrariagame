#include "blocks.hpp"
#include "data.hpp"
#include <fstream>
#include <vector>
#include "../include/json.hpp"
#include <iostream>
#include <array>

using json = nlohmann::json;

Block blocks[750] = {};

const sf::Texture tex_atlas(tex_path + "blocks/atlas.png");


void load_blocks() {
    // Open the JSON file
    std::ifstream f(path);

    if (!f.is_open()) {
        std::cerr << "Error: Could not open blocks.json." << std::endl;
    }

    // Parse the file content directly into a json object
    json data = json::parse(f);

    // Close the file (optional, as the ifstream destructor will close it)
    f.close();


    // Iterate over an array
    std::cout << "Blocks: \n";
    uint i = 0;
    for (const auto& json_block : data["blocks"].items()) {
        Block new_block;
        new_block.name = json_block.value()["name"].get<std::string>();
        if (json_block.value().contains("empty")) {
            new_block.empty = json_block.value()["empty"].get<bool>();
        }
        
        if (not new_block.empty) {
            new_block.atlas_coords = {json_block.value()["atlas_x"].get<int>(), json_block.value()["atlas_y"].get<int>()};
            if (json_block.value().contains("has_variants")) {
                new_block.has_variants = json_block.value()["has_variants"].get<bool>();
            }
        }

        blocks[i] = new_block;
        i++;
    }
}
