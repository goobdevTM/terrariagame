#include "blocks.hpp"
#include "world.hpp"
#include <vector>
#include <unordered_map>
#include <cmath>
#include <thread>



float scale = 4.f;

const float chunks_x = 5.f;
const float chunks_y = 5.f;



std::unordered_map<sf::Vector2i, std::array<int, chunk_size * chunk_size>, Vector2iHash> world;
std::unordered_map<sf::Vector2i, std::array<bool, chunk_size * chunk_size>, Vector2iHash> random_texture_world;




sf::Vector2i get_chunk_pos(sf::Vector2i world_pos) {
    return {int(floor(float(world_pos.x) / 16.f)), int(floor(float(world_pos.y) / 16.f))};
}

sf::Vector2i get_local_pos(sf::Vector2i world_pos, sf::Vector2i chunk_pos) {
    return {world_pos.x - (chunk_pos.x * 16), world_pos.y - (chunk_pos.y * 16)};
}

int xy_to_int(sf::Vector2i i) {
    return i.x + (i.y * chunk_size);
}

void init_chunk(sf::Vector2i chunk_pos)
{
    if (not world.contains(chunk_pos))
    {
        world[chunk_pos] = {0}; // initialize chunk to air blocks
    }
}

void set_block(sf::Vector2i world_pos, int block_id)
{
    sf::Vector2i chunk_pos = get_chunk_pos(world_pos);
    if (world.contains(chunk_pos)) {
        sf::Vector2i local_pos = get_local_pos(world_pos, chunk_pos);
        world[chunk_pos][xy_to_int(local_pos)] = block_id;
        random_texture_world[chunk_pos][xy_to_int(local_pos)] = randi_range(0,1);
    }
}

int get_block(sf::Vector2i world_pos)
{
    sf::Vector2i chunk_pos = get_chunk_pos(world_pos);
    if (not world.contains(chunk_pos))
    {
        return 0;
    }
    sf::Vector2i local_pos = get_local_pos(world_pos, chunk_pos);
    return world[chunk_pos][xy_to_int(local_pos)];
}

bool get_block_randomness(sf::Vector2i world_pos)
{
    sf::Vector2i chunk_pos = get_chunk_pos(world_pos);
    if (not random_texture_world.contains(chunk_pos))
    {
        return 0;
    }
    sf::Vector2i local_pos = get_local_pos(world_pos, chunk_pos);
    return random_texture_world[chunk_pos][xy_to_int(local_pos)];
}

void generate_chunk(sf::Vector2i chunk_pos)
{
    init_chunk(chunk_pos);
    int x = 0;
    int y = 0;
    for (int local_y = 0; local_y < chunk_size; ++local_y)
    {
        for (int local_x = 0; local_x < chunk_size; ++local_x)
        {
            x = local_x + int(chunk_pos.x * chunk_size);
            y = local_y + int(chunk_pos.y * chunk_size);

            /*WORLDGEN
            LOWER Y VALUE = HIGHER
            */

            if (y == 16) {
                set_block({x,y}, 2); //GRASS
            }
            if (y > 16) {
                set_block({x,y}, 1); //DIRT
            }
            if (y > 32) {
                set_block({x,y}, 3); //STONE
            }
            if (y > 256) {
                set_block({x,y}, 4); //DEEP STONE
            }
        }
    }
}

sf::Sprite new_sprite(tex_atlas, sf::IntRect({0,0},{16,16}));
sf::Vector2f sprite_pos = {0.f,0.f};

void world_and_chunk() {
	while (window.isOpen()) {
		for (int x = -3; x < ceil(chunks_x / float(scale)) + 3; ++x) {
			for (int y = -3; y < ceil(chunks_y / float(scale) + 3); ++y) {
				sf::Vector2i camera_chunk_pos = {int(camera_pos.x / (16.f * float(chunk_size) * scale)) + x, int(camera_pos.y / (16.f * float(chunk_size) * scale)) + y};
				if (not world.contains(camera_chunk_pos)) {
					generate_chunk(camera_chunk_pos);
				}
			}
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}


void render_chunk(sf::Vector2i chunk_pos)
{
    sf::Vector2i world_pos = {0,0};
    bool rand_type = false;
    int block_id = 0;
    for (float y = 0; y < chunk_size; ++y)
    {
        for (float x = 0; x < chunk_size; ++x)
        {
            world_pos = {(chunk_pos.x * 16) + int(x), (chunk_pos.y * 16) + int(y)};
            block_id = get_block(world_pos);
            if (not blocks[block_id].empty) {
                sprite_pos = {((x * scale * 16) - camera_pos.x) + (chunk_pos.x*(chunk_size*scale*16)), ((y * scale * 16) - camera_pos.y) + (chunk_pos.y*(chunk_size*scale*16))};
                if (sprite_pos.x >= -scale * 16 and sprite_pos.x <= 1920 + (scale * 16) and sprite_pos.y >= -scale * 16 and sprite_pos.y <= 1080 + (scale * 16))
                {
                    if (blocks[block_id].has_variants) {
                            rand_type = get_block_randomness(world_pos);
                    }
                    new_sprite.setScale({scale + 0.001f, scale + 0.001f});
                    new_sprite.setPosition(sprite_pos);
                    new_sprite.setTextureRect(sf::IntRect({blocks[block_id].atlas_coords.x * 16, (blocks[block_id].atlas_coords.y + rand_type) * 16}, {16,16}));
                    window.draw(new_sprite);
                }
            } 
        }
    }
}
