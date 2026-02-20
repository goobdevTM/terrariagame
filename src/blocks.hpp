#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <SFML/Graphics.hpp>
#include <string>



const std::string path = "../assets/json/blocks.json";

class Block {
    public:
        std::string name = "Unnamed Block";
        bool empty = false;
        sf::Vector2i atlas_coords = {};
};

extern Block blocks[750];

extern const sf::Texture tex_atlas;
extern std::vector<sf::IntRect> sprite_coords;

extern void load_blocks();



#endif // BLOCKS_HPP
