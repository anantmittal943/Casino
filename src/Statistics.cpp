#include "Statistics.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

namespace casino {
    namespace {
        long long addNonNegativeSaturated(
            long long total,
            long long amount,
            bool &overflowed) {
            if (amount > std::numeric_limits<long long>::max() - total) {
                overflowed = true;
                return std::numeric_limits<long long>::max();
            }
            return total + amount;
        }

        long long addSignedSaturated(
            long long total,
            long long amount,
            bool &overflowed) {
            constexpr long long maximum =
                    std::numeric_limits<long long>::max();
            constexpr long long minimum =
                    std::numeric_limits<long long>::min();

            if (amount > 0 && total > maximum - amount) {
                overflowed = true;
                return maximum;
            }
            if (amount < 0 && total < minimum - amount) {
                overflowed = true;
                return minimum;
            }
            return total + amount;
        }

        void incrementSaturated(std::size_t &value, bool &overflowed) {
            if (value == std::numeric_limits<std::size_t>::max()) {
                overflowed = true;
                return;
            }
            ++value;
        }
    } // namespace

    void Statistics::recordRound(const RoundResult &result) {
        if (!result.valid) {
            return;
        }

        history.push_back(result);
        incrementSaturated(totalRounds, totalsHaveOverflowed);
        totalWagered = addNonNegativeSaturated(
            totalWagered, result.betAmount, totalsHaveOverflowed);
        totalPayout = addNonNegativeSaturated(
            totalPayout, result.payout, totalsHaveOverflowed);
        netProfit = addSignedSaturated(
            netProfit, result.netChange, totalsHaveOverflowed);

        if (result.won) {
            incrementSaturated(wins, totalsHaveOverflowed);
            incrementSaturated(currentWinStreak, totalsHaveOverflowed);
            longestWinStreak =
                    std::max(longestWinStreak, currentWinStreak);
            largestWin = std::max(largestWin, result.netChange);
        } else {
            incrementSaturated(losses, totalsHaveOverflowed);
            currentWinStreak = 0;
        }
    }

    void Statistics::displayStatistics() const {
        std::cout << "\n========== SESSION STATISTICS ==========\n"
                << "Total rounds: " << totalRounds << '\n'
                << "Wins: " << wins << '\n'
                << "Losses: " << losses << '\n';

        const std::ios::fmtflags originalFlags = std::cout.flags();
        const std::streamsize originalPrecision = std::cout.precision();
        std::cout << std::fixed << std::setprecision(2)
                << "Win rate: " << getWinRate() << "%\n";
        std::cout.flags(originalFlags);
        std::cout.precision(originalPrecision);

        std::cout << "Total wagered: $" << totalWagered << '\n'
                << "Total payouts: $" << totalPayout << '\n'
                << "Net profit/loss: $" << netProfit << '\n'
                << "Largest single-round win: $" << largestWin << '\n'
                << "Current win streak: " << currentWinStreak << '\n'
                << "Longest win streak: " << longestWinStreak << '\n';

        if (totalsHaveOverflowed) {
            std::cout << "Warning: one or more statistics reached their "
                    "numeric limit and are saturated.\n";
        }
    }

    void Statistics::displayHistory() const {
        std::cout << "\n================ ROUND HISTORY ================\n";

        if (history.empty()) {
            std::cout << "No completed rounds yet.\n";
            return;
        }

        const std::ios::fmtflags originalFlags = std::cout.flags();
        std::cout << std::left
                << std::setw(7) << "Round"
                << std::setw(8) << "Guess"
                << std::setw(8) << "Number"
                << std::setw(12) << "Bet"
                << std::setw(12) << "Payout"
                << std::setw(12) << "Result"
                << "Net Change\n"
                << std::string(75, '-') << '\n';

        for (std::size_t i = 0; i < history.size(); ++i) {
            const RoundResult &round = history[i];
            std::cout << std::left
                    << std::setw(7) << i + 1
                    << std::setw(8) << round.guessedNumber
                    << std::setw(8) << round.winningNumber
                    << std::setw(12) << round.betAmount
                    << std::setw(12) << round.payout
                    << std::setw(12) << (round.won ? "WIN" : "LOSS")
                    << (round.netChange >= 0 ? "+" : "")
                    << round.netChange << '\n';
        }
        std::cout.flags(originalFlags);
    }

    std::size_t Statistics::getTotalRounds() const {
        return totalRounds;
    }

    std::size_t Statistics::getWins() const {
        return wins;
    }

    std::size_t Statistics::getLosses() const {
        return losses;
    }

    long long Statistics::getTotalWagered() const {
        return totalWagered;
    }

    long long Statistics::getTotalPayout() const {
        return totalPayout;
    }

    long long Statistics::getNetProfit() const {
        return netProfit;
    }

    long long Statistics::getLargestWin() const {
        return largestWin;
    }

    double Statistics::getWinRate() const {
        if (totalRounds == 0) {
            return 0.0;
        }
        return 100.0 * static_cast<double>(wins) /
               static_cast<double>(totalRounds);
    }

    std::size_t Statistics::getCurrentWinStreak() const {
        return currentWinStreak;
    }

    std::size_t Statistics::getLongestWinStreak() const {
        return longestWinStreak;
    }

    bool Statistics::totalsOverflowed() const {
        return totalsHaveOverflowed;
    }

    const std::vector<RoundResult> &Statistics::getHistory() const {
        return history;
    }
} // namespace casino
