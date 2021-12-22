#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "GameOfLife.h"



int main() {
    //main game window
    int screenWidth = 1920, screenHeight = 1080;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML OP", sf::Style::Fullscreen);
    

    //loading font
    sf::Font font;
    
    if (!font.loadFromFile("font.ttf")) { std::cout << "failed to load font \n";  return -1; }

    //ini game
    GameOfLife game(window, 3);
    auto dimensions = game.getDimensions();

    //test
    sf::Text text("Space to Play/Pause.\nUse mouse to add/remove cells \nescape to exit \nLeft Control to clear \nColumns = "+std::to_string(dimensions.x) + ", Rows = " + std::to_string(dimensions.y), font, 20);
    text.setFillColor(sf::Color(255,255,255,150));   

    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed:
                auto mouseInp = event.mouseButton;
                //std::cout << mouseInp.button << std::endl;
                //std::cout << "coords " << mouseInp.x << " " << mouseInp.y << std::endl;
                game.SetAliveOrDead(window, mouseInp.x, mouseInp.y);
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space)
                    game.gameState = !game.gameState;

                if (event.key.code == sf::Keyboard::LControl)
                    game.ClearGame();

                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;

            case sf::Event::Closed:
                break;

            default:
                break;
            }

        }

        if (game.gameState) {
            game.ComputeNextGen();
        }

        window.clear();
        game.DisplayGrid(window);
        window.draw(text);
        window.display();
    }
	return 0;
}
  