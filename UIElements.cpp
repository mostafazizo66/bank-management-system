#include "UIElements.hpp"

Textures::Textures(){
    buttonTexture.loadFromFile("src/Textures/button.png");
    inputTexture.loadFromFile("src/Textures/input.png");
    arial.loadFromFile("src/Fonts/arialb.ttf");
    arialb.loadFromFile("src/Fonts/ABH.ttf");
}

static Textures textures;

void setTexture(sf::RectangleShape &shape, sf::Texture &texture, sf::Vector2f position, sf::Vector2f size){
   shape.setTexture(&texture);
   shape.setPosition(position);
   shape.setSize(size);
}

void setText(sf::Text &text, sf::Vector2f position, sf::Color fillColor, float charSize){
    text.setFont(textures.arial);
    text.setPosition(position);
    text.setFillColor(fillColor);
    text.setCharacterSize(charSize);
}

InputDomain::InputBar::InputBar(sf::Vector2f position, float scale, sf::Vector2f size){
    if(size == sf::Vector2f(-1, -1)){
        size = sf::Vector2f(9 * scale, scale);
    }
    setTexture(bar, textures.inputTexture, position, size);
}

InputDomain::InputDomain(std::string head, sf::Vector2f position, float scale, sf::Color inputColor, sf::Color headColor, sf::Vector2f barSize)
    : inputBar(sf::Vector2f(position.x + scale/4 * head.size(), position.y - scale/9), scale, barSize) {

    setText(inputText, sf::Vector2f(position.x + scale * 0.27 * head.size(), position.y), inputColor, scale / 2);

    setText(text, position, headColor, scale / 2);
    hide = false;
    text.setString(head);
}

bool InputDomain::isTouchingMouse(sf::Vector2i pos){
    return inputBar.bar.getGlobalBounds().contains(pos.x, pos.y);
}

void InputDomain::draw(sf::RenderWindow &window, bool active, sf::Color baseColor, sf::Color activeColor){
    if(active){
        inputBar.bar.setFillColor(activeColor);
    }else{
        inputBar.bar.setFillColor(baseColor);
    }
    window.draw(inputBar.bar);
    window.draw(text);
    window.draw(inputText);
}

void InputDomain::write(sf::Event event){
    input += static_cast<char>(event.text.unicode);
    hidden += '*';
    inputText.setString(hide? hidden : input);
}

void InputDomain::erase(sf::Event event){
    if(input.size() > 0){
        input.pop_back();
    }
    if(hidden.size() > 0){
        hidden.pop_back();
    }
    inputText.setString(hide? hidden : input);
}

void InputDomain::flipHiddenText(){
    if(hide){
        inputText.setString(hidden);
    }
    else
    {
        inputText.setString(input);
    }
}

Button::Button(std::string head, sf::Vector2f position, float scale){
    shape.setTexture(&textures.buttonTexture);
    shape.setSize(sf::Vector2f(scale * 2, scale * 0.6));
    shape.setPosition(sf::Vector2f(position.x - 3 * scale / 4, position.y - scale/10));

    setText(text, sf::Vector2f(position.x - scale * 0.4, position.y), sf::Color::White, scale / 3);
    text.setString(head);
}

bool Button::isTouchingMouse(sf::Vector2i pos){
    return shape.getGlobalBounds().contains(pos.x, pos.y);
}

void Button::draw(sf::RenderWindow &window, sf::Vector2i pos, sf::Color baseColor, sf::Color touchColor){
    if(isTouchingMouse(pos)){
        shape.setFillColor(touchColor);
    }else{
        shape.setFillColor(baseColor);
    }
    window.draw(shape);
    window.draw(text);
}

void Button::setPosition(sf::Vector2i position, float scale){
    shape.setPosition(sf::Vector2f(position.x - 3 * scale / 4, position.y - scale/10));
    text.setPosition(sf::Vector2f(position.x - scale * 0.4, position.y));
}
