#include "data.hpp"
#include "blocks.hpp"

bool inventory_open = false;

sf::Texture hotbar_slot_tex(tex_path + "gui/hotbar_slot.png");
sf::Texture hotbar_slot_selected_tex(tex_path + "gui/hotbar_slot_selected.png");
sf::Sprite hotbar_slot(hotbar_slot_tex);
sf::Sprite hotbar_slot_selected(hotbar_slot_selected_tex);
sf::Sprite item_sprite(tex_atlas);
sf::Font item_font = sf::Font("../assets/fonts/Ithaca.ttf");
sf::Text item_text(item_font);

int hotbar[10];
int inventory[40];
int hotbar_counts[10];
int inventory_counts[40];

int selected_slot = 0;
int selected_block_type = 0;

const int max_per_slot = 100;

void init_hotbar_and_inventory() {
    for (int i = 0; i < 10; i++) {
        hotbar[i] = i % amount_of_blocks + 1;
        hotbar_counts[i] = max_per_slot;
    }
    for (int i = 0; i < 40; i++) {
        inventory[i] = i % amount_of_blocks + 1;
        inventory_counts[i] = max_per_slot;
    }
}

void draw_item(sf::Vector2f position, int block_id, int count) {
    if (not block_id == 0) {
        item_sprite.setTextureRect(sf::IntRect({blocks[block_id].atlas_coords.x * 16, (blocks[block_id].atlas_coords.y) * 16}, {16,16}));
        item_sprite.setPosition(position);
        window.draw(item_sprite);
        if (blocks[block_id].has_overlay) {
            item_sprite.setTextureRect(sf::IntRect({blocks[block_id].overlay_atlas_coords.x * 16, (blocks[block_id].overlay_atlas_coords.y) * 16}, {16,16}));
            window.draw(item_sprite);
        }
    }
    item_text.setString(std::to_string(count));
    item_text.setPosition({position.x, position.y + 32});
    window.draw(item_text);
}

void draw_inventory() {
    hotbar_slot.setScale({4,4});
    hotbar_slot_selected.setScale({4,4});
    item_sprite.setScale({4,4});
    item_text.setCharacterSize(32);
    item_text.setOutlineThickness(4);
    int block_id = 0;
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 4; y++) {
            block_id = inventory[x + (y * 10)];
            hotbar_slot.setPosition({960 - ((24*5)*4) + ((x*24)*4), 480 + ((y*24)*4)});
            window.draw(hotbar_slot);
            draw_item({hotbar_slot.getPosition().x + 16, hotbar_slot.getPosition().y + 16}, block_id, inventory_counts[x + (y * 10)]);
        }
    }
}

void draw_hotbar() {
    hotbar_slot.setScale({4,4});
    hotbar_slot_selected.setScale({4,4});
    item_sprite.setScale({4,4});
    int block_id = 0;
    item_text.setCharacterSize(32);
    item_text.setOutlineThickness(4);
    for (int x = 0; x < 10; x++) {
        block_id = hotbar[x];
        hotbar_slot.setPosition({960 - ((24*5)*4) + ((x*24)*4), 960});
        hotbar_slot_selected.setPosition({960 - ((24*5)*4) + ((x*24)*4), 960});
        if (selected_slot == x) {
            window.draw(hotbar_slot_selected);
        } else {
            window.draw(hotbar_slot);
        }
        draw_item({hotbar_slot.getPosition().x + 16, hotbar_slot.getPosition().y + 16}, block_id, hotbar_counts[x]);
    }
}

void open_inventory() {
    paused = true;
}

void close_inventory() {
    paused = false;
}

void reduce_count_in_slot(int slot, int reduce_by) {
    hotbar_counts[selected_slot] -= reduce_by;
    if (hotbar_counts[selected_slot] <= 0) {
        hotbar[selected_slot] = 0;
    }
}

void add_item_to_inventory(int id, int amount) {
    int amount_remaining = amount;
    for (int i = 0; i < 10; i++) {
        if (hotbar[i] == id or hotbar[i] == 0 and hotbar_counts[i] < max_per_slot) {
            hotbar[i] = id;
            hotbar_counts[i] += amount_remaining;
            if (hotbar_counts[i] > max_per_slot) {
                hotbar_counts[i] = max_per_slot;
            }
            amount_remaining -= max_per_slot - hotbar_counts[i];
            if (amount_remaining <= 0) {
                return;
            }
        }
    }
    for (int i = 0; i < 40; i++) {
        if (inventory[i] == id or inventory[i] == 0 and inventory_counts[i] < max_per_slot) {
            inventory[i] = id;
            inventory_counts[i] += amount_remaining;
            if (inventory_counts[i] > max_per_slot) {
                inventory_counts[i] = max_per_slot;
            }
            amount_remaining -= max_per_slot - inventory_counts[i];
            if (amount_remaining <= 0) {
                return;
            }
        }
    }
}

void fix_problems() {
    //if slot is air, make sure count is 0
    for (int i = 0; i < 10; i++) {
        if (hotbar[i] == 0) {
            hotbar_counts[i] = 0;
        }
    }
    for (int i = 0; i < 40; i++) {
        if (inventory[i] == 0) {
            inventory_counts[i] = 0;
        }
    }
}