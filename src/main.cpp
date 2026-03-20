#include "data.hpp"
#include "gui.hpp"
#include "world.hpp"
#include "input.hpp"
#include "blocks.hpp"
#include "inventory.hpp"
#include <thread>

Button my_buttons[max_button_array_size] = {};

sf::Text fps_counter(font);
sf::Text debug_text(font);

int hovered_block = 0;

void draw_buttons() {
	for (uint i = 0; i < max_button_array_size; ++i) {
		if (not my_buttons[i].getPosition().x) {break;} //End if this button is null
		Button button = my_buttons[i];
		my_buttons[i].process();
		my_buttons[i].draw(window);
	}
}




int main()
{
	sf::Vector2i last_place_pos = {-100000, -100000};
	fps_counter.setOutlineThickness(4);
	fps_counter.setCharacterSize(32);
	fps_counter.setPosition({8,8});
	debug_text.setOutlineThickness(4);
	debug_text.setCharacterSize(32);
	debug_text.setPosition({8,40});
	player.setOrigin({8,8});
	player.setPosition({960,540});
	player.setScale({scale,scale});
	start();
	load_blocks();
	init_hotbar_and_inventory();
	load_from_json("../assets/json/gui/titlescreen.json");
	for (uint i = 0; i < max_button_array_size; ++i) {
		Button button;
		button.setScale(current_json_buttons[i].getScale());
		button.setSize(current_json_buttons[i].getSize());
		button.setPosition(current_json_buttons[i].getPosition());
		button.setText(current_json_buttons[i].getText(), 16, 2, CENTERED);
		button.setColor(hex_to_color("#2b2fb0ff"));
		button.hover_color = hex_to_color("#2e3fc0ff");
		button.pressed_color = hex_to_color("#231f8aff");
		button.process();
		button.draw(window);
		my_buttons[i] = button;
	}
	std::thread thr1(world_and_chunk);
	while (window.isOpen())
	{
		calculate_delta();
		fps_counter.setString("FPS: " + std::to_string(int(fps)));
		mouse_just_clicked = false;
		right_mouse_just_clicked = false;
		process();
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) //LEFT CLICK
                {
                    mouse_down = true;
					mouse_just_clicked = true;
                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) //RIGHT CLICK
                {
                    right_mouse_down = true;
					right_mouse_just_clicked = true;
                }
            }
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) //LEFT UNCLICK
                {
                    mouse_down = false;
                }
                if (mouseButtonReleased->button == sf::Mouse::Button::Right) //RIGHT UNCLICK
                {
                    right_mouse_down = false;
                }
            }
			if (const auto* mouseScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
				if (mouseScrolled->delta > 0) {
					selected_slot += 1;
				} else if (mouseScrolled->delta < 0)
				{
					selected_slot -= 1;
				}
				if (selected_slot >= 10) {
					selected_slot = 0;
				} else if (selected_slot < 0) {
					selected_slot = 9;
				}
            }
			if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
				//PRESS KEY DOWN
                if (key->code == sf::Keyboard::Key::W)
				{up_pressed = true;}
                if (key->code == sf::Keyboard::Key::S)
				{down_pressed = true;}
                if (key->code == sf::Keyboard::Key::A)
				{left_pressed = true;}
                if (key->code == sf::Keyboard::Key::D)
				{right_pressed = true;}
				if (key->code == sf::Keyboard::Key::E)
				{
					inventory_open = not inventory_open;
					if (inventory_open) {
						open_inventory();
					} else {
						close_inventory();
					}
				}
				if (key->code == sf::Keyboard::Key::Num1 or key->code == sf::Keyboard::Key::Numpad1)
				{selected_slot = 0;}
				if (key->code == sf::Keyboard::Key::Num2 or key->code == sf::Keyboard::Key::Numpad2)
				{selected_slot = 1;}
				if (key->code == sf::Keyboard::Key::Num3 or key->code == sf::Keyboard::Key::Numpad3)
				{selected_slot = 2;}
				if (key->code == sf::Keyboard::Key::Num4 or key->code == sf::Keyboard::Key::Numpad4)
				{selected_slot = 3;}
				if (key->code == sf::Keyboard::Key::Num5 or key->code == sf::Keyboard::Key::Numpad5)
				{selected_slot = 4;}
				if (key->code == sf::Keyboard::Key::Num6 or key->code == sf::Keyboard::Key::Numpad6)
				{selected_slot = 5;}
				if (key->code == sf::Keyboard::Key::Num7 or key->code == sf::Keyboard::Key::Numpad7)
				{selected_slot = 6;}
				if (key->code == sf::Keyboard::Key::Num8 or key->code == sf::Keyboard::Key::Numpad8)
				{selected_slot = 7;}
				if (key->code == sf::Keyboard::Key::Num9 or key->code == sf::Keyboard::Key::Numpad9)
				{selected_slot = 8;}
				if (key->code == sf::Keyboard::Key::Num0 or key->code == sf::Keyboard::Key::Numpad0)
				{selected_slot = 9;}
            }
			if (const auto* key = event->getIf<sf::Event::KeyReleased>())
            {
				//RELEASE KEY UP
                if (key->code == sf::Keyboard::Key::W)
				{up_pressed = false;}
                if (key->code == sf::Keyboard::Key::S)
				{down_pressed = false;}
                if (key->code == sf::Keyboard::Key::A)
				{left_pressed = false;}
                if (key->code == sf::Keyboard::Key::D)
				{right_pressed = false;}
            }
		}
		selected_block_type = hotbar[selected_slot];
		//button.setSize(sf::Vector2f(std::clamp(mouse_pos.x / button.getScale().x, 14.f, 256.f), std::clamp(mouse_pos.y / button.getScale().y, 14.f, 256.f)));
		bool has_all_chunks = true;
		for (int x = 0; x < ceil(chunks_x / float(scale)); ++x) {
			for (int y = 0; y < ceil(chunks_y / float(scale)); ++y) {
				sf::Vector2i camera_chunk_pos = {int(camera_pos.x / (16.f * float(chunk_size) * scale)) + x, int(camera_pos.y / (16.f * float(chunk_size) * scale)) + y};
				if (not world.contains(camera_chunk_pos)) {
					has_all_chunks = false;
					break;
				}
			}
			if (not has_all_chunks) {
				break;
			}
		}
		if (has_all_chunks) { 
			
			
			sf::Vector2i block_cursor_pos = {int(floor((camera_pos.x + mouse_pos.x) / (16.f * scale))), int(floor((camera_pos.y + mouse_pos.y) / (16.f * scale)))};
			hovered_block = get_block(block_cursor_pos);
			bool can_place = (hovered_block == 0 or (selected_block_type == 0 and hovered_block != 0));
			debug_text.setString(\
				"Selected block: " + blocks[selected_block_type].name + " (id: " + std::to_string(selected_block_type) + ")\n" \
				"Hovering block: " + blocks[hovered_block].name + " (id: " + std::to_string(hovered_block) + ")\n" \
				"Can place: " + std::to_string(can_place) + "\n" \
			);
			if (right_mouse_down) {
				if (can_place and not paused) {
					if (((not right_mouse_just_clicked) and not (last_place_pos.x == block_cursor_pos.x and last_place_pos.y == block_cursor_pos.y)) or right_mouse_just_clicked) {
						set_block(block_cursor_pos, selected_block_type);		
						if (gamemode == Gamemodes::SURVIVAL) {
							if (not selected_block_type == 0 ) {
								reduce_count_in_slot(selected_slot, 1);
							}
							last_place_pos = block_cursor_pos;
						}
					}
				}
			}
			if (mouse_down) {
				if (not paused) {
					if (((not mouse_just_clicked) and not (last_place_pos.x == block_cursor_pos.x and last_place_pos.y == block_cursor_pos.y)) or mouse_just_clicked) {
						set_block(block_cursor_pos, 0);		
						if (gamemode == Gamemodes::SURVIVAL) {
							if (not hovered_block == 0) {
								add_item_to_inventory(blocks[hovered_block].drop, 1);
							}
							last_place_pos = block_cursor_pos;
						}
					}
				}
			}
			fix_problems();
			
			//DRAW (IF ALL CHUNKS ARE READY AND ABLE TO DRAW)
			window.clear(hex_to_color("#41a6e9ff"));
			
			
			for (int x = -1; x < ceil(chunks_x / float(scale)) + 1; ++x) {
				for (int y = -1; y < ceil(chunks_y / float(scale)) + 1; ++y) {
					sf::Vector2i camera_chunk_pos = {int(camera_pos.x / (16.f * float(chunk_size) * scale)) + x, int(camera_pos.y / (16.f * float(chunk_size) * scale)) + y};
					render_chunk(camera_chunk_pos);
				}
			}
			window.draw(player);
			window.draw(fps_counter);
			window.draw(debug_text);
			if (inventory_open) {
				draw_inventory();
			}
			draw_hotbar();
			//draw_buttons();
			window.display();
			
		}
	}
}
