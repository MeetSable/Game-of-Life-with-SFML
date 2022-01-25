#include "GameOfLife.h"

#include <cmath>
#include <iostream>

#define MOD(a,b) (b + a%b)%b

GameOfLife::GameOfLife(sf::RenderWindow& window, float a) : win(window), cellSize(a) {}

void GameOfLife::CreateGame(sf::View& gameView, sf::View& guiView) {
    gameDimensions = gameView.getSize();
    cols = std::ceil(gameDimensions.x / cellSize);
    rows = std::ceil(gameDimensions.y / cellSize);
    game.resize(cols, std::vector<bool>(rows, false));
    nextGen.resize(cols, std::vector<bool>(rows, false));
    numOfLiveCells = 0;
    reset = game;

    guiDimensions = sf::Vector2f(guiView.getSize().x / 5.f, guiView.getSize().y);
    patterns = { pattern("Single \nSquare", 1, 1, {1}, guiDimensions),
                pattern("Glider",           3, 3, {1,0,0,
                                                   0,1,1,
                                                   1,1,0}, guiDimensions),
                pattern("The R-pentomino", 3, 3,  {0,1,1,
                                                   1,1,0,
                                                   0,1,0}, guiDimensions),
                pattern("Die-Hard",        8, 3, {0,0,0,0,0,0,1,0,
                                                  1,1,0,0,0,0,0,0,
                                                  0,1,0,0,0,1,1,1}, guiDimensions),
                pattern("Acorn",           7, 3, {0,1,0,0,0,0,0,
                                                  0,0,0,1,0,0,0,
                                                  1,1,0,0,1,1,1 }, guiDimensions),
                pattern("Glider-Gun",      36, 9, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                                   0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                                   1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                                   0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}, guiDimensions), };
    for (int i = 1; i < patterns.size(); i++) {
        patterns[i].area.top = patterns[i - 1].area.top + 100.f;
    }
    total_shapes = patterns.size();
    guiRect = patterns[0].area;
    currentSelected.setSize(sf::Vector2f(guiRect.width, guiRect.height));
    currentSelected.setPosition(sf::Vector2f(guiRect.left, guiRect.top));
    currentSelected.setFillColor(sf::Color(255, 255, 255, 50));
}

void GameOfLife::ClearGame() {
    game = reset;
    gameState = false;
    numOfLiveCells = 0;
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
                if (n < 2 || n > 3) {
                    nextGen[i][j] = false;
                    numOfLiveCells--;
                }
            }
            else {
                if (n == 3) {
                    nextGen[i][j] = true;
                    numOfLiveCells++;
                }
            }

        }
    game = nextGen;
}

void GameOfLife::DisplayGrid()
{
    //intializing variables
    
    auto winSize = (sf::Vector2f)win.getView().getSize();
    /*int numLines = rows + cols - 2;
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
    sf::VertexArray gameSqr(sf::LinesStrip, 4);
    gameSqr[0].position = sf::Vector2f(0        , 0        );
    gameSqr[1].position = sf::Vector2f(0        , winSize.y);
    gameSqr[2].position = sf::Vector2f(winSize.x, winSize.y);
    gameSqr[3].position = sf::Vector2f(winSize.x, 0        );
    win.draw(gameSqr);

    sf::VertexArray liveCells(sf::Quads, 4 * numOfLiveCells);
    std::vector<sf::Vector2f> sqr(4);
    int BufferIndex = 0;
    for (int i = 0; i < cols; i++)
        for (int j = 0; j < rows; j++)
            if (game[i][j]) {
                sqr = CordsOfSqr(i, j);
                liveCells[BufferIndex * 4 + 0].position = sqr[0];
                liveCells[BufferIndex * 4 + 1].position = sqr[1];
                liveCells[BufferIndex * 4 + 2].position = sqr[2];
                liveCells[BufferIndex * 4 + 3].position = sqr[3];
                BufferIndex++;
            }
    win.draw(liveCells);
                
}

void GameOfLife::DrawShape(int x, int y)
{
    float offset = 0;
    sf::RectangleShape rect(sf::Vector2f(cellSize-offset, cellSize-offset));
    auto winSize = win.getView().getSize();

    rect.setPosition(sf::Vector2f(x * cellSize + offset/2.f, y * cellSize + offset/2.f));
    win.draw(rect);
}

void GameOfLife::SetShape(float x, float y)
{
    auto winSize = (sf::Vector2f)win.getView().getSize();
    int i = (int)std::ceil(cols * x / winSize.x);
    int j = (int)std::ceil(rows * y / winSize.y);
    int h = patterns[current_shape].h, w = patterns[current_shape].w;
    int h2 = std::ceil(h / 2.f), w2 = std::ceil(w / 2.f);
    for (int it = 0; it < w; it++)
        for (int jt = 0; jt < h; jt++)
        {
            bool temp = patterns[current_shape].shp[it + w*jt];
            if (game[MOD(i + it - w2, cols)][MOD(j + jt - h2, rows)] ^ temp) {
                numOfLiveCells += temp ? 1 : -1;
                game[MOD(i + it - w2, cols)][MOD(j + jt - h2, rows)] = temp;
            }
        }
}

std::vector<sf::Vector2f> GameOfLife::CordsOfSqr(int x, int y) {
    std::vector<sf::Vector2f> sqr(4);
    float xf = x * cellSize, yf = y * cellSize;
    sqr[0] = sf::Vector2f(xf           , yf + cellSize);
    sqr[1] = sf::Vector2f(xf           , yf           );
    sqr[2] = sf::Vector2f(xf + cellSize, yf           );
    sqr[3] = sf::Vector2f(xf + cellSize, yf + cellSize);
    return sqr;
}

void GameOfLife::DisplayguiGrid(sf::Font& font) {
    int size = patterns.size();
    //std::cout << size << std::endl;
    
    auto viewSize = win.getView().getSize();
    auto gridSize = sf::Vector2f(viewSize.x/5.f, 100.f);

    //std::cout << gridSize.x << " " << gridSize.y << std::endl;
    int numOfLines = 3 + size;
    sf::VertexArray grid(sf::Lines, numOfLines * 2);
    //h Lines
    grid[0].position = { 1         , 0 };
    grid[1].position = { 1         , viewSize.y };
    grid[2].position = { gridSize.x, 0          };
    grid[3].position = { gridSize.x, viewSize.y };

    for (int i = 0; i <= size; i++) {
        grid[(i + 2) * 2].position = { 0, i * gridSize.y};
        grid[(i + 2) * 2 + 1].position = { gridSize.x, i * gridSize.y};
    }
    win.draw(grid);

    for (int i = 0; i < size; i++) {
        sf::Text text(patterns[i].name, font, 20);
        text.setPosition(sf::Vector2f(-text.getLocalBounds().width/2.f + gridSize.x/2.f, -text.getLocalBounds().height / 2.f + gridSize.y * i + gridSize.y / 2.f));
        win.draw(text);
    }
    
    win.draw(currentSelected);
}

void GameOfLife::ChangeShape(sf::Vector2f mousePosition)
{
    for (int i = 0; i < total_shapes; i++) {
        if (patterns[i].area.contains(mousePosition)) {
            current_shape = i;
            currentSelected.setPosition(sf::Vector2f(guiRect.left, guiRect.height * i));
            //std::cout << patterns[i].name << std::endl;
            break;
        }
    }
}

