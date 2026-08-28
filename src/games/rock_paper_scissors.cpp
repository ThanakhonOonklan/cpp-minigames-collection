#include "games/rock_paper_scissors.h"
#include "utils/console_utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <conio.h>

namespace Games {
namespace RockPaperScissors {

namespace {

const std::string SCORE_FILE = "data/rps.txt";

struct PlayerScore {
    std::string name;
    int score;
};

void displayChoice(int choice) {
    switch (choice) {
        case 1: // Rock
            std::cout << "    _______ \n"
                      << "---'   ____) \n"
                      << "      (_____)\n"
                      << "      (_____)\n"
                      << "      (____) \n"
                      << "---.__(___)\n";
            break;
        case 2: // Paper
            std::cout << "    _______\n"
                      << "---'   ____)____\n"
                      << "          ______)\n"
                      << "          _______)\n"
                      << "         _______)\n"
                      << "---.__________)\n";
            break;
        case 3: // Scissors
            std::cout << "    _______\n"
                      << "---'   ____)____\n"
                      << "          ______)\n"
                      << "       __________)\n"
                      << "      (____)\n"
                      << "---.__(___)\n";
            break;
        default:
            std::cout << "Error\n";
    }
}

void saveHighScore(const std::string& playerName, int score) {
    std::ofstream outFile(SCORE_FILE, std::ios::app);
    if (outFile.is_open()) {
        outFile << playerName << " " << score << std::endl;
        outFile.close();
    } else {
        std::cerr << "Error opening file for writing!" << std::endl;
    }
}

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
        std::cerr << "Error opening file for reading!" << std::endl;
    }

    std::cout << "\nPress Enter key to return menu..." << std::endl;
    _getch();
}

void runRPSSession() {
    int playerChoice, computerChoice;
    int playerScore = 0, computerScore = 0;
    std::string playerName;

    std::srand(static_cast<unsigned int>(std::time(0)));

    std::cout << "Enter your name: ";
    std::cin >> playerName;

    char playAgain;
    do {
        Utils::clearScreen();

        std::cout << "\n=====================================" << std::endl;
        std::cout << "            Choose 1 item            " << std::endl;
        std::cout << "=====================================" << std::endl;
        std::cout << "1. Rock" << std::endl;
        std::cout << "2. Paper" << std::endl;
        std::cout << "3. Scissors" << std::endl;
        std::cout << "Enter number (1) (2) (3): ";

        while (!(std::cin >> playerChoice) || playerChoice < 1 || playerChoice > 3) {
            std::cout << "Please enter number (1) (2) (3): ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        computerChoice = std::rand() % 3 + 1;

        std::cout << "\nYou choose: " << std::endl;
        displayChoice(playerChoice);
        std::cout << "\nComputer choose: " << std::endl;
        displayChoice(computerChoice);

        if (playerChoice == computerChoice) {
            std::cout << Utils::Color::YELLOW << "It's a tie!" << Utils::Color::RESET << std::endl;
        } else if ((playerChoice == 1 && computerChoice == 3) || 
                   (playerChoice == 2 && computerChoice == 1) || 
                   (playerChoice == 3 && computerChoice == 2)) {
            std::cout << Utils::Color::GREEN << "You win!" << Utils::Color::RESET << std::endl;
            playerScore++;
        } else {
            std::cout << Utils::Color::RED << "Computer wins!" << Utils::Color::RESET << std::endl;
            computerScore++;
        }

        std::cout << "\n=====================================" << std::endl;
        std::cout << "Scores - You: " << playerScore << " | Computer: " << computerScore << std::endl;
        std::cout << "=====================================" << std::endl;

        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    saveHighScore(playerName, playerScore);
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
        std::cout << "  ____     ____     _____  \n";
        std::cout << " |  _ \\   |  _ \\   / ____| \n";
        std::cout << " | |_) |  | |_) | | (___   \n";
        std::cout << " |  _ <   |  __/   \\___ \\  \n";
        std::cout << " | | \\ \\  | |      ____) | \n";
        std::cout << " |_|  \\_\\ |_|     |_____/  \n";
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
                runRPSSession();
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

} // namespace RockPaperScissors
} // namespace Games
