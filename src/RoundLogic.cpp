#include "RoundLogic.h"

#include <limits>

namespace casino {
    RoundResult resolveRound(
        player::Player &player,
        long long betAmount,
        int guess,
        int winningNumber,
        int maxNumber) {
        RoundResult result;
        result.guessedNumber = guess;
        result.winningNumber = winningNumber;
        result.betAmount = betAmount;

        if (betAmount <= 0) {
            result.error = "Bet must be greater than zero.";
            return result;
        }

        if (!player.canAfford(betAmount)) {
            result.error = "Insufficient balance.";
            return result;
        }

        if (maxNumber < 1 || guess < 1 || guess > maxNumber ||
            winningNumber < 1 || winningNumber > maxNumber) {
            result.error = "Guess or winning number is out of range.";
            return result;
        }

        if (guess == winningNumber) {
            const long long maximumBalance =
                    std::numeric_limits<long long>::max();

            if (betAmount > maximumBalance / maxNumber) {
                result.error = "Payout is too large.";
                return result;
            }

            result.payout = betAmount * maxNumber;
            const long long balanceAfterBet =
                    player.getBalance() - betAmount;

            if (result.payout > maximumBalance - balanceAfterBet) {
                result.error = "Balance limit exceeded.";
                return result;
            }
        }

        if (!player.deductBet(betAmount)) {
            result.error = "Could not deduct bet.";
            return result;
        }

        if (guess == winningNumber) {
            if (!player.addMoney(result.payout)) {
                // The balance was checked before the bet was deducted.
                player.addMoney(betAmount);
                result.error = "Could not credit payout.";
                return result;
            }

            result.won = true;
            result.netChange = result.payout - betAmount;
        } else {
            result.netChange = -betAmount;
        }

        result.valid = true;
        return result;
    }
} // namespace casino
