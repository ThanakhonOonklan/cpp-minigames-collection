#include "games/number_guess.h"
#include "utils/console_utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <conio.h>

namespace Games {
namespace NumberGuess {

namespace {

const std::string SCORE_FILE = "data/number_guess.txt";

struct PlayerScore {
    std::string name;
    int score;
};

void displayHighScores() {
    std::ifstream inFile(SCORE_FILE);
    std::vector<PlayerScore> scores;
    std::string name;
    int score;

    if (inFile.is_open()) {
        while (inFile >> name >> score) {
            scores.push_back({name, score});
        }
        inFile.close();

        std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score;
        });

        std::cout << "\n=====================================" << std::endl;
        std::cout << "           Top 10 High Scores        " << std::endl;
        std::cout << "=====================================" << std::endl;
        for (size_t i = 0; i < scores.size() && i < 10; ++i) {
            std::cout << i + 1 << ". " << scores[i].name << " - " << scores[i].score << std::endl;
        }
        std::cout << "=====================================" << std::endl;
    } else {
        std::cout << "Error opening file for reading!" << std::endl;
    }
}

void saveHighScore(const std::string& playerName, int score) {
    std::ofstream outFile(SCORE_FILE, std::ios::app);
    if (outFile.is_open()) {
        outFile << playerName << " " << score << std::endl;
        outFile.close();
    } else {
        std::cout << "Error opening file for writing!" << std::endl;
    }
}

void playGameSession() {
    char playAgain;
    std::string playerName;

    std::cout << "Enter your name: ";
    std::cin >> playerName;

    do {
        Utils::clearScreen();
        std::srand(static_cast<unsigned int>(std::time(0)));

        int randomNumber = std::rand() % 100 + 1;
        int guess;
        const int MAX_ATTEMPTS = 6;
        int low = 1;
        int high = 100;
        int attemptsUsed = 0;

        std::cout << "\n***************************************" << std::endl;
        std::cout << "\n  Select a number between 1 and 100." << std::endl;
        std::cout << "\n***************************************" << std::endl;
        std::cout << "You have all chance : " << MAX_ATTEMPTS << std::endl;

        for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
            std::cout << "\n--- Guess Attempt " << attempt << " ---" << std::endl;
            std::cout << "Enter your number: ";
            std::cin >> guess;
            attemptsUsed = attempt;

            if (guess == randomNumber) {
                std::cout << "\n**********************************" << std::endl;
                std::cout << "*  " << Utils::Color::GREEN << "CONGRATULATIONS! YOU WON!" << Utils::Color::RESET << "  *" << std::endl;
                std::cout << "*  The correct number is : " << randomNumber << " *" << std::endl;
                std::cout << "**********************************" << std::endl;
                break;
            } else if (guess < randomNumber) {
                std::cout << Utils::Color::YELLOW << "too low!" << Utils::Color::RESET << std::endl;
                low = guess + 1;
            } else {
                std::cout << Utils::Color::YELLOW << "too high! " << Utils::Color::RESET << std::endl;
                high = guess - 1;
            }

            std::cout << "The number is between " << low << " and " << high << ".\n";
            std::cout << "----------------------------------------\n";

            if (attempt == MAX_ATTEMPTS) {
                std::cout << "\n**********************************" << std::endl;
                std::cout << "*  " << Utils::Color::RED << "GAME OVER! YOU LOST!" << Utils::Color::RESET << "  *" << std::endl;
                std::cout << "*  The correct number is : " << randomNumber << " *" << std::endl;
                std::cout << "**********************************" << std::endl;
            }
        }

        int score = (MAX_ATTEMPTS - attemptsUsed) * 10;
        saveHighScore(playerName, score);
        displayHighScores();

        std::cout << "\nDo you want to play again? (y/n): ";
        std::cin >> playAgain;
        Utils::clearScreen();

    } while (playAgain == 'y' || playAgain == 'Y');
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
        std::cout << "  _   _   _   _  ___  ___ ______   _____  ______   _____   _   _   _____   _____   _____ \n";
        std::cout << " | \\ | | | | | | |  \\/  | | ___ \\ |  ___| | ___ \\ |  __ \\ | | | | |  ___| /  ___| /  ___| \n";
        std::cout << " |  \\| | | | | | | .  . | | |_/ / | |__   | |_/ / | |  \\/ | | | | | |__   \\ `--.  \\ `--.  \n";
        std::cout << " | . ` | | | | | | |\\/| | | ___ \\ |  __|  |    /  | | __  | | | | |  __|   `--. \\  `--. \\ \n";
        std::cout << " | |\\  | | |_| | | |  | | | |_/ / | |___  | |\\ \\  | |_\\ \\ | |_| | | |___  /\\__/ / /\\__/ / \n";
        std::cout << " \\_| \\_/  \\___/  \\_|  |_/ \\____/  \\____/  \\_| \\_|  \\____/  \\___/  \\____/  \\____/  \\____/  \n";
        std::cout << "======================================================================================================" << std::endl;

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
                std::cout << "\nPress Enter key to return the Menu..." << std::endl;
                _getch();
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

} // namespace NumberGuess
} // namespace Games
