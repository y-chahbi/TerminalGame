#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#define WIDTH 10
#define HEIGHT 10

int ArrMap[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

class Ascci {
    private:
        std::string     arr[WIDTH][HEIGHT];
        std::mutex      mtx;

    public:
        Ascci() {
            for (int i = 0; i < WIDTH; i++)
                for (int j = 0; j < HEIGHT; j++)
                    arr[i][j] = " ";
        }

        void AscciBackround(std::string PixelType) {
            for (int i = 0; i < WIDTH; i++)
                for (int j = 0; j < HEIGHT; j++)
                    arr[i][j] = PixelType;
        }

        void AscciSetPixel(int x, int y, std::string PixelType) {
            for (int i = 0; i < WIDTH; i++)
                for (int j = 0; j < HEIGHT; j++)
                    if (i == x && j == y)
                        arr[i][j] = PixelType;
        }

        void AscciGetPixel() {
            for (int i = 0; i < WIDTH; i++) {
                for (int j = 0; j < HEIGHT; j++)
                    std::cout << arr[i][j];
                std::cout << std::endl;
            }
        }

        void AsciiClear() {
            for (int i = 0; i < WIDTH; i++)
                for (int j = 0; j < HEIGHT; j++)
                    arr[i][j] = " ";
        }

        int AscciKeyCode() {
            #ifdef _WIN32
                    return _getch();
            #else
                    struct termios oldt, newt;
                    char ch;
                    tcgetattr(STDIN_FILENO, &oldt);
                    newt = oldt;
                    newt.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                    ch = getchar();
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return static_cast<int>(ch);
            #endif
        }
};

void inputThread(Ascci* ascci, int& PosPX, int& PosPY) {
    while (true) {
        if (ascci->AscciKeyCode() == 97) // a
            PosPX--;
        if (ascci->AscciKeyCode() == 100) // d
            PosPX++;
        if (ascci->AscciKeyCode() == 119) // w
            PosPY--;
        if (ascci->AscciKeyCode() == 115) // s
            PosPY++;
    }
}

void renderThread(Ascci* ascci, int& PosPX, int& PosPY) {
    while (true) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (ArrMap[i][j] == 1)
                    ascci->AscciSetPixel(i, j, "█");
                else if (ArrMap[i][j] == 0)
                    ascci->AscciSetPixel(i, j, " ");
            }
        }
        ascci->AscciSetPixel(PosPY, PosPX, "\033[1;31m◎\033[0m");
        ascci->AscciGetPixel();
        ascci->AsciiClear();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        system("clear");
    }
}

int main() {
    Ascci   ascci;
    int     PosPX = 5;
    int     PosPY = 5;

    std::thread input(inputThread, &ascci, std::ref(PosPX), std::ref(PosPY));
    std::thread render(renderThread, &ascci, std::ref(PosPX), std::ref(PosPY));
    
    input.join();
    render.join();

    return 0;
}