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

int generateRandomNumber(int difficultyMax) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disturb(1, difficultyMax);
    return disturb(gen);
}

int main() {}