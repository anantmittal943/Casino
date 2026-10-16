#include "Player.h"
#include "RoundLogic.h"
#include "Statistics.h"

#include <gtest/gtest.h>

#include <limits>

namespace {
    using casino::RoundResult;
    using casino::Statistics;
    using player::Player;

    TEST(StatisticsTests, StartsWithZeroValues) {
        const Statistics stats;

        EXPECT_EQ(stats.getTotalRounds(), 0U);
        EXPECT_EQ(stats.getWins(), 0U);
        EXPECT_EQ(stats.getLosses(), 0U);
        EXPECT_EQ(stats.getTotalWagered(), 0);
        EXPECT_EQ(stats.getTotalPayout(), 0);
        EXPECT_EQ(stats.getNetProfit(), 0);
        EXPECT_EQ(stats.getLargestWin(), 0);
        EXPECT_DOUBLE_EQ(stats.getWinRate(), 0.0);
        EXPECT_EQ(stats.getCurrentWinStreak(), 0U);
        EXPECT_EQ(stats.getLongestWinStreak(), 0U);
        EXPECT_TRUE(stats.getHistory().empty());
    }

    TEST(StatisticsTests, RecordsWinLossTotalsHistoryAndWinRate) {
        Player player("Tester", 1'000);
        Statistics stats;

        const RoundResult firstWin =
                casino::resolveRound(player, 10, 3, 3, 5);
        const RoundResult secondWin =
                casino::resolveRound(player, 10, 4, 4, 5);
        const RoundResult loss =
                casino::resolveRound(player, 10, 1, 2, 5);

        ASSERT_TRUE(firstWin.valid);
        ASSERT_TRUE(secondWin.valid);
        ASSERT_TRUE(loss.valid);
        stats.recordRound(firstWin);
        stats.recordRound(secondWin);
        stats.recordRound(loss);

        EXPECT_EQ(stats.getTotalRounds(), 3U);
        EXPECT_EQ(stats.getWins(), 2U);
        EXPECT_EQ(stats.getLosses(), 1U);
        EXPECT_EQ(stats.getTotalWagered(), 30);
        EXPECT_EQ(stats.getTotalPayout(), 100);
        EXPECT_EQ(stats.getNetProfit(), 70);
        EXPECT_EQ(stats.getLargestWin(), 40);
        EXPECT_NEAR(stats.getWinRate(), 200.0 / 3.0, 1e-12);
        EXPECT_EQ(stats.getCurrentWinStreak(), 0U);
        EXPECT_EQ(stats.getLongestWinStreak(), 2U);

        ASSERT_EQ(stats.getHistory().size(), 3U);
        EXPECT_EQ(stats.getHistory().at(0).guessedNumber, 3);
        EXPECT_EQ(stats.getHistory().at(0).winningNumber, 3);
        EXPECT_EQ(stats.getHistory().at(0).betAmount, 10);
        EXPECT_EQ(stats.getHistory().at(0).payout, 50);
        EXPECT_EQ(stats.getHistory().at(0).netChange, 40);
        EXPECT_FALSE(stats.getHistory().at(2).won);
    }

    TEST(StatisticsTests, TracksStreakAfterLossResetsCurrentStreak) {
        Player player("Tester", 1'000);
        Statistics stats;

        const RoundResult firstWin =
                casino::resolveRound(player, 10, 1, 1, 10);
        const RoundResult secondWin =
                casino::resolveRound(player, 10, 2, 2, 10);
        const RoundResult loss =
                casino::resolveRound(player, 10, 3, 4, 10);
        const RoundResult finalWin =
                casino::resolveRound(player, 10, 5, 5, 10);

        ASSERT_TRUE(firstWin.valid);
        ASSERT_TRUE(secondWin.valid);
        ASSERT_TRUE(loss.valid);
        ASSERT_TRUE(finalWin.valid);
        stats.recordRound(firstWin);
        stats.recordRound(secondWin);
        stats.recordRound(loss);
        stats.recordRound(finalWin);

        EXPECT_EQ(stats.getCurrentWinStreak(), 1U);
        EXPECT_EQ(stats.getLongestWinStreak(), 2U);
    }

    TEST(StatisticsTests, IgnoresInvalidRoundResults) {
        Statistics stats;
        RoundResult invalid;
        invalid.valid = false;

        stats.recordRound(invalid);

        EXPECT_EQ(stats.getTotalRounds(), 0U);
        EXPECT_TRUE(stats.getHistory().empty());
    }

    TEST(StatisticsTests, SaturatesCumulativeTotalsInsteadOfOverflowing) {
        Statistics stats;
        RoundResult largeRound;
        largeRound.valid = true;
        largeRound.betAmount = std::numeric_limits<long long>::max();
        largeRound.payout = std::numeric_limits<long long>::max();
        largeRound.netChange = std::numeric_limits<long long>::max();

        stats.recordRound(largeRound);
        stats.recordRound(largeRound);

        EXPECT_EQ(
            stats.getTotalWagered(),
            std::numeric_limits<long long>::max());
        EXPECT_EQ(
            stats.getTotalPayout(),
            std::numeric_limits<long long>::max());
        EXPECT_EQ(
            stats.getNetProfit(),
            std::numeric_limits<long long>::max());
        EXPECT_TRUE(stats.totalsOverflowed());
    }

    TEST(StatisticsTests, SaturatesNetLossInsteadOfUnderflowing) {
        Statistics stats;
        RoundResult firstLoss;
        firstLoss.valid = true;
        firstLoss.betAmount = 1;
        firstLoss.netChange = std::numeric_limits<long long>::min();
        RoundResult secondLoss;
        secondLoss.valid = true;
        secondLoss.betAmount = 1;
        secondLoss.netChange = -1;

        stats.recordRound(firstLoss);
        stats.recordRound(secondLoss);

        EXPECT_EQ(
            stats.getNetProfit(),
            std::numeric_limits<long long>::min());
        EXPECT_TRUE(stats.totalsOverflowed());
    }
} // namespace