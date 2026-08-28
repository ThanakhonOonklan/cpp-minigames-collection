#include "games/snake.h"
#include "utils/console_utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <thread>
#include <chrono>

namespace Games {
namespace Snake {

namespace {

const std::string SCORE_FILE = "data/snake.txt";
const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

int headX, headY, fruitX, fruitY, currentScore;
int tailX[200], tailY[200];
int tailLength;
Direction dir;
std::string playerName;
bool isGameOver = false;

void setup() {
    dir = STOP;
    headX = WIDTH / 2;
    headY = HEIGHT / 2;
    fruitX = std::rand() % WIDTH;
    fruitY = std::rand() % HEIGHT;
    currentScore = 0;
    tailLength = 0;
    isGameOver = false;
}

void draw() {
    Utils::setCursorPosition(0, 0);

    for (int i = 0; i < WIDTH + 2; i++) {
        std::cout << "#";
    }
    std::cout << "\n";

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) {
                std::cout << "#";
            }

            if (i == headY && j == headX) {
                std::cout << Utils::Color::GREEN << "O" << Utils::Color::RESET;
            } else if (i == fruitY && j == fruitX) {
                std::cout << Utils::Color::YELLOW << "F" << Utils::Color::RESET;
            } else {
                bool isTail = false;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        std::cout << Utils::Color::BRIGHT_GREEN << "o" << Utils::Color::RESET;
                        isTail = true;
                        break;
                    }
                }
                if (!isTail) {
                    std::cout << " ";
                }
            }

            if (j == WIDTH - 1) {
                std::cout << "#";
            }
        }
        std::cout << "\n";
    }

    for (int i = 0; i < WIDTH + 2; i++) {
        std::cout << "#";
    }
    std::cout << "\nPlayer Score: " << currentScore << "    \n";
}

void processInput() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': case 'A':
                if (dir != RIGHT || tailLength == 0) dir = LEFT;
                break;
            case 'd': case 'D':
                if (dir != LEFT || tailLength == 0) dir = RIGHT;
                break;
            case 'w': case 'W':
                if (dir != DOWN || tailLength == 0) dir = UP;
                break;
            case 's': case 'S':
                if (dir != UP || tailLength == 0) dir = DOWN;
                break;
            case 'q': case 'Q':
                isGameOver = true;
                break;
        }
    }
}

void saveHighScore() {
    std::ofstream file(SCORE_FILE, std::ios::app);
    if (file.is_open()) {
        file << playerName << " " << currentScore << std::endl;
        file.close();
    }
}

void updateLogic() {
    if (isGameOver || dir == STOP) return;

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;

    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:  headX--; break;
        case RIGHT: headX++; break;
        case UP:    headY--; break;
        case DOWN:  headY++; break;
        default: break;
    }

    // Check boundary collision
    if (headX >= WIDTH || headX < 0 || headY >= HEIGHT || headY < 0) {
        isGameOver = true;
        Utils::setCursorPosition(WIDTH / 2 - 5, HEIGHT / 2);
        std::cout << Utils::Color::BLUE << "Game Over!" << Utils::Color::RESET << "\n";
        saveHighScore();
        return;
    }

    // Check tail collision
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == headX && tailY[i] == headY) {
            isGameOver = true;
            Utils::setCursorPosition(WIDTH / 2 - 5, HEIGHT / 2);
            std::cout << Utils::Color::BLUE << "Game Over!" << Utils::Color::RESET << "\n";
            saveHighScore();
            return;
        }
    }

    // Check eating fruit
    if (headX == fruitX && headY == fruitY) {
        currentScore += 10;
        fruitX = std::rand() % WIDTH;
        fruitY = std::rand() % HEIGHT;
        if (tailLength < 199) {
            tailLength++;
        }
    }
}

void displayHighScores() {
    std::ifstream inFile(SCORE_FILE);
    std::vector<std::pair<std::string, int>> scores;
    std::string name;
    int score;

    if (inFile.is_open()) {
        while (inFile >> name >> score) {
            scores.emplace_back(name, score);
        }
        inFile.close();

        std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });

        std::cout << "\n=====================================" << std::endl;
        std::cout << "           Top 10 High Scores        " << std::endl;
        std::cout << "=====================================" << std::endl;
        for (size_t i = 0; i < std::min(size_t(10), scores.size()); ++i) {
            std::cout << i + 1 << ". " << scores[i].first << " - " << scores[i].second << std::endl;
        }
        std::cout << "=====================================" << std::endl;
    } else {
        std::cerr << "Error opening file for reading!" << std::endl;
    }

    std::cout << "\nPress Enter key to return to menu..." << std::endl;
    _getch();
}

void runSnakeSession() {
    Utils::hideCursor();
    std::cout << "Enter your name: ";
    std::cin >> playerName;

    while (true) {
        setup();
        Utils::clearScreen();

        while (!isGameOver) {
            draw();
            processInput();
            updateLogic();
            std::this_thread::sleep_for(std::chrono::milliseconds(90));
        }

        Utils::showCursor();
        std::cout << "\nPress Enter to play again or '0' to exit: ";
        char choice = _getch();
        if (choice == '0') {
            return;
        }
        Utils::hideCursor();
    }
}

void showMenu() {
    std::vector<std::string> options = {
        "Play Game",
        "View High Scores",
        "Exit"
    };
    int selectedOption = 0;

    while (true) {
        Utils::showCursor();
        Utils::clearScreen();

        std::cout << " _____   _   _    ___    _   __  _____  \n";
        std::cout << "/  ___| | \\ | |  / _ \\  | | / / |  ___| \n";
        std::cout << "\\ `--.  |  \\| | / /_\\ \\ | |/ /  | |__   \n";
        std::cout << " `--. \\ | . ` | |  _  | |    \\  |  __|  \n";
        std::cout << "/\\__/ / | |\\  | | | | | | |\\  \\ | |___  \n";
        std::cout << "\\____/  \\_| \\_/ \\_| |_/ \\_| \\_/ \\____/  \n";
        std::cout << "=====================================" << std::endl;

        for (size_t i = 0; i < options.size(); ++i) {
            if (static_cast<int>(i) == selectedOption) {
                std::cout << Utils::Color::GREEN << " > " << options[i] << " < " << Utils::Color::RESET << std::endl;
            } else {
                std::cout << "   " << options[i] << std::endl;
            }
        }

        char key = _getch();
        if (key == 'w' || key == 'W') {
            selectedOption = (selectedOption - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
        } else if (key == 's' || key == 'S') {
            selectedOption = (selectedOption + 1) % static_cast<int>(options.size());
        } else if (key == '\r') {
            Utils::clearScreen();
            if (selectedOption == 0) {
                runSnakeSession();
            } else if (selectedOption == 1) {
                displayHighScores();
            } else if (selectedOption == 2) {
                std::cout << "Exit the game" << std::endl;
                break;
            }
        }
    }
}

} // namespace

int play() {
    showMenu();
    return 0;
}

} // namespace Snake
} // namespace Games
