//
// Created by anant on 15-10-2026.
//

#include "../include/Player.h"

#include <string>
#include <limits>

namespace player {
    Player::Player(const std::string &name, int balance) : name(name), balance(balance) {
    }

    const std::string &Player::getName() const {
        return name;
    }

    long long Player::getBalance() const {
        return balance;
    }

    bool Player::canAfford(long long amount) const {
        return amount > 0 && amount <= balance;
    }

    bool Player::deductBet(long long amount) {
        if (!canAfford(amount)) {
            return false;
        }

        balance -= amount;
        return true;
    }

    bool Player::addMoney(long long amount) {
        if (amount < 0 ||
            amount > std::numeric_limits<long long>::max() - balance) {
            return false;
        }

        balance += amount;
        return true;
    }
} // player
