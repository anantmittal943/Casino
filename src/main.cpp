#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "GameSettings.h"
#include "Player.h"
#include "RoundLogic.h"
#include "Statistics.h"

namespace {
    int generateRandomNumber(int upperBound) {
        static std::random_device randomDevice;
        static std::mt19937 engine(randomDevice());
        std::uniform_int_distribution<int> distribution(1, upperBound);
        return distribution(engine);
    }

    bool readInteger(const std::string &prompt, long long &value) {
        while (true) {
            std::cout << prompt;

            std::string line;
            if (!std::getline(std::cin, line)) {
                return false;
            }

            std::stringstream stream(line);
            char extraCharacter = '\0';
            if ((stream >> value) && !(stream >> extraCharacter)) {
                return true;
            }

            std::cout << "Invalid input. Please enter a whole number.\n";
        }
    }

    bool configureSettings(casino::GameSettings &settings) {
        std::cout << "\n========== GAME SETTINGS ==========\n"
                << "1. Easy   (1-5)\n"
                << "2. Medium (1-10)\n"
                << "3. Hard   (1-20)\n"
                << "4. Custom range\n";

        long long choice = 0;
        while (true) {
            if (!readInteger("Select difficulty: ", choice)) {
                return false;
            }
            if (choice >= 1 && choice <= 4) {
                break;
            }
            std::cout << "Choose an option between 1 and 4.\n";
        }

        casino::Difficulty difficulty = casino::Difficulty::Medium;
        long long customRange = 0;
        if (choice == 1) {
            difficulty = casino::Difficulty::Easy;
        } else if (choice == 2) {
            difficulty = casino::Difficulty::Medium;
        } else if (choice == 3) {
            difficulty = casino::Difficulty::Hard;
        } else {
            difficulty = casino::Difficulty::Custom;
            while (true) {
                if (!readInteger(
                    "Enter maximum number (2-1000): ",
                    customRange)) {
                    return false;
                }
                if (casino::isValidCustomRange(customRange)) {
                    break;
                }
                std::cout << "Range must be between 2 and 1000.\n";
            }
        }

        if (!casino::setDifficulty(settings, difficulty, customRange)) {
            std::cout << "Invalid difficulty settings.\n";
            return false;
        }

        settings.minBet = 1;
        settings.maxBet = std::min(settings.startingBalance, 1'000'000LL);
        std::cout << "\nMinimum bet: $" << settings.minBet
                << "\nMaximum bet: $" << settings.maxBet
                << "\nDifficulty configured successfully!\n";
        return true;
    }

    bool playRound(
        player::Player &player,
        const casino::GameSettings &settings,
        casino::Statistics &stats) {
        long long betAmount = 0;
        long long guessInput = 0;

        std::cout << "\nCurrent balance: $" << player.getBalance() << '\n';

        while (true) {
            if (!readInteger("Enter bet amount: $", betAmount)) {
                return false;
            }

            if (betAmount < settings.minBet) {
                std::cout << "Minimum bet is $" << settings.minBet << ".\n";
            } else if (betAmount > settings.maxBet) {
                std::cout << "Maximum bet is $" << settings.maxBet << ".\n";
            } else if (!player.canAfford(betAmount)) {
                std::cout << "Not enough balance. Try a smaller bet.\n";
            } else {
                break;
            }
        }

        while (true) {
            if (!readInteger(
                "Guess a number between 1 and " +
                std::to_string(settings.maxNumber) + ": ",
                guessInput)) {
                return false;
            }

            if (guessInput < 1 || guessInput > settings.maxNumber) {
                std::cout << "Guess must be between 1 and "
                        << settings.maxNumber << ".\n";
            } else {
                break;
            }
        }

        const int winningNumber = generateRandomNumber(settings.maxNumber);
        const casino::RoundResult result = casino::resolveRound(
            player,
            betAmount,
            static_cast<int>(guessInput),
            winningNumber,
            settings.maxNumber);

        if (!result.valid) {
            std::cout << "Round error: " << result.error << '\n';
            return true;
        }

        stats.recordRound(result);

        if (result.won) {
            std::cout << "\nJackpot! The number was "
                    << result.winningNumber << ".\n"
                    << "Total payout: $" << result.payout << '\n'
                    << "Net profit: $" << result.netChange << '\n';
        } else {
            std::cout << "\nOuch. The winning number was "
                    << result.winningNumber << ".\n"
                    << "You lost $" << result.betAmount << ".\n";
        }

        std::cout << "Remaining balance: $" << player.getBalance() << '\n';
        return true;
    }
} // namespace

int main() {
    std::cout << "====--- Welcome to the Terminal Casino ---====\n"
            << "Enter your name: ";

    std::string playerName;
    if (!std::getline(std::cin, playerName)) {
        return 0;
    }
    if (playerName.empty()) {
        playerName = "Player";
    }

    casino::GameSettings settings;
    if (!configureSettings(settings)) {
        std::cout << "\nUnable to configure game. Exiting.\n";
        return 0;
    }

    player::Player currentPlayer(playerName, settings.startingBalance);
    casino::Statistics stats;

    std::cout << "\nWelcome, " << currentPlayer.getName() << "!\n"
            << "Starting balance: $" << currentPlayer.getBalance() << '\n'
            << "Number range: 1-" << settings.maxNumber << '\n';

    while (true) {
        std::cout << "\n========== CASINO MENU ==========\n"
                << "1. Play a round\n"
                << "2. View statistics\n"
                << "3. View round history\n"
                << "4. Quit\n";

        long long choice = 0;
        if (!readInteger("Choose an option: ", choice)) {
            break;
        }

        if (choice == 1) {
            if (currentPlayer.getBalance() <= 0) {
                std::cout << "You're out of money! GAME OVER.\n";
                break;
            }
            if (!playRound(currentPlayer, settings, stats)) {
                std::cout << "\nInput ended. Exiting game.\n";
                break;
            }
        } else if (choice == 2) {
            stats.displayStatistics();
        } else if (choice == 3) {
            stats.displayHistory();
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Invalid option. Choose 1-4.\n";
        }
    }

    std::cout << "\n========== SESSION SUMMARY ==========\n";
    stats.displayStatistics();
    std::cout << "\nFinal balance: $" << currentPlayer.getBalance() << '\n'
            << "Thanks for playing, " << currentPlayer.getName() << "!\n";
    return 0;
}
