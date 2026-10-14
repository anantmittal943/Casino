#include <iostream>
#include <string>
#include <random>
#include <limits>
#include <sstream>

using namespace std;

class Player {
public:
    string name;
    long long balance;

    Player(string name, int balance) : name(name), balance(balance) {
    }
};

int generateRandomNumber(int difficulty) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disturb(1, difficulty);
    return disturb(gen);
}

bool readInteger(const string &prompt, long long &value) {
    string line;

    while (true) {
        cout << prompt;

        if (!getline(cin, line)) {
            return false;
        }

        stringstream ss(line);
        char extra;

        if ((ss >> value) && !(ss >> extra)) {
            return true;
        }

        cout << "Invalid input. Please try again: ";
    }
}

void playRound(Player &player) {
    long long betAmount, guess;
    const int maxNumber = 10;

    cout << "Current Balance: $" << player.balance << endl;

    while (true) {
        if (readInteger("Enter bet amount: $", betAmount)) {
            return;
        }

        if (betAmount <= 0) {
            cout << "Bet must be greater than $0!" << endl;
        } else if (betAmount > player.balance) {
            cout << "Not enough balance !!\nTry smaller bets." << endl;
        } else if (betAmount > numeric_limits<long long>::max()) {
            cout << "Bet amount is too large !!" << endl;
        } else {
            break;
        }
    }

    while (true) {
        if (readInteger("Guess a number between 1 and " + to_string(maxNumber) + ": ", guess)) {
            return;
        }

        if (guess < 1 || guess > maxNumber) {
            cout << "Your guess must be between 1 and " << maxNumber << endl;
        } else {
            break;
        }
    }

    int winningNumber = generateRandomNumber(maxNumber);

    player.balance -= betAmount; //early deduction

    if (guess == winningNumber) {
        long long payout = betAmount * 10;

        if (payout > numeric_limits<long long>::max() - player.balance) {
            cout << "Payout exceeds the supported balance limit." << endl;
            player.balance += betAmount; //reimbursement
            return;
        }

        player.balance += payout;
        cout << "\nJackpot! The number was " << winningNumber << ".\n";
        cout << "Total payout: $" << payout << "\n";
        cout << "Net profit: $" << payout - betAmount << "\n";
    } else {
        cout << "\nOuch. The winning number was " << winningNumber << ".\nYou lost $" << betAmount << ".\n";
    }

    cout << "Remaining balance: $" << player.balance << "\n";
}

int main() {
    cout << "====--- Welcome to the Terminal Casino ---====" << endl;

    string playerName;
    cout << "Enter your name: ";

    if (!getline(cin, playerName)) {
        return 0;
    }

    if (playerName.empty()) {
        playerName = "Player";
    }

    Player p1(playerName, 10000);

    while (true) {
        if (p1.balance <= 0) {
            cout << "\nYou're broke!\nGAME OVER\n";
            break;
        }

        playRound(p1);

        if (!cin) {
            break;
        }

        string choice;
        cout << "\nPlay again? (y/n): ";

        if (!getline(cin, choice)) {
            break;
        }

        if (choice == "n" || choice == "N") {
            break;
        }

        if (choice != "y" && choice != "Y") {
            cout << "Invalid choice. Exiting game.\n";
            break;
        }
    }

    cout << "\nYou're leaving with $" << p1.balance << ". Thanks for playing!\n";

    return 0;
}
