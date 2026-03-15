#ifndef WORLD_HPP
#define WORLD_HPP

#include "data.hpp" 
#include <functional>

struct Vector2iHash
{
    std::size_t operator()(const sf::Vector2i &v) const
    {
        // Simple hash combination technique
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

extern float scale;

extern const float chunks_x;
extern const float chunks_y;

const int chunk_size = 16;
extern std::unordered_map<sf::Vector2i, std::array<int, chunk_size * chunk_size>, Vector2iHash> world;
extern std::unordered_map<sf::Vector2i, std::array<bool, chunk_size * chunk_size>, Vector2iHash> random_texture_world;

void generate_chunk(sf::Vector2i chunk_pos);

void set_block(sf::Vector2i world_pos, int block_id, bool do_update = true);

int get_block(sf::Vector2i world_pos);

extern void world_and_chunk();
extern void world_and_chunk_b();


extern void render_chunk(sf::Vector2i chunk_pos);

#endif //WORLD_HPP