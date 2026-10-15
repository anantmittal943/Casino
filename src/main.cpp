#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "Player.h"
#include "RoundLogic.h"

namespace {
    constexpr int maxNumber = 10;
    constexpr long long startingBalance = 1'000;

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

    bool playRound(player::Player &player) {
        long long betAmount = 0;
        long long guessInput = 0;

        std::cout << "\nCurrent balance: $" << player.getBalance() << '\n';

        while (true) {
            if (!readInteger("Enter bet amount: $", betAmount)) {
                return false;
            }

            if (betAmount <= 0) {
                std::cout << "Bet must be greater than zero.\n";
            } else if (!player.canAfford(betAmount)) {
                std::cout << "Not enough balance. Try a smaller bet.\n";
            } else {
                break;
            }
        }

        while (true) {
            if (!readInteger("Guess a number between 1 and 10: ", guessInput)) {
                return false;
            }

            if (guessInput < 1 || guessInput > maxNumber) {
                std::cout << "Your guess must be between 1 and "
                        << maxNumber << ".\n";
            } else {
                break;
            }
        }

        const int winningNumber = generateRandomNumber(maxNumber);
        const casino::RoundResult result = casino::resolveRound(
            player,
            betAmount,
            static_cast<int>(guessInput),
            winningNumber,
            maxNumber);

        if (!result.valid) {
            std::cout << "Round error: " << result.error << '\n';
            return true;
        }

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

    bool askToPlayAgain() {
        while (true) {
            std::string choice;
            std::cout << "\nPlay again? (y/n): ";
            if (!std::getline(std::cin, choice)) {
                return false;
            }

            if (choice == "y" || choice == "Y") {
                return true;
            }
            if (choice == "n" || choice == "N") {
                return false;
            }

            std::cout << "Please enter y or n.\n";
        }
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

    player::Player currentPlayer(playerName, startingBalance);

    while (currentPlayer.getBalance() > 0) {
        if (!playRound(currentPlayer)) {
            std::cout << "\nInput ended. Exiting game.\n";
            break;
        }
        if (currentPlayer.getBalance() <= 0) {
            std::cout << "\nYou're broke!\nGAME OVER\n";
            break;
        }
        if (!askToPlayAgain()) {
            break;
        }
    }

    std::cout << "\nYou're leaving with $" << currentPlayer.getBalance()
            << ". Thanks for playing!\n";
    return 0;
}
