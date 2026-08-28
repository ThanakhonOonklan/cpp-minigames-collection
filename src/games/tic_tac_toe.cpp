#include "games/tic_tac_toe.h"
#include "utils/console_utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>

namespace Games {
namespace TicTacToe {

namespace {

const std::string SCORE_FILE = "data/tic_tac.txt";

struct PlayerScore {
    std::string name;
    int wins;
};

void displayHighScores() {
    std::ifstream inFile(SCORE_FILE);
    std::vector<PlayerScore> scores;
    std::string name;
    int wins;

    if (inFile.is_open()) {
        while (inFile >> name >> wins) {
            scores.push_back({name, wins});
        }
        inFile.close();

        std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return a.wins > b.wins;
        });

        std::cout << "\n=====================================" << std::endl;
        std::cout << "    Top Players High Scores          " << std::endl;
        std::cout << "=====================================" << std::endl;
        for (size_t i = 0; i < scores.size() && i < 10; ++i) {
            std::cout << i + 1 << ". " << scores[i].name << " - " << scores[i].wins << " wins" << std::endl;
        }
        std::cout << "=====================================" << std::endl;
    } else {
        std::cout << "Error opening file for reading!" << std::endl;
    }
}

void saveHighScore(const std::string& playerName, int wins) {
    std::ofstream outFile(SCORE_FILE, std::ios::app);
    if (outFile.is_open()) {
        outFile << playerName << " " << wins << std::endl;
        outFile.close();
    } else {
        std::cout << "Error opening file for writing!" << std::endl;
    }
}

void printBoard(char board[3][3]) {
    std::cout << "----------------------------\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "|        |        |        |\n";
        std::cout << "|    " << (board[i][0] == '1' + i * 3 + 0 ? '.' : board[i][0]) << "   |    " 
                  << (board[i][1] == '1' + i * 3 + 1 ? '.' : board[i][1]) << "   |    " 
                  << (board[i][2] == '1' + i * 3 + 2 ? '.' : board[i][2]) << "   |\n";
        std::cout << "|        |        |        |\n";
        std::cout << "----------------------------\n";
    }
}

bool checkWin(char board[3][3], char player) {
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) || 
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

void resetBoard(char board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = '1' + i * 3 + j;
        }
    }
}

void playGameSession() {
    char board[3][3];
    resetBoard(board);

    std::string player1, player2;
    int turn = 0;
    int choice;
    int wins1 = 0, wins2 = 0;

    std::cout << "Enter Player 1 name (X): ";
    std::cin >> player1;
    std::cout << "Enter Player 2 name (O): ";
    std::cin >> player2;

    while (true) {
        Utils::clearScreen();
        printBoard(board);

        std::cout << "| " << (turn % 2 == 0 ? player1 : player2) << "'s turn. Choose a number (0 to exit): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::getline(std::cin, discard);
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting the game..." << std::endl;
            return;
        }

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (choice < 1 || choice > 9 || board[row][col] == 'X' || board[row][col] == 'O') {
            std::cout << Utils::Color::YELLOW << "Invalid move! Try again." << Utils::Color::RESET << std::endl;
            _getch();
            continue;
        }

        char currentChar = (turn % 2 == 0) ? 'X' : 'O';
        board[row][col] = currentChar;

        if (checkWin(board, currentChar)) {
            Utils::clearScreen();
            printBoard(board);
            if (turn % 2 == 0) {
                std::cout << Utils::Color::GREEN << player1 << " wins!" << Utils::Color::RESET << std::endl;
                wins1++;
                saveHighScore(player1, wins1);
            } else {
                std::cout << Utils::Color::GREEN << player2 << " wins!" << Utils::Color::RESET << std::endl;
                wins2++;
                saveHighScore(player2, wins2);
            }
            std::cout << "\nPress any key to start a new round...";
            _getch();
            resetBoard(board);
            turn = 0;
            continue;
        }

        if (turn == 8) {
            Utils::clearScreen();
            printBoard(board);
            std::cout << Utils::Color::YELLOW << "It's a draw!" << Utils::Color::RESET << std::endl;
            std::cout << "\nPress any key to start a new round...";
            _getch();
            resetBoard(board);
            turn = 0;
            continue;
        }

        turn++;
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
        Utils::clearScreen();
        std::cout << "  _____    ___    ____    _____    _      ____   _____    ____    _____  \n";
        std::cout << " |_   _|  |_ _|  / ___|  |_   _|  / \\   / ___|  |_   _|  / __ \\  | ____|\n";
        std::cout << "   | |     | |  | |        | |   / _ \\  | |       | |   | |  | | |  _|  \n";
        std::cout << "   | |     | |  | |___     | |  /_/ \\_\\ | |___    | |   | |__| | | |___ \n";
        std::cout << "   |_|    |___|  \\____|    |_|  | | | |  \\____|   |_|    \\____/  |_____|\n";
        std::cout << "============================================================================" << std::endl;

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
                std::cout << "\nPress Enter key to return to Menu..." << std::endl;
                _getch();
            } else if (selectedOption == 2) {
                std::cout << "Exiting the game" << std::endl;
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

} // namespace TicTacToe
} // namespace Games
