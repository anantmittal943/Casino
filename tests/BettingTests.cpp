#include "GameSettings.h"
#include "Player.h"
#include "RoundLogic.h"

#include <gtest/gtest.h>

#include <array>
#include <limits>

namespace {
    using casino::Difficulty;
    using casino::GameSettings;
    using casino::RoundResult;
    using player::Player;

    TEST(BettingTests, WinningRoundPaysTenTimesAtDefaultRange) {
        Player player("Tester", 1'000);
        const RoundResult result =
                casino::resolveRound(player, 100, 7, 7);

        ASSERT_TRUE(result.valid);
        EXPECT_TRUE(result.won);
        EXPECT_EQ(result.betAmount, 100);
        EXPECT_EQ(result.payout, 1'000);
        EXPECT_EQ(result.netChange, 900);
        EXPECT_EQ(player.getBalance(), 1'900);
    }

    TEST(BettingTests, LosingRoundDeductsOnlyTheBet) {
        Player player("Tester", 1'000);
        const RoundResult result =
                casino::resolveRound(player, 100, 7, 4, 10);

        ASSERT_TRUE(result.valid);
        EXPECT_FALSE(result.won);
        EXPECT_EQ(result.payout, 0);
        EXPECT_EQ(result.netChange, -100);
        EXPECT_EQ(player.getBalance(), 900);
    }

    TEST(BettingTests, RejectsInvalidBetsWithoutChangingBalance) {
        Player player("Tester", 1'000);

        const RoundResult zeroBet =
                casino::resolveRound(player, 0, 7, 7, 10);
        const RoundResult negativeBet =
                casino::resolveRound(player, -100, 7, 7, 10);
        const RoundResult unaffordableBet =
                casino::resolveRound(player, 1'001, 7, 7, 10);

        EXPECT_FALSE(zeroBet.valid);
        EXPECT_FALSE(negativeBet.valid);
        EXPECT_FALSE(unaffordableBet.valid);
        EXPECT_EQ(player.getBalance(), 1'000);
    }

    TEST(BettingTests, RejectsNumbersOutsideTheConfiguredRange) {
        Player player("Tester", 1'000);

        const RoundResult invalidGuess =
                casino::resolveRound(player, 100, 11, 7, 10);
        const RoundResult invalidWinningNumber =
                casino::resolveRound(player, 100, 7, 11, 10);
        const RoundResult invalidRange =
                casino::resolveRound(player, 100, 1, 1, 0);

        EXPECT_FALSE(invalidGuess.valid);
        EXPECT_FALSE(invalidWinningNumber.valid);
        EXPECT_FALSE(invalidRange.valid);
        EXPECT_EQ(player.getBalance(), 1'000);
    }

    TEST(BettingTests, DifficultyRangesProduceMatchingGrossPayouts) {
        struct DifficultyCase {
            Difficulty difficulty;
            int maxNumber;
        };
        constexpr std::array cases{
            DifficultyCase{Difficulty::Easy, 5},
            DifficultyCase{Difficulty::Medium, 10},
            DifficultyCase{Difficulty::Hard, 20},
            DifficultyCase{Difficulty::Custom, 50}
        };

        for (const DifficultyCase &testCase: cases) {
            SCOPED_TRACE(testCase.maxNumber);
            GameSettings settings;
            const long long customRange =
                    testCase.difficulty == Difficulty::Custom
                        ? testCase.maxNumber
                        : 0;
            ASSERT_TRUE(casino::setDifficulty(
                settings, testCase.difficulty, customRange));
            ASSERT_EQ(settings.maxNumber, testCase.maxNumber);

            Player player("Tester", 5'000);
            constexpr long long bet = 100;
            const RoundResult result = casino::resolveRound(
                player,
                bet,
                testCase.maxNumber,
                testCase.maxNumber,
                settings.maxNumber);

            ASSERT_TRUE(result.valid);
            EXPECT_TRUE(result.won);
            EXPECT_EQ(result.payout, bet * testCase.maxNumber);
            EXPECT_EQ(result.netChange, bet * (testCase.maxNumber - 1));
            EXPECT_EQ(
                player.getBalance(),
                5'000 + bet * (testCase.maxNumber - 1));
        }
    }

    TEST(BettingTests, RejectsInvalidCustomRangeWithoutChangingSettings) {
        GameSettings settings;
        ASSERT_TRUE(casino::setDifficulty(settings, Difficulty::Hard));
        ASSERT_EQ(settings.maxNumber, 20);

        EXPECT_FALSE(casino::isValidCustomRange(1));
        EXPECT_FALSE(casino::isValidCustomRange(1'001));
        EXPECT_FALSE(casino::setDifficulty(
            settings, Difficulty::Custom, 1'001));
        EXPECT_EQ(settings.maxNumber, 20);

        EXPECT_TRUE(casino::isValidCustomRange(2));
        EXPECT_TRUE(casino::isValidCustomRange(1'000));
        EXPECT_TRUE(casino::setDifficulty(settings, Difficulty::Custom, 2));
        EXPECT_EQ(settings.maxNumber, 2);
    }

    TEST(BettingTests, RejectsPayoutOverflowWithoutChangingBalance) {
        constexpr long long maximum =
                std::numeric_limits<long long>::max();
        Player player("Tester", maximum);
        const long long bet = maximum / 10 + 1;

        const RoundResult result =
                casino::resolveRound(player, bet, 1, 1, 10);

        EXPECT_FALSE(result.valid);
        EXPECT_EQ(player.getBalance(), maximum);
    }

    TEST(BettingTests, RejectsBalanceOverflowWithoutChangingBalance) {
        constexpr long long maximum =
                std::numeric_limits<long long>::max();
        Player player("Tester", maximum);

        const RoundResult result =
                casino::resolveRound(player, 1, 1, 1, 10);

        EXPECT_FALSE(result.valid);
        EXPECT_EQ(player.getBalance(), maximum);
    }
} // namespace