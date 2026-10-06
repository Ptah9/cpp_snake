#include <iostream>
#include <linux/limits.h>
#include <vector>
using namespace std;
struct RGB {
    int r;
    int g;
    int b;
};


using Frame = std::vector<std::vector<RGB>>;
using Board = std::vector<std::vector<char>>;



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
    for(int y = 0; y < frameScheme.size(); y++){
        for(int x = 0; x < frameScheme[y].size(); x++){
            printPixel(frameScheme[y][x]);
        }
        cout << "\033[0m\n";
    }
    cout << "\033[0m\033[?25h\n"; // вернуть курсор
}

Frame makeBoardFrame(Board gameBoard){
    const RGB black {0,0,0}; 
    const RGB gray {128,128, 128}; 
    const RGB red {255,0,0}; 
    const RGB green {0,255,0}; 
    const RGB blue {0,0,255}; 
    const RGB purpNeon {191,0,255};
    Frame boardFrame;
    for(int y = 0; y < gameBoard.size(); y++){
        boardFrame.push_back({});
        for(int x = 0; x < gameBoard[y].size(); x++){
            switch (gameBoard[y][x]) {
                case '0':
                    boardFrame[y].push_back(black);
                    break;
                case 'w':
                    boardFrame[y].push_back(gray);
                    break;
                case 'f':
                    boardFrame[y].push_back(red);
                    break;
                case 'S':
                    boardFrame[y].push_back(blue);
                    break;
                case 's':
                    boardFrame[y].push_back(green);
                    break; 
                default:
                    boardFrame[y].push_back(purpNeon);
            }
        }
    }
    return boardFrame;
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
    Board gameBoard = {
        {'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'w'},
        {'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'}
    };

    drawFrame(makeBoardFrame(gameBoard));





    return 0;
}

    // const int width = 500;
    // const int height = 500;

    // // Очистить терминал один раз и поставить курсор в начало
    // cout << "\033[2J\033[H";

    // // Спрятать курсор
    // cout << "\033[?25l";

    // for (int frame = 0; frame < 300; ++frame) {
    //     // Вернуться к левому верхнему углу для следующего кадра
    //     cout << "\033[H";

    //     for (int y = 0; y < height; ++y) {
    //         for (int x = 0; x < width; ++x) {
    //             int r = x * 255 / (width - 1);
    //             int g = y * 255 / (height - 1);
    //             int b = (frame * 2) % 256;

    //             printPixel(r, g, b);
    //         }

    //         // Сброс фона перед переносом строки
    //         cout << "\033[0m\n";
    //     }

    //     cout.flush();

    //     this_thread::sleep_for(
    //         chrono::milliseconds(30)
    //     );
    // }

    // // Вернуть обычные цвета и курсор
    // cout << "\033[0m\033[?25h\n";
// }