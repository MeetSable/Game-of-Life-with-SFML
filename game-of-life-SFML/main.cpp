#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

#include "GameOfLife.h"


class Game {
private:
    int screenWidth, screenHeight;
    int gameWidth, gameHeight;
    
    sf::RenderWindow window;
    sf::View GameView;
    sf::View RulesView;
    sf::View guiView;
    sf::Font font;
    sf::Vector2i dimensions;

    sf::Rect<float> gameRect, guiRect, rulesRect;

    GameOfLife gameOfLife;
    int cellSize;

    std::chrono::steady_clock::time_point start;
    std::chrono::duration<double> d;
    double fps, fps_temp;

public:
    Game(float);
    void init();
    void run();
    void processEvent();
    void draw();
};

Game::Game(float a):gameOfLife(window, a), cellSize(a) {
    screenWidth = 1280, screenHeight = 720;
    window.create(sf::VideoMode(screenWidth, screenHeight), "SFML OP", sf::Style::Default);

    gameWidth = 1920, gameHeight = 1080;
    GameView.reset(sf::FloatRect(0, 0, gameWidth, gameHeight));
    GameView.setViewport(sf::FloatRect(0, 0, 4.f / 5.f, 4.f / 5.f));
    window.setView(GameView);
    gameRect = sf::FloatRect(0, 0, screenWidth * 4.f / 5.f, screenHeight * 4.f / 5.f);

    RulesView.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
    RulesView.setViewport(sf::FloatRect(0, 4.f / 5.f, 1, 1));
    rulesRect = sf::FloatRect(0, screenHeight * 4.f / 5.f, screenWidth * 4.f / 5.f, screenHeight * 1.f / 5.f);

    guiView.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
    guiView.setViewport(sf::FloatRect(4.f/5.f, 0, 1, 1));
    guiRect = sf::FloatRect(screenWidth * 4.f / 5.f, 0, screenWidth * 1.f / 5.f, screenHeight);

    gameOfLife.CreateGame(GameView, guiView);

    fps = 0, fps_temp = 0;
}

void Game::init() {
    if (!font.loadFromFile("font.ttf")) { std::cout << "failed to load font \n";  return; }
    window.setView(GameView);
    dimensions = gameOfLife.getDimensions();
    start = std::chrono::high_resolution_clock::now();
}

void Game::run() {
    init();
    while (window.isOpen()) {
        processEvent();

        if (gameOfLife.gameState) {
            gameOfLife.ComputeNextGen();
            d = std::chrono::high_resolution_clock::now() - start;
            fps_temp++;
            if (d.count() > 1.f) {
                fps = fps_temp;
                fps_temp = 0;
                start = std::chrono::high_resolution_clock::now();
            }
        }       
        draw();
    }
}

void Game::processEvent() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
        {
            auto mousePosition = sf::Mouse::getPosition(window);
            if (gameRect.contains((sf::Vector2f)mousePosition)) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    
                    auto gameCords = window.mapPixelToCoords(mousePosition, GameView);

                    //std::cout << mouseInp.button << std::endl;
                    //std::cout << "coords " << .x << " " << mouseInp.y << std::endl;
                    window.setView(GameView);
                    gameOfLife.SetShape(gameCords.x, gameCords.y);
                }
            }
            else if (guiRect.contains((sf::Vector2f)mousePosition)) {
                std::cout << "we're in gui\n";
            }
            break;
        }

        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Space)
                gameOfLife.gameState = !gameOfLife.gameState;

            if (event.key.code == sf::Keyboard::LControl)
                gameOfLife.ClearGame();

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
}

void Game::draw() {
    window.clear();

    window.setView(GameView);
    gameOfLife.DisplayGrid();

    window.setView(guiView);
    gameOfLife.DisplayguiGrid(font);

    window.setView(RulesView);
    std::string state = gameOfLife.gameState ? "Alive" : "Dead";
    std::string s =
        "Space to Play/Pause. Current State = " + state
        + "\nUse mouse to add/remove cells"
        + "\nescape to exit"
        + "\nLeft Control to clear"
        + "\nColumns = " + std::to_string(dimensions.x)
        + ", Rows = " + std::to_string(dimensions.y) + "\n"
        + "FPS: " + std::to_string(fps);
    sf::Text text(s, font, 20);
    text.setFillColor(sf::Color(255, 255, 255));
    window.draw(text);

    window.display();


}

int main() {
    Game game(5);
    game.run();
	return 0;
}
  