#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct pattern {
	std::string name;
	int w, h;
	std::vector<bool> shp;
	sf::Rect<float> area;
	pattern(std::string name, int x, int y, std::vector<bool> arr, sf::Vector2f dim) :name(name), w(x), h(y), shp(arr), area(0, 0, dim.x, 100.f) {}
};

class GameOfLife
{
private:
	int cols, rows;
	std::vector<std::vector<bool>> game;
	int numOfLiveCells;

	std::vector<sf::Vector2f> CordsOfSqr(int, int);
	std::vector<pattern> patterns;

	int current_shape = 0;
	int total_shapes = 2;
	sf::Rect<float> guiRect;
	sf::RectangleShape currentSelected;

	sf::Vector2f guiDimensions, gameDimensions;

public:
	float cellSize;
	bool gameState = false;
	std::vector<std::vector<bool>> nextGen;
	std::vector<std::vector<bool>> reset;
	sf::RenderWindow& win;


	GameOfLife(sf::RenderWindow&, float);
	void CreateGame(sf::View& gameView, sf::View& guiView);
	void ClearGame();
	sf::Vector2i getDimensions();
	void ComputeNextGen();
	void DisplayGrid();
	void DrawShape(int, int);
	void SetShape(float, float);
	void DisplayguiGrid(sf::Font&);
	void ChangeShape(sf::Vector2f);
};

