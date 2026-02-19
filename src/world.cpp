#include "world.hpp"
#include <vector>
#include <unordered_map>
#include <cmath>
#include <functional>

float scale = 4.f;

const float chunks_x = 9.f;
const float chunks_y = 6.f;

std::unordered_map<sf::Vector2i, std::array<uint, chunk_size * chunk_size>, Vector2iHash> world;

const sf::Texture tex_atlas(tex_path + "blocks/dirt.png");

sf::Sprite test_sprite(tex_atlas, sf::IntRect({0, 0}, {16, 16}));

void init_chunk(sf::Vector2i chunk_pos)
{
    if (not world.contains(chunk_pos))
    {
        world[chunk_pos] = {0}; // initialize chunk to air blocks
    }
}

void set_block(sf::Vector2i world_pos, uint block_id)
{
    sf::Vector2i chunk_pos = {int(float(world_pos.x) / 16.f), int(float(world_pos.y) / 16.f)};
    init_chunk(chunk_pos);
    world[chunk_pos][0] = block_id;
}

void generate_chunk(sf::Vector2i chunk_pos)
{
    init_chunk(chunk_pos);
    for (uint i = 0; i < chunk_size * chunk_size; ++i)
    {
        set_block(sf::Vector2i((chunk_pos.x * 16), (chunk_pos.y * 16)), 0);
    }
}

void render_chunk(sf::Vector2i chunk_pos, sf::RenderWindow &window)
{
    for (float y = 0; y < chunk_size; ++y)
    {
        for (float x = 0; x < chunk_size; ++x)
        {
            test_sprite.setScale({scale, scale});
            test_sprite.setPosition({((x * scale * 16) - camera_pos.x) + (chunk_pos.x*(chunk_size*scale*16)),    ((y * scale * 16) - camera_pos.y) + (chunk_pos.y*(chunk_size*scale*16))});
            window.draw(test_sprite);
        }
    }
}