#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

#include "GameOfLife.h"



int main() {
    //main game window
    int screenWidth = 1280, screenHeight = 720;
    int gameWidth = 1920, gameHeight = 1080;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML OP", sf::Style::Default);
    window.setFramerateLimit(144);
    
    //Game View
    sf::View GameView(sf::FloatRect(0, 0, gameWidth, gameHeight));
    GameView.setViewport(sf::FloatRect(0, 0, 4.f/5.f, 4.f/5.f));

    //Rules View
    sf::View RulesView(sf::FloatRect(0, 0, screenWidth, screenHeight));
    RulesView.setViewport(sf::FloatRect(0, 4.f/5.f, 1, 1));

    //loading font
    sf::Font font;
    
    if (!font.loadFromFile("font.ttf")) { std::cout << "failed to load font \n";  return -1; }

    //ini game
    window.setView(GameView);
    GameOfLife game(window, 15);
    auto dimensions = game.getDimensions();

    //fps
    std::chrono::steady_clock::time_point start, end, delay;
    double fps = 0;
    std::chrono::duration<double> d;
    while (window.isOpen())
    {
        start = std::chrono::high_resolution_clock::now();
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed:
            {
                auto mouseCords = sf::Mouse::getPosition(window);
                auto gameCords = window.mapPixelToCoords(mouseCords, GameView);

             
                //std::cout << mouseInp.button << std::endl;
                //std::cout << "coords " << .x << " " << mouseInp.y << std::endl;
                window.setView(GameView);
                game.SetAliveOrDead(gameCords.x, gameCords.y);
                break;
            }

            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Space)
                    game.gameState = !game.gameState;

                if (event.key.code == sf::Keyboard::LControl)
                    game.ClearGame();

                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            }

            case sf::Event::Closed:
            {
                window.close();                
                break;
            }

            default:
                break;
            }

        }

        if (game.gameState) {
            game.ComputeNextGen();
        }

        window.clear();

        window.setView(GameView);
        game.DisplayGrid();

        window.setView(RulesView);
        std::string state = game.gameState ? "Alive" : "Dead";
        std::string s = 
              "Space to Play/Pause. Current State = " + state
            + "\nUse mouse to add/remove cells" 
            + "\nescape to exit" 
            + "\nLeft Control to clear" 
            + "\nColumns = " + std::to_string(dimensions.x) 
            + ", Rows = " + std::to_string(dimensions.y) + "\n"
            + "FPS: " + std::to_string(fps);
        sf::Text text(s, font, 20);
        text.setFillColor(sf::Color(255, 255, 255, 150));
        window.draw(text);
        window.display();

        end = std::chrono::high_resolution_clock::now();
        d = end - delay;
        if (game.gameState && 1 < d.count() ) {
            d = end - start;
            fps = 1.f / d.count();
            delay = std::chrono::high_resolution_clock::now();
        }
    }
	return 0;
}
  