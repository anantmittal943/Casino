#ifndef CASINO_ROUND_LOGIC_H
#define CASINO_ROUND_LOGIC_H

#include <string>

#include "Player.h"

namespace casino {
    struct RoundResult {
        bool valid = false;
        std::string error;

        int guessedNumber = 0;
        int winningNumber = 0;

        long long betAmount = 0;
        long long payout = 0;
        long long netChange = 0;

        bool won = false;
    };

    RoundResult resolveRound(
        player::Player &player,
        long long betAmount,
        int guess,
        int winningNumber,
        int maxNumber = 10);
} // namespace casino

#endif // CASINO_ROUND_LOGIC_H
