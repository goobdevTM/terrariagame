#ifndef BLOCK_UPDATES_HPP
#define BLOCK_UPDATES_HPP

#include <SFML/Graphics.hpp>

enum update_events {
    WOOD_LOG_BREAK = 1,
};


void update_block(sf::Vector2i world_pos, bool update_neighbors = false, int event = 0);

#endif //BLOCK_UPDATES_HPP