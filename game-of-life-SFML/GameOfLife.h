#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class GameOfLife
{
private:
	int cols, rows;
	std::vector<std::vector<bool>> game;
	int numOfLiveCells;

	std::vector<sf::Vector2f> CordsOfSqr(int, int);
public:
	float cellSize;
	bool gameState = false;
	std::vector<std::vector<bool>> nextGen;
	std::vector<std::vector<bool>> reset;
	sf::RenderWindow& win;


	GameOfLife(sf::RenderWindow&, float);
	void ClearGame();
	sf::Vector2i getDimensions();
	void ComputeNextGen();
	void DisplayGrid();
	void DrawShape(int, int);
	void SetAliveOrDead(float, float);
};

