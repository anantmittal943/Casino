#include "Player.h"

#include <limits>
#include <stdexcept>

namespace player {
    Player::Player(const std::string &name, long long initialBalance)
        : name(name), balance(initialBalance) {
        if (initialBalance < 0) {
            throw std::invalid_argument("Initial balance cannot be negative.");
        }
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
