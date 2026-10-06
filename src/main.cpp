#include <cstddef>
#include <iostream>
#include <linux/limits.h>
#include <random>
#include <vector>
#include <unordered_map>
#include <thread>
#include <chrono>

#include <termios.h>
#include <unistd.h>
#include <utility>
using namespace std;
struct RGB {
    int r;
    int g;
    int b;
};
struct coordinates{
    int y;
    int x;
};

using Frame = std::vector<std::vector<RGB>>;
using Board = unordered_map<char, vector<coordinates>>;

constexpr int boardSize = 10;

const RGB black {0,0,0};
const RGB gray {128,128, 128};
const RGB red {255,0,0};
const RGB darkGreen {1,50,32};
const RGB green {0,255,0};
// const RGB blue {0,0,255};
// const RGB purpNeon {191,0,255};



void setBackground(int r, int g, int b) {
    if (r == 256) {
        cout << "\033[0m";
    }
    else{
        cout << "\033[48;2;"
         << r << ';' << g << ';' << b << 'm';
    }
}
void printPixel(RGB color) {
    setBackground(color.r, color.g, color.b);
    cout << "  ";
}

void drawFrame(const Frame& frameScheme){
    cout << "\033[?25l"; // скрыть курсор
    for(size_t y = 0; y < frameScheme.size(); y++){
        for(size_t x = 0; x < frameScheme[y].size(); x++){
            printPixel(frameScheme[y][x]);
        }
        cout << "\033[0m\n";
    }
}

Frame makeBoardFrame(const Board& gameBoard){
    Frame boardFrame = {
        {gray, gray, gray, gray, gray, gray, gray, gray, gray, gray, gray, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, black, black, black, black, black, black, black, black, black, black, gray},
        {gray, gray, gray, gray, gray, gray, gray, gray, gray, gray, gray, gray}
    };
    
    for (const auto& food : gameBoard.at('f')){
        if (food.x >= 0 && food.x < boardSize && food.y >= 0 && food.y < boardSize) {
            boardFrame[food.y + 1][food.x + 1] = red;
        }
    }

    for (size_t i = 0; i < gameBoard.at('s').size(); i++){
        const auto& part = gameBoard.at('s')[i];
        if (part.x >= 0 && part.x < boardSize && part.y >= 0 && part.y < boardSize) {
            if (i == 0) boardFrame[part.y + 1][part.x + 1] = darkGreen;
            else boardFrame[part.y + 1][part.x + 1] = green;
        }
    }
    return boardFrame;
}

// u - up, d - down, l - left, r - right
pair<Board, bool> gameTic(Board gameBoard, char direction){
    auto& snake = gameBoard['s'];
    auto& food = gameBoard['f'];
    if (snake.empty() || food.empty()) {
        return {gameBoard, false};
    }

    for (size_t i = snake.size(); i > 1; --i){
        snake[i - 1] = snake[i - 2];
    }
    switch (direction){
        case 'u':
            snake[0].y -= 1;
            break;
        case 'd':
            snake[0].y += 1;
            break;
        case 'r':
            snake[0].x += 1;
            break;
        case 'l':
            snake[0].x -= 1;
            break;
    }

    if (snake[0].x == food[0].x && snake[0].y == food[0].y){
        snake.push_back(snake.back());

        vector<coordinates> freeCells;
        for (int y = 0; y < boardSize; ++y) {
            for (int x = 0; x < boardSize; ++x) {
                bool occupied = false;
                for (const auto& part : snake) {
                    if (part.x == x && part.y == y) {
                        occupied = true;
                        break;
                    }
                }
                if (!occupied) {
                    freeCells.push_back({y, x});
                }
            }
        }
        if (freeCells.empty()) {
            return {gameBoard, false};
        }
        static std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> distribution(0, freeCells.size() - 1);
        food[0] = freeCells[distribution(generator)];

    }

    for (size_t i = 1; i < snake.size(); ++i){
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return {gameBoard, false};
        }
    }

    if (snake[0].x < 0 || snake[0].x >= boardSize ||
        snake[0].y < 0 || snake[0].y >= boardSize){
        return {gameBoard, false};
    }
    return {gameBoard, true};
}

char getDirection(char currentDirection){
    char key = '\0';
    if (read(STDIN_FILENO, &key, 1) <= 0) {
        return currentDirection;
    }
    switch (key) {
        case 'w':
            if (currentDirection != 'd') return 'u';
            break;
        case 's':
            if (currentDirection != 'u') return 'd';
            break;
        case 'a':
            if (currentDirection != 'r') return 'l';
            break;
        case 'd':
            if (currentDirection != 'l') return 'r';
            break;
        case 'q':
            return 'q';
    }
    return currentDirection;
}

void gameLoop(Board startBoard){
    termios oldSettings;
    if (tcgetattr(STDIN_FILENO, &oldSettings) == -1) {
        return;
    }

    termios settings = oldSettings;
    settings.c_lflag &= ~(ICANON | ECHO);
    settings.c_cc[VMIN] = 0;
    settings.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &settings) == -1) {
        return;
    }

    std::cout << "\033[?25l" << std::flush;

    cout << "\033[2J\033[H";

    Board nowBoard = startBoard;
    char nowDirection = 'u';
    while (true){
        nowDirection = getDirection(nowDirection);
        if (nowDirection == 'q') break;
        pair<Board, bool> result = gameTic(nowBoard, nowDirection);
        if (!result.second) {
            break;
        }
        nowBoard = result.first;
        cout << "\033[H";
        drawFrame(makeBoardFrame(nowBoard));
        this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    std::cout << "\033[?25h\n" << std::flush;
}

int main() {
    // test initial board
    Board board;
    board['f'] = {{2, 2}};
    board['s'] = {{7, 3}, {8, 3}, {9, 3}};

    drawFrame(makeBoardFrame(board));
    gameLoop(board);

    return 0;
}