#include <iostream>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
#ifdef _WIN32
#include <conio.h>  // for _kbhit() and _getch() on Windows
#include <windows.h> // for Sleep() on Windows
#else
#include <unistd.h> // for usleep() on POSIX
#include <termios.h> // for terminal I/O on POSIX
#include <fcntl.h>   // for fcntl() on POSIX
#endif

char** getCharArray(int size) {
    char** map = new char*[size];
    for (int i = 0; i < size; ++i) {
        map[i] = new char[size];
    }

    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            map[i][j] = 'A' + (rand() % 26); // Initialize with random uppercase letters
        }
    }

    return map;
}

void deleteCharArray(char** array, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

void printCharArray(char** map, int size) {
    std::cout << "\033[H"; // Move cursor to home position
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            if (j == 0 || j == size - 1 || i == 0 || i == size - 1) {
                std::cout << '0' << " ";
            } else {
                std::cout << map[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

struct Position {
    int x;
    int y;
};

Position updateArrayWithKeyPress(char** map, int size, char key, int x, int y) {
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            map[i][j] = ' ';
        }
    }
    switch (key) {
        case 'w':
            if (y > 1) y--;
            break;
        case 'a':
            if (x > 1) x--;
            break;
        case 's':
            if (y < size - 2) y++;
            break;
        case 'd':
            if (x < size - 2) x++;
            break;
        default:
            std::cout << "Big Bad";
    }
    /*if (key == 'w'){
        map[x][y] = 'O';
        y = y - 1;
        map[x][y] = 'X';
    }
    else if (key == 'a'){
        map[x][y] = 'O';
        x = x - 1;
        map[x][y] = 'X';
    }
    else if (key == 's'){
        map[x][y] = 'O';
        y = y + 1;
        map[x][y] = 'X';
    }
    else if (key == 'd'){
        map[x][y] = 'O';
        x = x + 1;
        map[x][y] = 'X';
    }
    else {
        std::cout << "Big Bad";
    }*/
    map[x][y] = 'X';
    return {x, y};
}


int gameloop(char** map, int size, Position pos){
    #ifdef _WIN32
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) break; // Exit on ESC key
            pos = updateArrayWithKeyPress(map, size, key, pos.x, pos.y);
            printCharArray(map, size);
            Sleep(10); // Delay for readability
        }
    }
    #else
        // POSIX setup for non-blocking input
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
        int counter;
        while (true) {
            char key;
            ssize_t n = read(STDIN_FILENO, &key, 1);
            if (n > 0) {
                if (key == 27) break; // Exit on ESC key
                pos = updateArrayWithKeyPress(map, size, key, pos.x, pos.y);
                printCharArray(map, size);
                //usleep(10); // Delay for readability
            }
            else {
                printCharArray(map, size);
            }
            counter++;
            std::cout<<counter;
            usleep(17000);
        }

        // Restore terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif

        deleteCharArray(map, size); // Free the allocated memory
        return 0;
}


int main() {
    std::srand(std::time(0)); // Initialize random seed (???????????)
    int size = 10; // Fixed size for testing
    
    Position pos = {size / 2, size / 2};

    char** map = getCharArray(size);

    std::cout << "\033[2J"; // Clear the screen

    gameloop(map, size, pos);

/*#ifdef _WIN32
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) break; // Exit on ESC key
            pos = updateArrayWithKeyPress(map, size, key, pos.x, pos.y);
            printCharArray(map, size);
            Sleep(10); // Delay for readability
        }
    }
#else
    // POSIX setup for non-blocking input
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    int counter;
    while (true) {
        char key;
        ssize_t n = read(STDIN_FILENO, &key, 1);
        if (n > 0) {
            if (key == 27) break; // Exit on ESC key
            pos = updateArrayWithKeyPress(map, size, key, pos.x, pos.y);
            printCharArray(map, size);
            //usleep(10); // Delay for readability
        }
        else {
            printCharArray(map, size);
        }
        counter++;
        std::cout<<counter;
        usleep(17000);
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    deleteCharArray(map, size); // Free the allocated memory
    return 0;*/
}
