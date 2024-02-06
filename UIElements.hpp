#ifndef UI_H_
#define UI_H_

#include <SFML/Graphics.hpp>

struct Textures{
    /*static*/ sf::Texture buttonTexture;
    /*static*/ sf::Texture inputTexture;
    /*static*/ sf::Font arial, arialb;
    Textures();
};

struct InputDomain{
    struct InputBar{
        sf::RectangleShape bar;
        sf::Texture barTexture;
        InputBar(sf::Vector2f position, float scale, sf::Vector2f size = sf::Vector2f(-1, -1));
    };
    
    InputBar inputBar;
    sf::Text text;
    std::string input;
    bool hide;
    std::string hidden;
    sf::Text inputText;
    InputDomain(std::string head, sf::Vector2f position, float scale,
     sf::Color inputColor = sf::Color::White, sf::Color headColor = sf::Color::White, sf::Vector2f barSize = sf::Vector2f(-1, -1));
    
    bool isTouchingMouse(sf::Vector2i pos);
    void draw(sf::RenderWindow &window, bool active, sf::Color baseColor, sf::Color avtiveColor);

    void write(sf::Event event);
    void erase(sf::Event event);
    void flipHiddenText();
};

struct Button{
    sf::RectangleShape shape;
    sf::Texture shapeTexture;
    sf::Text text;
    Button(std::string head, sf::Vector2f position, float scale);

    bool isTouchingMouse(sf::Vector2i pos);
    void draw(sf::RenderWindow &window, sf::Vector2i pos, sf::Color baseColor, sf::Color touchColor);
    void setPosition(sf::Vector2i pos, float scale);
};

void setText(sf::Text &text, sf::Vector2f position, sf::Color fillColor, float charSize);

void setTexture(sf::RectangleShape &shape, sf::Texture &texture, sf::Vector2f position, sf::Vector2f size);

#endif