#include "games/hangman.h"
#include "utils/console_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <conio.h>

namespace Games {
namespace Hangman {

namespace {

const std::string SCORE_FILE = "data/hangman.txt";

struct PlayerScore {
    std::string name;
    int score;
};

struct WordHint {
    std::string word;
    std::string hint;
};

void displayWord(const std::string& word, const bool guessed[]) {
    std::cout << "\nWord: ";
    for (size_t i = 0; i < word.size(); i++) {
        if (guessed[i]) {
            std::cout << word[i] << " ";
        } else {
            std::cout << "_ ";
        }
    }
    std::cout << std::endl;
}

bool isWordGuessed(const bool guessed[], size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (!guessed[i]) {
            return false;
        }
    }
    return true;
}

void displayHangman(int wrongGuesses) {
    std::cout << "\n==== HANGMAN ====\n";
    switch (wrongGuesses) {
        case 0:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << "     |\n"
                      << "     |\n"
                      << "     |\n"
                      << "     |\n"
                      << "=========\n";
            break;
        case 1:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << " O   |\n"
                      << "     |\n"
                      << "     |\n"
                      << "     |\n"
                      << "=========\n";
            break;
        case 2:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << " O   |\n"
                      << " |   |\n"
                      << "     |\n"
                      << "     |\n"
                      << "=========\n";
            break;
        case 3:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << " O   |\n"
                      << "/|   |\n"
                      << "     |\n"
                      << "     |\n"
                      << "=========\n";
            break;
        case 4:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << " O   |\n"
                      << "/|\\  |\n"
                      << "     |\n"
                      << "     |\n"
                      << "=========\n";
            break;
        case 5:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << " O   |\n"
                      << "/|\\  |\n"
                      << "/    |\n"
                      << "     |\n"
                      << "=========\n";
            break;
        case 6:
            std::cout << " +---+\n"
                      << " |   |\n"
                      << " O   |\n"
                      << "/|\\  |\n"
                      << "/|\\  |\n"
                      << "     |\n"
                      << "=========\n";
            break;
    }
    std::cout << "=================\n";
}

void saveScore(const std::string& playerName, int score) {
    std::ofstream file(SCORE_FILE, std::ios::app);
    if (file.is_open()) {
        file << playerName << " " << score << std::endl;
        file.close();
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

    std::cout << "\nPress Enter key to return the Menu..." << std::endl;
    _getch();
}

int runHangmanGame() {
    Utils::clearScreen();

    const std::vector<WordHint> wordHints = {
        {"elephant", "A large animal with a trunk"},
        {"giraffe", "Tallest land animal"},
        {"computer", "An electronic device for storing and processing data"},
        {"umbrella", "Used for protection against rain"},
        {"guitar", "A musical instrument with strings"},
        {"bicycle", "A vehicle with two wheels"},
        {"piano", "A large musical instrument with keys"},
        {"kitchen", "A room for cooking"},
        {"mountain", "A large natural elevation of the earth's surface"},
        {"river", "A large natural stream of water"},
        {"ocean", "A vast body of saltwater"},
        {"vaccine", "A substance used to stimulate the production of antibodies"},
        {"whale", "The largest animal on Earth"},
        {"soccer", "A popular team sport played with a round ball"},
        {"pizza", "A dish of Italian origin consisting of a usually round base"},
        {"jacket", "An outer garment worn on the upper body"},
        {"basketball", "A sport played by two teams"},
        {"toaster", "A kitchen appliance for toasting bread"},
        {"television", "A device for viewing broadcast content"},
        {"vacation", "A period spent away from home or business in travel"},
        {"library", "A collection of books and other printed materials"}
    };

    std::string playerName;
    std::cout << "Enter your name: ";
    std::cin >> playerName;

    char playAgain;
    do {
        Utils::clearScreen();
        int wordIndex = std::rand() % wordHints.size();
        std::string word = wordHints[wordIndex].word;
        std::string hint = wordHints[wordIndex].hint;
        bool guessed[100] = {false};
        int wrongGuesses = 0;
        char guess;

        std::cout << "\n===================\n";
        std::cout << "Hint: " << hint << "\n";
        displayHangman(wrongGuesses);
        displayWord(word, guessed);

        while (wrongGuesses < 6 && !isWordGuessed(guessed, word.size())) {
            std::cout << "Enter a letter: ";
            std::cin >> guess;

            Utils::clearScreen();
            bool correctGuess = false;
            for (size_t i = 0; i < word.size(); i++) {
                if (word[i] == guess) {
                    guessed[i] = true;
                    correctGuess = true;
                }
            }

            if (!correctGuess) {
                wrongGuesses++;
            }

            std::cout << "\n===================\n";
            std::cout << "Hint: " << hint << "\n";
            displayHangman(wrongGuesses);
            displayWord(word, guessed);
        }

        if (isWordGuessed(guessed, word.size())) {
            int score = 6 - wrongGuesses;
            std::cout << "\n" << Utils::Color::GREEN << "You Win!!" << Utils::Color::RESET << std::endl;
            std::cout << "\n=====================================" << std::endl;
            std::cout << "Score : " << score << std::endl;
            std::cout << "=====================================" << std::endl;
            saveScore(playerName, score);
        } else {
            std::cout << "\n" << Utils::Color::RED << "Game over! The word was: " << word << Utils::Color::RESET << std::endl;
        }

        std::cout << "\nDo you want to play again? (y/n): ";
        std::cin >> playAgain;
        Utils::clearScreen();
    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
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
        std::cout << "  _   _     _      _   _   _____   __  __      _     _   _ \n";
        std::cout << " | | | |   / \\    | \\ | | / ____| |  \\/  |    / \\   | \\ | |\n";
        std::cout << " | |_| |  / _ \\   |  \\| || |  __  | |\\/| |   / _ \\  |  \\| |\n";
        std::cout << " |  _  | /_/ \\_\\  | |\\  || | |_ | | |  | |  /_/ \\_\\ | |\\  |\n";
        std::cout << " |_| |_| | | | |  |_| \\_| \\_____| |_|  |_|  | | | | |_| \\_|\n";
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
                runHangmanGame();
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

} // namespace Hangman
} // namespace Games
