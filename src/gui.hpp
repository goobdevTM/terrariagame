#ifndef GUI_HPP
#define GUI_HPP

#include "data.hpp"
#include <cmath>
#include "delta.hpp"
#include "../include/json.hpp"

void load_from_json(std::string path);

enum text_offsets {
    CENTERED,
    LEFT,
    RIGHT,
    ABOVE,
    BELOW
};

class Button {
    int tex_size = 16;
    int corner_size = 7;
    int middle_size = tex_size - (corner_size*2);
    std::string tex_path = "../assets/textures/gui/button.png";
    //LEFT TO RIGHT, TOP TO BOTTOM
    sf::Texture textures[9] = {

        sf::Texture(tex_path, false, sf::IntRect({0, 0}, {corner_size, corner_size})), //TOP LEFT
        sf::Texture(tex_path, false, sf::IntRect({corner_size, 0}, {middle_size, corner_size})), //TOP MIDDLE
        sf::Texture(tex_path, false, sf::IntRect({corner_size + middle_size, 0}, {corner_size, corner_size})), //TOP RIGHT

        sf::Texture(tex_path, false, sf::IntRect({0, corner_size}, {corner_size, middle_size})), //MIDDLE LEFT
        sf::Texture(tex_path, false, sf::IntRect({corner_size, corner_size}, {middle_size, middle_size})), //MIDDLE MIDDLE
        sf::Texture(tex_path, false, sf::IntRect({corner_size + middle_size, corner_size}, {corner_size, middle_size})), //MIDDLE RIGHT

        sf::Texture(tex_path, false, sf::IntRect({0, corner_size + middle_size}, {corner_size, corner_size})), //BOTTOM LEFT
        sf::Texture(tex_path, false, sf::IntRect({corner_size, corner_size + middle_size}, {middle_size, corner_size})), //BOTTOM MIDDLE
        sf::Texture(tex_path, false, sf::IntRect({corner_size + middle_size, corner_size + middle_size}, {corner_size, corner_size})) //BOTTOM RIGHT

    }; 

