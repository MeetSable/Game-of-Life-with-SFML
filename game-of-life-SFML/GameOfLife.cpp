#include "GameOfLife.h"

#include <cmath>
#include <iostream>

#define MOD(a,b) (b + a%b)%b

GameOfLife::GameOfLife(sf::RenderWindow& win, float a) {
    cellSize = a;
    auto winSize = win.getView().getSize();
    cols = std::ceil(winSize.x / cellSize);
    rows = std::ceil(winSize.y / cellSize);
	game.resize(cols, std::vector<bool>(rows, false));
    nextGen.resize(cols, std::vector<bool>(rows, false));
}

void GameOfLife::ClearGame() {
    game = reset;
}

sf::Vector2i GameOfLife::getDimensions() {
    return sf::Vector2i(cols, rows);
}

void GameOfLife::ComputeNextGen()
{
    nextGen = game;
    //Any live cell with two or three live neighbours survives.
    //Any dead cell with three live neighbours becomes a live cell.
    //All other live cells die in the next generation. Similarly, all other dead cells stay dead.
    for (int i = 0; i < cols; i++)
        for (int j = 0; j < rows; j++)
        {
            int n = 0;
            for (int k = 0; k < 3; k++)
            {
                for (int kk = 0; kk < 3; kk++)
                {
                    if (n > 3) break;
                    if (k == 1 && kk == 1) continue;
                    //std::cout << MOD((i + k - 1), cols) << std::endl;
                    if (game[MOD((i + k - 1), cols)][MOD((j + kk - 1), rows)]) n++;
                }
                if (n > 3) break;
            }
            if (game[i][j]) {
                if (n < 2 || n > 3) nextGen[i][j] = false;
            }
            else {
                if (n == 3) nextGen[i][j] = true;
            }

        }
    game = nextGen;
}

void GameOfLife::DisplayGrid(sf::RenderWindow& win)
{
    //intializing variables
    
    /*auto winSize = (sf::Vector2f)win.getView().getSize();
    int numLines = rows + cols - 2;
    sf::VertexArray grid(sf::Lines, 2 * (numLines));

    for (int i = 0; i < cols - 1; i++) {
        grid[i * 2].position = { (i + 1) * cellSize,0 };
        grid[i * 2 + 1].position = { (i + 1) * cellSize, winSize.y };
    }

    for (int i = 0; i < rows - 1; i++) {
        grid[(i + cols - 1) * 2].position = { 0, (i + 1) * cellSize };
        grid[(i + cols - 1) * 2 + 1].position = { winSize.x, (i + 1) * cellSize };
    }

    win.draw(grid); */

    for (int i = 0; i < cols; i++)
        for (int j = 0; j < rows; j++)
            if (game[i][j])
                DrawShape(win, i, j);
}

void GameOfLife::DrawShape(sf::RenderWindow& win, int x, int y)
{
    float offset = 0;
    sf::RectangleShape rect(sf::Vector2f(cellSize-offset, cellSize-offset));
    auto winSize = win.getView().getSize();

    rect.setPosition(sf::Vector2f(x * cellSize + offset/2.f, y * cellSize + offset/2.f));
    win.draw(rect);
}

void GameOfLife::SetAliveOrDead(sf::RenderWindow& win, float x, float y)
{
    auto winSize = (sf::Vector2f)win.getSize();
    int i = (int)std::floor(cols * x / winSize.x);
    int j = (int)std::floor(rows * y / winSize.y);
    game[i][j] = !game[i][j];
}