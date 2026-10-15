#ifndef CASINO_PLAYER_H
#define CASINO_PLAYER_H

#include <string>

namespace player {
    class Player {
    private:
        std::string name;
        long long balance;

    public:
        Player(const std::string &name, long long initialBalance);

        const std::string &getName() const;

        long long getBalance() const;

        bool canAfford(long long amount) const;

        bool deductBet(long long amount);

        bool addMoney(long long amount);
    };
} // player

#endif //CASINO_PLAYER_H
