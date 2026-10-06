#define frame vector<vector<RGB>>
#include <iostream>
#include <vector>
using namespace std;

struct RGB {
    int r;
    int g;
    int b;
};



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

void drawFrame(frame frameSheme){
    cout << "\033[?25l"; // скрыть курсор
    for(int y = 0; y < frameSheme.size(); y++){
        for(int x = 0; x < frameSheme[y].size(); x++){
            printPixel(frameSheme[y][x]);
        }
        cout << "\033[0m\n";
    }
    cout << "\033[0m\033[?25h\n"; // вернуть курсор
}



int main() {

    RGB p{166,112,229};
    RGB w{256,255,255};
    RGB b{0,0,0};

    frame cpaseInvadersEnemy = {
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
    // p - еда
    // S - голова змейки
    // s - тело змейки
    // vector<vector<char>> gameBoard = {};
    char a = 'S';
    char f = 's';
    cout << a << f;





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