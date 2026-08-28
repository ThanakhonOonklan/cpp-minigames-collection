#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

#include "utils/console_utils.h"
#include "games/number_guess.h"
#include "games/tic_tac_toe.h"
#include "games/hangman.h"
#include "games/snake.h"
#include "games/rock_paper_scissors.h"
#include "games/sudoku.h"
#include "games/game_2048.h"

void printMenu(const std::vector<std::string>& options, int selectedOption) {
    Utils::clearScreen();
    std::cout << "  __  __    _____    _   _    _   _  \n";
    std::cout << " |  \\/  |  | ____|  | \\ | |  | | | | \n";
    std::cout << " | |\\/| |  |  _|    |  \\| |  | | | | \n";
    std::cout << " | |  | |  | |___   | |\\  |  | |_| | \n";
    std::cout << " |_|  |_|  |_____|  |_| \\_|   \\___/  \n";
    std::cout << "                                     \n";
    std::cout << "=====================================\n";

    for (size_t i = 0; i < options.size(); ++i) {
        if (static_cast<int>(i) == selectedOption) {
            std::cout << Utils::Color::GREEN << " > " << options[i] << " < " << Utils::Color::RESET << "\n";
        } else {
            std::cout << "   " << options[i] << "\n";
        }
    }
}

int main() {
    std::vector<std::string> options = {
        "1. Number Guess Game",
        "2. Tic-Tac-Toe",
        "3. Hangman",
        "4. Snake Game",
        "5. Rock, Paper, Scissors",
        "6. Sudoku",
        "7. 2048",
        "8. Exit"
    };
    int selectedOption = 0;

    while (true) {
        printMenu(options, selectedOption);

        char key = _getch();
        if (key == 'w' || key == 'W') {
            selectedOption = (selectedOption - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
        } else if (key == 's' || key == 'S') {
            selectedOption = (selectedOption + 1) % static_cast<int>(options.size());
        } else if (key == '\r') {
            Utils::clearScreen();
            switch (selectedOption) {
                case 0:
                    Games::NumberGuess::play();
                    break;
                case 1:
                    Games::TicTacToe::play();
                    break;
                case 2:
                    Games::Hangman::play();
                    break;
                case 3:
                    Games::Snake::play();
                    break;
                case 4:
                    Games::RockPaperScissors::play();
                    break;
                case 5:
                    Games::Sudoku::play();
                    break;
                case 6:
                    Games::Game2048::play();
                    break;
                case 7:
                    std::cout << "Thank you for playing! Exiting...\n";
                    return 0;
            }
        }
    }

    return 0;
}
