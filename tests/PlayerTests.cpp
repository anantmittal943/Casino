#include "Player.h"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

namespace {
    using player::Player;

    TEST(PlayerTests, StoresNameAndInitialBalance) {
        const Player player("Anant", 5'000);

        EXPECT_EQ(player.getName(), "Anant");
        EXPECT_EQ(player.getBalance(), 5'000);
    }

    TEST(PlayerTests, AllowsOnlyAffordablePositiveBets) {
        const Player player("Tester", 1'000);

        EXPECT_TRUE(player.canAfford(1));
        EXPECT_TRUE(player.canAfford(1'000));
        EXPECT_FALSE(player.canAfford(0));
        EXPECT_FALSE(player.canAfford(-100));
        EXPECT_FALSE(player.canAfford(1'001));
    }

    TEST(PlayerTests, DeductsValidBet) {
        Player player("Tester", 1'000);

        EXPECT_TRUE(player.deductBet(200));
        EXPECT_EQ(player.getBalance(), 800);
    }

    TEST(PlayerTests, InvalidDeductionsPreserveBalance) {
        Player player("Tester", 1'000);

        EXPECT_FALSE(player.deductBet(0));
        EXPECT_FALSE(player.deductBet(-100));
        EXPECT_FALSE(player.deductBet(1'001));
        EXPECT_EQ(player.getBalance(), 1'000);
    }

    TEST(PlayerTests, AddsMoney) {
        Player player("Tester", 1'000);

        EXPECT_TRUE(player.addMoney(500));
        EXPECT_EQ(player.getBalance(), 1'500);
    }

    TEST(PlayerTests, RejectsNegativeMoneyWithoutChangingBalance) {
        Player player("Tester", 1'000);

        EXPECT_FALSE(player.addMoney(-100));
        EXPECT_EQ(player.getBalance(), 1'000);
    }

    TEST(PlayerTests, RejectsBalanceOverflowWithoutChangingBalance) {
        constexpr long long maximum =
                std::numeric_limits<long long>::max();
        Player player("Tester", maximum);

        EXPECT_FALSE(player.addMoney(1));
        EXPECT_EQ(player.getBalance(), maximum);
    }

    TEST(PlayerTests, RejectsNegativeInitialBalance) {
        EXPECT_THROW(Player("Invalid", -1), std::invalid_argument);
    }
} // namespace