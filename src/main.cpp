#include "data.hpp"
#include "gui.hpp"


int main()
{
	Button button;
	button.setPosition({25.f, 25.f});
	button.setScale({4.f,4.f});
	button.setSize({128.f, 40.f});
	button.setPosition({960.f, 600.f});
	button.setText("Play", 32, 2, CENTERED);
	button.setColor(hex_to_color("#2b2fb0ff"));
	button.hover_color = hex_to_color("#2e3fc0ff");
	button.pressed_color = hex_to_color("#231f8aff");
	//button.hover_color = sf::Color::White;

	start();
	load_from_json("../assets/json/gui/titlescreen.json");
	
	while (window.isOpen())
	{
		calculate_delta();
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
			
		}
		
		//button.setSize(sf::Vector2f(std::clamp(mouse_pos.x / button.getScale().x, 14.f, 256.f), std::clamp(mouse_pos.y / button.getScale().y, 14.f, 256.f)));
		window.clear(hex_to_color("#41a6e9ff"));
		button.process();
		button.draw(window);
		window.display();
	}
}
