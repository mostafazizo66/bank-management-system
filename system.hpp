#ifndef SYS_H_
#define SYS_H_

#include <vector>
#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "files.hpp"
#include "account.hpp"
#include "algorithms.hpp"
#include "UIElements.hpp"
#include "colors.cpp"

struct System
{
    std::vector<account> accounts;
    sf::RenderWindow window;
    float scale;
    System();

    sf::VideoMode setResolution();
    std::string renderLogin();
    void renderMenu(std::string user);
    void run();
};


#endif