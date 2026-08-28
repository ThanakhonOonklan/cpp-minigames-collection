#include "games/sudoku.h"
#include "utils/console_utils.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include <conio.h>

namespace Games {
namespace Sudoku {

namespace {

const std::string SCORE_FILE = "data/sudoku.txt";
const int BOARD_SIZE = 9;

time_t gameStartTime = 0;
std::vector<std::vector<int>> currentBoard(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
int cursorRow = 0;
int cursorCol = 0;

std::string formatTime(time_t timeInSeconds) {
    long hours = timeInSeconds / 3600;
    long minutes = (timeInSeconds % 3600) / 60;
    long seconds = timeInSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}

std::string getElapsedTime() {
    return formatTime(static_cast<time_t>(std::difftime(std::time(0), gameStartTime)));
}

void startTimer() {
    gameStartTime = std::time(0);
}

void displayBoard() {
    std::cout << "\nTime: " << getElapsedTime() << "\n-------------------------\n";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (i == cursorRow && j == cursorCol) {
                std::cout << Utils::Color::GREEN << (currentBoard[i][j] == 0 ? "." : std::to_string(currentBoard[i][j])) << Utils::Color::RESET << " ";
            } else {
                std::cout << (currentBoard[i][j] == 0 ? "\033[37m.\033[0m " : std::to_string(currentBoard[i][j]) + " ");
            }
            if ((j + 1) % 3 == 0) std::cout << "| ";
        }
        std::cout << "\n";
        if ((i + 1) % 3 == 0) std::cout << "-------------------------\n";
    }
}

void randomizeBoard() {
    currentBoard = std::vector<std::vector<int>>(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
    std::default_random_engine rng(static_cast<unsigned int>(std::time(0)));

    for (int gridRow = 0; gridRow < 3; ++gridRow) {
        for (int gridCol = 0; gridCol < 3; ++gridCol) {
            std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            std::shuffle(numbers.begin(), numbers.end(), rng);
            int index = 0;
            for (int r = gridRow * 3; r < gridRow * 3 + 3; ++r) {
                for (int c = gridCol * 3; c < gridCol * 3 + 3; ++c) {
                    currentBoard[r][c] = (index < 3 + std::rand() % 2) ? numbers[index++] : 0;
                }
            }
        }
    }
}

bool isValid(int row, int col, int num) {
    for (int j = 0; j < BOARD_SIZE; ++j) {
        if (currentBoard[row][j] == num) return false;
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (currentBoard[i][col] == num) return false;
    }
    int boxStartRow = row - row % 3;
    int boxStartCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (currentBoard[boxStartRow + i][boxStartCol + j] == num) return false;
        }
    }
    return true;
}

void saveHighScore(const std::string& playerName, const std::string& timeString) {
    std::ofstream outFile(SCORE_FILE, std::ios::app);
    if (outFile.is_open()) {
        outFile << playerName << " : " << timeString << std::endl;
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing!" << std::endl;
    }
}

void displayHighScores() {
    std::ifstream inFile(SCORE_FILE);
    std::vector<std::pair<std::string, std::string>> scores;
    std::string name, timeString;

    if (inFile.is_open()) {
        while (inFile >> name >> std::ws && std::getline(inFile, timeString)) {
            scores.emplace_back(name, timeString);
        }
        inFile.close();

        std::cout << "\n=====================================\n           Top 10 High Scores\n=====================================\n";
        for (size_t i = 0; i < scores.size() && i < 10; ++i) {
            std::cout << i + 1 << ". " << scores[i].first << " " << scores[i].second << std::endl;
        }
        std::cout << "=====================================\n";
    } else {
        std::cerr << "Error opening file for reading!" << std::endl;
    }

    std::cout << "Press Enter key to return to the Menu..." << std::endl;
    _getch();
}

void playGameSession() {
    Utils::clearScreen();
    std::string playerName;
    std::cout << "Enter your name: ";
    std::cin >> playerName;

    randomizeBoard();
    startTimer();
    cursorRow = 0;
    cursorCol = 0;

    while (true) {
        Utils::clearScreen();
        displayBoard();

        char input;
        std::cout << "\n W [Up]  A [Left] S [Down] D [Right]\nEnter number 1-9 (0 to Exit): ";
        std::cin >> input;
        Utils::clearScreen();

        if (input == '0') {
            saveHighScore(playerName, getElapsedTime());
            break;
        }

        switch (input) {
            case 'w': case 'W': cursorRow = std::max(0, cursorRow - 1); break;
            case 's': case 'S': cursorRow = std::min(BOARD_SIZE - 1, cursorRow + 1); break;
            case 'a': case 'A': cursorCol = std::max(0, cursorCol - 1); break;
            case 'd': case 'D': cursorCol = std::min(BOARD_SIZE - 1, cursorCol + 1); break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': {
                int num = input - '0';
                if (isValid(cursorRow, cursorCol, num)) {
                    currentBoard[cursorRow][cursorCol] = num;
                    std::cout << Utils::Color::GREEN << "Number placed!" << Utils::Color::RESET << std::endl;
                } else {
                    std::cout << Utils::Color::RED << "Invalid move! Try again." << Utils::Color::RESET << std::endl;
                }
                break;
            }
            default:
                std::cout << Utils::Color::YELLOW << "Invalid input! Please try again." << Utils::Color::RESET << std::endl;
                break;
        }

        bool won = std::all_of(currentBoard.begin(), currentBoard.end(), [](const std::vector<int>& r) {
            return std::all_of(r.begin(), r.end(), [](int n) { return n != 0; });
        });

        if (won) {
            std::cout << Utils::Color::GREEN << "Congratulations! You completed the Sudoku!" << Utils::Color::RESET << std::endl;
            saveHighScore(playerName, getElapsedTime());
            std::cout << "Press any key to return to menu...";
            _getch();
            break;
        }
    }
}

void showMenu() {
    std::vector<std::string> options = {"Play Game", "View High Scores", "Exit"};
    int selectedOption = 0;

    while (true) {
        Utils::clearScreen();
        std::cout << "  ____    _   _    ____    ____    _  __    _   _  \n";
        std::cout << " / ___|  | | | |  |  _ \\  / __ \\  | |/ /   | | | | \n";
        std::cout << " \\___ \\  | | | |  | | | || |  | | | ' /    | | | | \n";
        std::cout << "  ___) | | |_| |  | |_| || |__| | | . \\    | |_| | \n";
        std::cout << " |____/   \\___/   |____/  \\____/  |_|\\_\\    \\___/  \n";
        std::cout << "=====================================\n";

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
                playGameSession();
            } else if (selectedOption == 1) {
                displayHighScores();
            } else {
                break;
            }
        }
    }
}

} // namespace

int play() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    showMenu();
    return 0;
}

} // namespace Sudoku
} // namespace Games
