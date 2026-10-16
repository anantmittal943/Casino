#ifndef CASINO_STATISTICS_H
#define CASINO_STATISTICS_H

#include <cstddef>
#include <vector>

#include "RoundLogic.h"

namespace casino {
    class Statistics {
    public:
        void recordRound(const RoundResult &result);

        void displayStatistics() const;

        void displayHistory() const;

        std::size_t getTotalRounds() const;

        std::size_t getWins() const;

        std::size_t getLosses() const;

        long long getTotalWagered() const;

        long long getTotalPayout() const;

        long long getNetProfit() const;

        long long getLargestWin() const;

        std::size_t getCurrentWinStreak() const;

        std::size_t getLongestWinStreak() const;

        bool totalsOverflowed() const;

        const std::vector<RoundResult> &getHistory() const;

    private:
        std::size_t totalRounds = 0;
        std::size_t wins = 0;
        std::size_t losses = 0;

        long long totalWagered = 0;
        long long totalPayout = 0;
        long long netProfit = 0;
        long long largestWin = 0;

        std::size_t currentWinStreak = 0;
        std::size_t longestWinStreak = 0;

        bool totalsHaveOverflowed = false;
        std::vector<RoundResult> history;
    };
} // namespace casino

#endif // CASINO_STATISTICS_H
