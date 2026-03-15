#include "world.hpp"
#include "blocks.hpp"
#include "block_updates.hpp"

void update_block(sf::Vector2i world_pos, bool update_neighbors, int event) {
    int block_id = get_block(world_pos);
    int new_event = event;
    int neighbors[8] = {0};
    int idx = 0;
    int leaf_neighbor = 0;
    bool leaf_good = false;
    for (int x = -1; x < 2; ++x) {
        for (int y = -1; y < 2; ++y) {
            if (not (x == 0 and y == 0)) {
                neighbors[idx] = get_block({world_pos.x + x, world_pos.y + y});
                idx += 1;
            }
        }            
    }
    
    Block block_data = blocks[block_id];
    switch (block_id)
    {
    case 1: //dirt
        if (blocks[get_block({world_pos.x, world_pos.y - 1})].empty) { //block above is air or equivalent
            set_block(world_pos, 2, false); //replace self with grass
        }
        break;
    case 2: //grass
        if (not blocks[get_block({world_pos.x, world_pos.y - 1})].empty) { //block above isnt air or equivalent
            set_block(world_pos, 1, false); //replace self with dirt
        }
        break;
    case 5: //wood log
        if (blocks[get_block({world_pos.x, world_pos.y + 1})].empty) { //block below is air or equivalent
            new_event = update_events::WOOD_LOG_BREAK;
            set_block(world_pos, 0, false); //break self
            update_block({world_pos.x, world_pos.y - 1}, true, new_event); //update wood log block above
        }
        break;
    case 6: //leaves
        for (int neighbor : neighbors) {
            if (neighbor == 5) {
                leaf_neighbor = 2;
            } else if (neighbor == 6) {
                if (leaf_neighbor == 0) {
                    leaf_neighbor = 1;
                }
            }
        }
        if (event == update_events::WOOD_LOG_BREAK and leaf_neighbor == 2) {
            leaf_good = true;
            std::cout << world_pos.x << ", " << world_pos.y << "\n";
        } 
        else if (event != update_events::WOOD_LOG_BREAK and leaf_neighbor > 0) {
            leaf_good = true;
        }
        
        if (not leaf_good) {
            new_event = update_events::WOOD_LOG_BREAK;
            set_block(world_pos, 0, false);
            for (int x = -1; x < 2; ++x) {
                for (int y = -1; y < 2; ++y) {
                    if (not (x == 0 and y == 0)) {
                        if (get_block({world_pos.x + x, world_pos.y + y}) == 6) {
                            update_block({world_pos.x + x, world_pos.y + y}, false, update_events::WOOD_LOG_BREAK);
                        }
                    }
                }            
            }
        }
        break;
    default:
        break;
    }

    if (update_neighbors) {
        for (int x = -1; x < 2; ++x) {
            for (int y = -1; y < 2; ++y) {
                if (not (x == 0 and y == 0)) {
                    update_block({world_pos.x + x, world_pos.y + y}, false, new_event);
                }
            }            
        }
    }
    
}