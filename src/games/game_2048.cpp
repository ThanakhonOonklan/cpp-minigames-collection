#include "games/game_2048.h"
#include "utils/console_utils.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <conio.h>

namespace Games {
namespace Game2048 {

namespace {

const std::string SCORE_FILE = "data/2048.txt";
const int SIZE_2048 = 4;
int currentScore2048 = 0;
std::string playerName;

void displayBoard(const std::vector<std::vector<int>>& board) {
    Utils::clearScreen();
    std::cout << "Score: " << currentScore2048 << "\n";
    std::cout << "=====================\n";

    for (int i = 0; i < SIZE_2048; ++i) {
        std::cout << "|";
        for (int j = 0; j < SIZE_2048; ++j) {
            if (board[i][j] == 0) {
                std::cout << "    |";
            } else {
                switch (board[i][j]) {
                    case 2:    std::cout << Utils::Color::BLUE; break;
                    case 4:    std::cout << Utils::Color::GREEN; break;
                    case 8:    std::cout << Utils::Color::YELLOW; break;
                    case 16:   std::cout << Utils::Color::RED; break;
                    case 32:   std::cout << Utils::Color::MAGENTA; break;
                    case 64:   std::cout << Utils::Color::CYAN; break;
                    case 128:  std::cout << Utils::Color::WHITE; break;
                    case 256:  std::cout << Utils::Color::GRAY; break;
                    default:   std::cout << Utils::Color::BOLD; break;
                }
                std::cout << std::setw(4) << board[i][j] << Utils::Color::RESET << "|";
            }
        }
        std::cout << "\n=====================\n";
    }

    std::cout << "\nW [Up]  A [Left] S [Down] D [Right]  (Q to quit)\n";
}

void addNewNumber(std::vector<std::vector<int>>& board) {
    int x, y;
    do {
        x = std::rand() % SIZE_2048;
        y = std::rand() % SIZE_2048;
    } while (board[x][y] != 0);
    board[x][y] = (std::rand() % 2 + 1) * 2;
}

bool moveAndMerge(std::vector<std::vector<int>>& board, char direction) {
    bool moved = false;

    if (direction == 'w' || direction == 'W') {
        for (int j = 0; j < SIZE_2048; ++j) {
            int lastMergedRow = -1;
            for (int i = 1; i < SIZE_2048; ++i) {
                if (board[i][j] != 0) {
                    int currentRow = i;
                    while (currentRow > 0 && board[currentRow - 1][j] == 0) {
                        board[currentRow - 1][j] = board[currentRow][j];
                        board[currentRow][j] = 0;
                        currentRow--;
                        moved = true;
                    }
                    if (currentRow > 0 && board[currentRow - 1][j] == board[currentRow][j] && lastMergedRow != currentRow - 1) {
                        board[currentRow - 1][j] *= 2;
                        currentScore2048 += board[currentRow - 1][j];
                        board[currentRow][j] = 0;
                        lastMergedRow = currentRow - 1;
                        moved = true;
                    }
                }
            }
        }
    } else if (direction == 's' || direction == 'S') {
        for (int j = 0; j < SIZE_2048; ++j) {
            int lastMergedRow = SIZE_2048;
            for (int i = SIZE_2048 - 2; i >= 0; --i) {
                if (board[i][j] != 0) {
                    int currentRow = i;
                    while (currentRow < SIZE_2048 - 1 && board[currentRow + 1][j] == 0) {
                        board[currentRow + 1][j] = board[currentRow][j];
                        board[currentRow][j] = 0;
                        currentRow++;
                        moved = true;
                    }
                    if (currentRow < SIZE_2048 - 1 && board[currentRow + 1][j] == board[currentRow][j] && lastMergedRow != currentRow + 1) {
                        board[currentRow + 1][j] *= 2;
                        currentScore2048 += board[currentRow + 1][j];
                        board[currentRow][j] = 0;
                        lastMergedRow = currentRow + 1;
                        moved = true;
                    }
                }
            }
        }
    } else if (direction == 'a' || direction == 'A') {
        for (int i = 0; i < SIZE_2048; ++i) {
            int lastMergedCol = -1;
            for (int j = 1; j < SIZE_2048; ++j) {
                if (board[i][j] != 0) {
                    int currentCol = j;
                    while (currentCol > 0 && board[i][currentCol - 1] == 0) {
                        board[i][currentCol - 1] = board[i][currentCol];
                        board[i][currentCol] = 0;
                        currentCol--;
                        moved = true;
                    }
                    if (currentCol > 0 && board[i][currentCol - 1] == board[i][currentCol] && lastMergedCol != currentCol - 1) {
                        board[i][currentCol - 1] *= 2;
                        currentScore2048 += board[i][currentCol - 1];
                        board[i][currentCol] = 0;
                        lastMergedCol = currentCol - 1;
                        moved = true;
                    }
                }
            }
        }
    } else if (direction == 'd' || direction == 'D') {
        for (int i = 0; i < SIZE_2048; ++i) {
            int lastMergedCol = SIZE_2048;
            for (int j = SIZE_2048 - 2; j >= 0; --j) {
                if (board[i][j] != 0) {
                    int currentCol = j;
                    while (currentCol < SIZE_2048 - 1 && board[i][currentCol + 1] == 0) {
                        board[i][currentCol + 1] = board[i][currentCol];
                        board[i][currentCol] = 0;
                        currentCol++;
                        moved = true;
                    }
                    if (currentCol < SIZE_2048 - 1 && board[i][currentCol + 1] == board[i][currentCol] && lastMergedCol != currentCol + 1) {
                        board[i][currentCol + 1] *= 2;
                        currentScore2048 += board[i][currentCol + 1];
                        board[i][currentCol] = 0;
                        lastMergedCol = currentCol + 1;
                        moved = true;
                    }
                }
            }
        }
    }

    return moved;
}

bool checkWin(const std::vector<std::vector<int>>& board) {
    for (const auto& row : board) {
        for (const auto& num : row) {
            if (num == 2048) {
                return true;
            }
        }
    }
    return false;
}

bool checkLose(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < SIZE_2048; ++i) {
        for (int j = 0; j < SIZE_2048; ++j) {
            if (board[i][j] == 0) return false;
            if (j < SIZE_2048 - 1 && board[i][j] == board[i][j + 1]) return false;
            if (i < SIZE_2048 - 1 && board[i][j] == board[i + 1][j]) return false;
        }
    }
    return true;
}

void saveHighScore(const std::string& name, int score) {
    std::ofstream outFile(SCORE_FILE, std::ios::app);
    if (outFile.is_open()) {
        outFile << name << " " << score << std::endl;
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing!" << std::endl;
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

void run2048Session() {
    std::cout << "Enter your name: ";
    std::cin >> playerName;

    std::srand(static_cast<unsigned int>(std::time(0)));
    currentScore2048 = 0;

    std::vector<std::vector<int>> board(SIZE_2048, std::vector<int>(SIZE_2048, 0));
    addNewNumber(board);
    addNewNumber(board);

    while (true) {
        displayBoard(board);

        char move = _getch();
        if (move == 'q' || move == 'Q') break;

        bool moved = moveAndMerge(board, move);
        if (moved) {
            addNewNumber(board);
        }

        if (checkWin(board)) {
            displayBoard(board);
            std::cout << Utils::Color::GREEN << "You win!" << Utils::Color::RESET << std::endl;
            break;
        }

        if (checkLose(board)) {
            displayBoard(board);
            std::cout << Utils::Color::RED << "You lose!" << Utils::Color::RESET << std::endl;
            break;
        }
    }

    saveHighScore(playerName, currentScore2048);
    std::cout << "\nGame finished! High scores:\n";
    displayHighScores();
}

void showMenu() {
    std::vector<std::string> options = {
        "Play Game",
        "View High Scores",
        "Exit"
    };
    int selectedOption = 0;

    while (true) {
        Utils::clearScreen();

        std::cout << " _____   _____     ___   _____ \n";
        std::cout << "/ __  \\ |  _  |   /   | |  _  |\n";
        std::cout << "`' / /' | |/' |  / /| |  \\ V / \n";
        std::cout << "  / /   |  /| | / /_| |  / _ \\ \n";
        std::cout << "./ /___ \\ |_/ / \\___  | | |_| |\n";
        std::cout << "\\_____/  \\___/      |_| \\_____/ \n";
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
                run2048Session();
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

} // namespace Game2048
} // namespace Games