    sf::Sprite sprites[9] = {
        sf::Sprite(textures[0]),
        sf::Sprite(textures[1]),
        sf::Sprite(textures[2]),
        sf::Sprite(textures[3]),
        sf::Sprite(textures[4]),
        sf::Sprite(textures[5]),
        sf::Sprite(textures[6]),
        sf::Sprite(textures[7]),
        sf::Sprite(textures[8])
    };
    sf::Vector2f position;
    sf::Vector2f scale = {1.f,1.f};
    sf::Vector2f animated_scale = {1.f,1.f};
    sf::Vector2f size;
    sf::Vector2f true_size;
    sf::Color color;
    bool has_text = false;
    std::string text;
    int font_size;
    int outline_size;
    const sf::Font font = sf::Font("../assets/fonts/Ithaca.ttf");
    sf::Text text_object = sf::Text(font, "", font_size);
    text_offsets text_offset;
    bool scale_text = true;
    float text_detail = 4.f;
    AABB collision = {0,0,0,0};
    bool evil_mouse_down = false;
    sf::Vector3f animated_color_float = {0.f,0.f,0.f};
    void setCollision() {
        collision = {position.x, position.y, position.x + (true_size.x*scale.x), position.y + (true_size.y*scale.y)};
    }
    public:
        bool hovered = false;
        bool pressed = false;
        sf::Color hover_color;
        sf::Color pressed_color;
        void setPosition(sf::Vector2f new_position, bool centered = true) {
            position = new_position;
            if (centered) {
                position -= sf::Vector2f((true_size.x/2.f) * animated_scale.x, (true_size.y/2.f) * animated_scale.y);
            }
            for (uint i = 0; i < 9; ++i) {
                sf::Vector2f offset = {};
                int offsetx = i % 3;
                if (offsetx == 1) {
                    offsetx = corner_size;
                    sprites[i].setScale(sf::Vector2f(size.x*animated_scale.x, sprites[i].getScale().y));
                }
                else if (offsetx == 2) {offsetx = corner_size + true_size.x - float(corner_size)*2;}
                offset.x = float(offsetx);
                offsetx = i / 3;
                if (offsetx == 1) {
                    offsetx = corner_size;
                    sprites[i].setScale(sf::Vector2f(sprites[i].getScale().x, size.y*animated_scale.y));
                }
                else if (offsetx == 2) {offsetx = corner_size + true_size.y - float(corner_size)*2;}
                offset.y = float(offsetx);
                sprites[i].setPosition(((position - (sf::Vector2f((true_size.x/2.f) * animated_scale.x, (true_size.y/2.f) * animated_scale.y)  -  sf::Vector2f((true_size.x/2.f) * scale.x, (true_size.y/2.f) * scale.y)))  +  sf::Vector2f(offset.x*animated_scale.x, offset.y*animated_scale.y)) + sf::Vector2f());
            }
            setCollision();
        }
        sf::Vector2f getPosition() {
            return position + sf::Vector2f(true_size.x/2.f * animated_scale.x, true_size.y/2.f * animated_scale.y);
        }
        void setScale(sf::Vector2f new_scale, bool set_vars = true) {
            if (set_vars) {
                scale = new_scale;
                animated_scale = new_scale;
            }
            for (uint i = 0; i < 9; ++i) {
                sprites[i].setScale(new_scale);
            }
            setPosition(getPosition());
            if (scale_text) {
                text_object.setScale(sf::Vector2f(new_scale.x / text_detail, new_scale.y / text_detail));
            }
            setCollision();
        }   
        sf::Vector2f getScale() {
            return scale;
        }
        void setSize(sf::Vector2f new_size) {
            true_size = new_size;
            size = sf::Vector2f(float(new_size.x - corner_size*2)/float(middle_size),float(new_size.y - corner_size*2)/float(middle_size));
            std::cout << "Size x: " << size.x << " Size y: " << size.y << "\n";
            setPosition(getPosition());
        }
        sf::Vector2f getSize(bool with_scale = false) {
            if (with_scale) {
                return sf::Vector2f(true_size.x*scale.x, true_size.y*scale.y);
            }
            return true_size;
        }
        void setColor(sf::Color new_color, bool change_vars = true, bool change_hover_pressed = true) {
            if (change_vars) {
                color = new_color;
                animated_color_float = sf::Vector3f(color.r, color.g, color.b);
                if (change_hover_pressed) {
                    hover_color = sf::Color(std::clamp(color.r + 10,0,255),std::clamp(color.g + 10,0,255),std::clamp(color.b + 10,0,255));
                    pressed_color = sf::Color(std::clamp(color.r - 10,0,255),std::clamp(color.g - 10,0,255),std::clamp(color.b - 10,0,255));
                }
            }
            for (uint i = 0; i < 9; ++i) {
                sprites[i].setColor(new_color);
            }
        }
        sf::Color getColor() {
            return sprites[0].getColor();
        }
        void setText(std::string new_text, int new_font_size = 16, int new_outline_size = 4, text_offsets new_text_offset = text_offsets::CENTERED){
            text = new_text;
            font_size = new_font_size * text_detail;
            if (new_outline_size > 0) {
                outline_size = new_outline_size * text_detail;
            }
            has_text = true;
            text_object.setFont(font);
            text_object.setCharacterSize(font_size);
            text_object.setFillColor(sf::Color::White);
            text_object.setOutlineColor(sf::Color::Black);
            text_object.setOutlineThickness(outline_size);
            text_object.setString(text);
            text_object.setPosition(position + sf::Vector2f(true_size.x/2.f * scale.x, true_size.y/2.f * scale.y));
            if (scale_text) {
                text_object.setScale(sf::Vector2f(getScale().x / text_detail, getScale().y / text_detail));
            }
            sf::Rect<float> text_rect = text_object.getLocalBounds();
            text_object.setOrigin(text_rect.getCenter());
            switch (text_offset)
            {
                case CENTERED:
                    text_object.move({getSize(true).x / 2.0f, getSize(true).y / 2.0f});
                    break;
                case LEFT:
                    text_object.setOrigin({text_rect.size.x, text_rect.getCenter().y});
                    text_object.move({-getSize(true).x / 2.0f, 0.0f});
                    break;
                case RIGHT:
                    text_object.setOrigin({-text_rect.size.x, text_rect.getCenter().y});
                    text_object.move({getSize(true).x / 2.0f, 0.0f});
                    break;
                case ABOVE:
                    text_object.setOrigin({text_rect.getCenter().x, text_rect.size.y});
                    text_object.move({0.0f, -getSize(true).y / 2.0f});
                    break;
                case BELOW:
                    text_object.setOrigin({text_rect.getCenter().x, -text_rect.size.y});
                    text_object.move({0.0f, getSize(true).y / 2.0f});
                    break;

                default:
                    break;
            }
        }
        void draw(sf::RenderWindow &window) {
            for (uint i = 0; i < 9; ++i) {
                window.draw(sprites[i]);
            }
            window.draw(text_object);
        }
        void animateColor(sf::Color to_color, float lerp_speed) {
            animated_color_float = sf::Vector3f(std::lerp(animated_color_float.x, float(to_color.r), float(delta) * lerp_speed), std::lerp(animated_color_float.y, float(to_color.g), float(delta) * lerp_speed), std::lerp(animated_color_float.z, float(to_color.b), float(delta) * lerp_speed));
            setColor(sf::Color(std::clamp(int(animated_color_float.x), 0, 255), std::clamp(int(animated_color_float.y), 0, 255), std::clamp(int(animated_color_float.z), 0, 255)), false);
        }
        void animateScale(sf::Vector2f to_scale, float lerp_speed) {
            animated_scale = sf::Vector2f(std::lerp(animated_scale.x, to_scale.x, delta * lerp_speed), std::lerp(animated_scale.y, to_scale.y, delta * lerp_speed));
            setScale(animated_scale, false);
        }
        void process() {
            //std::cout << "Mouse pos x: " << mouse_pos.x << " y: " << mouse_pos.y << " AABB: {" << collision.minX << ", " << collision.minY << ", " << collision.maxX << ", " << collision.maxY << "} \n";
            if (collision.intersects_point(mouse_pos)) {
                if (not mouse_down) {
                    evil_mouse_down = false; //IMPOSSIBLE TO HAVE CLICKED OUTSIDE, SET FALSE
                }
                if (mouse_down and not evil_mouse_down) {
                    pressed = true;
                    animateColor(pressed_color, 120.f);
                    animateScale(sf::Vector2f(scale.x * 0.98f, scale.y * 0.98f), 60.f);
                } else {
                    if (pressed) {
                        animateColor(hover_color, 120.f);
                        animateScale(sf::Vector2f(scale.x * 1.05f, scale.y * 1.05f), 60.f);
                    } else {
                        animateColor(hover_color, 40.f);
                        animateScale(sf::Vector2f(scale.x * 1.05f, scale.y * 1.05f), 40.f);
                    }
                    pressed = false;
                    hovered = true;
                }
            } else {
                hovered = false;
                pressed = false;
                animateColor(color, 30.f);
                animateScale(sf::Vector2f(scale.x * 1.f, scale.y * 1.f), 40.f);
                evil_mouse_down = mouse_down; //CLICKED OUTSIDE OF BUTTON
            }
        }
};

#endif //GUI_HPP