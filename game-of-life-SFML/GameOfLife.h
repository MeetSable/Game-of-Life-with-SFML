#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class GameOfLife
{
private:
	int cols, rows;
	std::vector<std::vector<bool>> game;
public:
	float cellSize;
	bool gameState = false;
	std::vector<std::vector<bool>> nextGen;
	std::vector<std::vector<bool>> reset;


	GameOfLife(sf::RenderWindow&, float);
	void ClearGame();
	sf::Vector2i getDimensions();
	void ComputeNextGen();
	void DisplayGrid(sf::RenderWindow&);
	void DrawShape(sf::RenderWindow&, int, int);
	void SetAliveOrDead(sf::RenderWindow&, float, float);
};

