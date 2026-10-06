#include <cstddef>
#include <iostream>
#include <linux/limits.h>
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
void oneMorePixel() {
    cout << "  ";
}

void drawFrame(Frame frameScheme){
    cout << "\033[?25l"; // скрыть курсор
    for(size_t y = 0; y < frameScheme.size(); y++){
        for(size_t x = 0; x < frameScheme[y].size(); x++){
            printPixel(frameScheme[y][x]);
        }
        cout << "\033[0m\n";
    }
}

Frame makeBoardFrame(Board gameBoard){
    const RGB black {0,0,0}; 
    const RGB gray {128,128, 128}; 
    const RGB red {255,0,0}; 
    const RGB darkGreen {1,50,32}; 
    const RGB green {0,255,0}; 
    // const RGB blue {0,0,255}; 
    // const RGB purpNeon {191,0,255};
    
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
    
    for (size_t i = 0; i < gameBoard['f'].size(); i++){
        boardFrame[gameBoard['f'][i].y + 1][gameBoard['f'][i].x + 1] = red;
    }

    for (size_t i = 0; i < gameBoard['s'].size(); i++){
        if (i == 0) boardFrame[gameBoard['s'][i].y + 1][gameBoard['s'][i].x + 1] = darkGreen;
        else boardFrame[gameBoard['s'][i].y + 1][gameBoard['s'][i].x + 1] = green;
    }
    return boardFrame;
}

// u - up, d - down, l - left, r - right
pair<Board, bool> gameTic(Board gameBoard, char direction){
    for (size_t i = gameBoard['s'].size()-1; i >= 1; i--){
        gameBoard['s'][i].x = gameBoard['s'][i-1].x;
        gameBoard['s'][i].y = gameBoard['s'][i-1].y;
    }
    switch (direction){
        case 'u':
            gameBoard['s'][0].y -= 1;
            break;
        case 'd':
            gameBoard['s'][0].y += 1;
            break;
        case 'r':
            gameBoard['s'][0].x += 1;
            break;
        case 'l':
            gameBoard['s'][0].x -= 1;
            break;
    }

    if (gameBoard['s'][0].x == gameBoard['f'][0].x && gameBoard['s'][0].y == gameBoard['f'][0].y){
        gameBoard['s'].push_back({gameBoard['s'][gameBoard['s'].size()-1].y, gameBoard['s'][gameBoard['s'].size()-1].x});
        int xa, ya;
        size_t flag = gameBoard['s'].size()-1;
        while(flag == gameBoard['s'].size()-1){
            flag = 0;
            xa = rand() % 10; ya = rand() % 10;
            for (size_t i = 0; i < gameBoard['s'].size(); i++){
            if (!(xa == gameBoard['s'][i].x && ya == gameBoard['s'][i].y)) flag+=1;
        }
    }
        gameBoard['f'][0] = {ya, xa};

    }

    for (size_t i = gameBoard['s'].size()-1; i >= 1; i--){
        if (gameBoard['s'][0].x == gameBoard['s'][i].x && gameBoard['s'][0].y == gameBoard['s'][i].y) return {gameBoard, false};
    }

    if (gameBoard['s'][0].x < 0 || gameBoard['s'][0].x > 9 || gameBoard['s'][0].y < 0 || gameBoard['s'][0].y > 9){
        return {gameBoard, false};
    }
    return {gameBoard, true};
}

char getDirection(char currentDirection){
    char key = '\0';
    read(STDIN_FILENO, &key, 1);
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
    tcgetattr(STDIN_FILENO, &oldSettings);

    termios settings = oldSettings;
    settings.c_lflag &= ~(ICANON | ECHO);
    settings.c_cc[VMIN] = 0;
    settings.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
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
}

int main() {

    RGB p{166,112,229};
    RGB w{256,255,255};
    RGB b{0,0,0};

    Frame cpaseInvadersEnemy = {
        {w, w, w, b, b, b, w, w, w, w, w, w, w, w, b, b, b, w, w, w},
        {w, w, w, b, p, b, b, b, b, w, w, b, b, b, b, p, b, w, w, w},
        {w, w, b, b, b, b, b, p, b, w, w, b, p, b, b, b, b, b, w, w},
        {w, w, b, p, p, b, b, p, b, b, b, b, p, b, b, p, p, b, w, w},
        {b, b, b, p, b, b, p, p, p, p, p, p, p, p, b, b, p, b, b, b},
        {b, p, b, b, b, p, p, p, p, p, p, p, p, p, p, b, b, b, p, b},
        {b, p, b, b, p, p, p, p, p, p, p, p, p, p, p, p, b, b, p, b},
        {b, p, b, b, p, p, b, b, p, p, p, p, b, b, p, p, b, b, p, b},
        {b, p, b, b, p, p, b, b, p, p, p, p, b, b, p, p, b, b, p, b},
        {b, b, p, p, p, p, b, b, p, p, p, p, b, b, p, p, p, p, b, b},
        {w, b, b, b, p, p, p, p, p, p, p, p, p, p, p, p, b, b, b, w},
        {w, w, w, b, p, p, p, p, p, p, p, p, p, p, p, p, b, w, w, w},
        {w, w, w, b, b, b, p, p, b, b, b, b, p, p, b, b, b, w, w, w},
        {w, w, w, w, w, b, p, p, b, w, w, b, p, p, b, w, w, w, w, w},
        {w, w, w, b, b, b, b, b, b, w, w, b, b, b, b, b, b, w, w, w},
        {w, w, w, b, p, b, w, w, w, w, w, w, w, w, b, p, b, w, w, w},
        {w, w, w, b, b, b, w, w, w, w, w, w, w, w, b, b, b, w, w, w}
    };

    // 0 - пустота
    // w - стена
    // f - еда
    // S - голова змейки
    // s - тело змейки

    unordered_map<char, vector<coordinates>> board;
    board['f'] = {{2, 2}};
    board['s'] = {{8, 3}, {9, 3}, {10, 3}};

    drawFrame(makeBoardFrame(board));
    gameLoop(board);



    cout << "\033[0m\033[?25h\n"; // вернуть курсор
    return 0;
}