#include "data.hpp"
#include "gui.hpp"
#include "world.hpp"
#include "input.hpp"
#include "blocks.hpp"
#include <thread>

Button my_buttons[max_button_array_size] = {};

sf::Text fps_counter(font);

void draw_buttons() {
	for (uint i = 0; i < max_button_array_size; ++i) {
		if (not my_buttons[i].getPosition().x) {break;} //End if this button is null
		Button button = my_buttons[i];
		my_buttons[i].process();
		my_buttons[i].draw(window);
	}
}

void world_and_chunk() {
	while (window.isOpen()) {
		for (int x = -3; x < ceil(chunks_x / float(scale)) + 3; ++x) {
			for (int y = -3; y < ceil(chunks_y / float(scale)) + 3; ++y) {
				sf::Vector2i camera_chunk_pos = {int(camera_pos.x / (16.f * float(chunk_size) * scale)) + x, int(camera_pos.y / (16.f * float(chunk_size) * scale)) + y};
				if (not world.contains(camera_chunk_pos)) {
					generate_chunk(camera_chunk_pos);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int main()
{
	fps_counter.setOutlineThickness(4);
	fps_counter.setCharacterSize(32);
	fps_counter.setPosition({8,8});
	start();
	load_blocks();
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
		fps_counter.setString(std::to_string(int(fps)));
		mouse_just_clicked = false;
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
            }
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) //LEFT CLICK
                {
                    mouse_down = false;
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
			//DRAW (IF ALL CHUNKS ARE READY AND ABLE TO DRAW)
			window.clear(hex_to_color("#41a6e9ff"));
			
			//draw_buttons();
			for (int x = -1; x < ceil(chunks_x / float(scale)) + 1; ++x) {
				for (int y = -1; y < ceil(chunks_y / float(scale)) + 1; ++y) {
					sf::Vector2i camera_chunk_pos = {int(camera_pos.x / (16.f * float(chunk_size) * scale)) + x, int(camera_pos.y / (16.f * float(chunk_size) * scale)) + y};
					render_chunk(camera_chunk_pos, window);
				}
			}
			window.draw(fps_counter);
			window.display();
		}
	}
}
