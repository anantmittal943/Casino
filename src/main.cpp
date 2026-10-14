#include <iostream>
#include <string>
#include <random>

using namespace std;

class Player {
public:
    string name;
    int balance;

    Player(string name, int balance) : name(name), balance(balance) {}
};

int generateRandomNumber(int difficulty) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disturb(1, difficulty);
    return disturb(gen);
}

void playRound(Player &player) {
    int betAmount, guess, maxNumber = 10;

    cout << "Current Balance: $" << player.balance << "\n";
    cout << "Enter bet amount: $";
    cin >> betAmount;

    if (betAmount > player.balance) {
        cout << "Not enough balance !!";
        return;
    }

    cout << "Guess a number between 1 and " << maxNumber << ": ";
    cin >> guess;

    int winningNumber = generateRandomNumber(maxNumber);

    if (guess == winningNumber) {
        int winnings = betAmount * 2;
        player.balance += winnings;
        cout << "Jackpot! The number was " << winningNumber << ". You won $" << winnings << "!\n";
    } else {
        player.balance -= betAmount;
        cout << "Ouch. The winning number was " << winningNumber << ". You lost $" << betAmount << ".\n";
    }
}

int main() {
    cout << "====--- Welcome to the Terminal Casino ---====" << endl;

    string playerName;
    cout << "Enter your name: ";
    getline(cin, playerName);

    Player p1(playerName, 1000);

    char keepPlaying = 'y';
    while (keepPlaying == 'y' || keepPlaying == 'Y') {
        if (p1.balance <= 0) {
            cout << "You're broke !!\nGAME OVER";
            break;
        }

        playRound(p1);

        cout << "\nPlay again? (y/n): ";
        cin >> keepPlaying;
    }

    cout << "You're leaving with $" << p1.balance << ". Thanks for playing !!" << endl;
    return 0;
}
