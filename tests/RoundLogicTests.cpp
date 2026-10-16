#include "Player.h"
#include "RoundLogic.h"
#include "Statistics.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace {
    int failures = 0;

    void expect(bool condition, const std::string &description) {
        if (!condition) {
            std::cerr << "FAIL: " << description << '\n';
            ++failures;
        }
    }

    void testWinningRound() {
        player::Player player("Winner", 1'000);
        const casino::RoundResult result =
                casino::resolveRound(player, 100, 7, 7);

        expect(result.valid, "matching guess is a valid round");
        expect(result.won, "matching guess wins");
        expect(result.payout == 1'000, "winning payout is ten times the bet");
        expect(result.netChange == 900, "winning net change excludes returned stake");
        expect(player.getBalance() == 1'900, "winning balance is updated");
    }

    void testRangeBasedPayout() {
        player::Player player("Hard mode", 1'000);
        const casino::RoundResult result =
                casino::resolveRound(player, 25, 20, 20, 20);

        expect(result.valid, "20-number range accepts a winning round");
        expect(result.payout == 500, "20-number range pays twenty times the bet");
        expect(result.netChange == 475, "range-based payout reports net profit");
    }

    void testLosingRound() {
        player::Player player("Loser", 1'000);
        const casino::RoundResult result =
                casino::resolveRound(player, 100, 7, 3);

        expect(result.valid, "non-matching guess is a valid round");
        expect(!result.won, "non-matching guess loses");
        expect(result.payout == 0, "losing payout is zero");
        expect(result.netChange == -100, "losing net change equals the lost bet");
        expect(player.getBalance() == 900, "losing balance is updated");
    }

    void testInvalidBetsDoNotChangeBalance() {
        player::Player player("Player", 1'000);

        const casino::RoundResult zeroBet =
                casino::resolveRound(player, 0, 5, 5);
        const casino::RoundResult negativeBet =
                casino::resolveRound(player, -1, 5, 5);
        const casino::RoundResult unaffordableBet =
                casino::resolveRound(player, 1'001, 5, 5);

        expect(!zeroBet.valid, "zero bet is rejected");
        expect(!negativeBet.valid, "negative bet is rejected");
        expect(!unaffordableBet.valid, "unaffordable bet is rejected");
        expect(player.getBalance() == 1'000, "invalid bets preserve balance");
    }

    void testInvalidNumbersDoNotChangeBalance() {
        player::Player player("Player", 1'000);

        const casino::RoundResult invalidGuess =
                casino::resolveRound(player, 100, 0, 5);
        const casino::RoundResult invalidWinningNumber =
                casino::resolveRound(player, 100, 5, 11);
        const casino::RoundResult invalidRange =
                casino::resolveRound(player, 100, 1, 1, 0);

        expect(!invalidGuess.valid, "out-of-range guess is rejected");
        expect(!invalidWinningNumber.valid, "out-of-range winning number is rejected");
        expect(!invalidRange.valid, "invalid game range is rejected");
        expect(player.getBalance() == 1'000, "invalid numbers preserve balance");
    }

    void testOverflowDoesNotChangeBalance() {
        constexpr long long maximum =
                std::numeric_limits<long long>::max();
        player::Player player("Player", maximum);

        const casino::RoundResult payoutOverflow =
                casino::resolveRound(player, maximum / 10 + 1, 5, 5);
        const casino::RoundResult balanceOverflow =
                casino::resolveRound(player, 1, 5, 5);

        expect(!payoutOverflow.valid, "unrepresentable payout is rejected");
        expect(!balanceOverflow.valid, "balance overflow is rejected");
        expect(player.getBalance() == maximum, "overflow rejection preserves balance");
    }

    void testNegativeInitialBalanceIsRejected() {
        bool threw = false;
        try {
            player::Player invalidPlayer("Invalid", -1);
        } catch (const std::invalid_argument &) {
            threw = true;
        }

        expect(threw, "negative initial balance is rejected");
    }

    void testStatisticsAndStreaks() {
        player::Player player("Statistics", 1'000);
        casino::Statistics stats;
        const casino::RoundResult firstWin =
                casino::resolveRound(player, 10, 3, 3, 5);
        const casino::RoundResult secondWin =
                casino::resolveRound(player, 10, 4, 4, 5);
        const casino::RoundResult loss =
                casino::resolveRound(player, 10, 1, 2, 5);

        stats.recordRound(firstWin);
        stats.recordRound(secondWin);
        expect(stats.getCurrentWinStreak() == 2,
               "two consecutive wins produce a streak of two");
        stats.recordRound(loss);

        expect(stats.getTotalRounds() == 3, "statistics count valid rounds");
        expect(stats.getWins() == 2, "statistics count wins");
        expect(stats.getLosses() == 1, "statistics count losses");
        expect(stats.getTotalWagered() == 30, "statistics total all wagers");
        expect(stats.getTotalPayout() == 100, "statistics total gross payouts");
        expect(stats.getNetProfit() == 70, "statistics total net changes");
        expect(stats.getLargestWin() == 40, "statistics track largest net win");
        expect(stats.getCurrentWinStreak() == 0, "a loss resets current streak");
        expect(stats.getLongestWinStreak() == 2, "statistics retain longest streak");
        expect(stats.getHistory().size() == 3, "history stores each valid round");

        casino::RoundResult invalid;
        stats.recordRound(invalid);
        expect(stats.getTotalRounds() == 3, "invalid rounds are not recorded");
        expect(stats.getHistory().size() == 3, "invalid rounds do not enter history");
    }

    void testStatisticsSaturateOnOverflow() {
        casino::Statistics stats;
        casino::RoundResult largeRound;
        largeRound.valid = true;
        largeRound.betAmount = std::numeric_limits<long long>::max();
        largeRound.payout = std::numeric_limits<long long>::max();
        largeRound.netChange = std::numeric_limits<long long>::max();

        stats.recordRound(largeRound);
        stats.recordRound(largeRound);

        expect(stats.getTotalWagered() ==
               std::numeric_limits<long long>::max(),
               "wager totals saturate instead of overflowing");
        expect(stats.getTotalPayout() ==
               std::numeric_limits<long long>::max(),
               "payout totals saturate instead of overflowing");
        expect(stats.getNetProfit() ==
               std::numeric_limits<long long>::max(),
               "net totals saturate instead of overflowing");
        expect(stats.totalsOverflowed(), "statistics report saturated totals");
    }
} // namespace

int main() {
    testWinningRound();
    testRangeBasedPayout();
    testLosingRound();
    testInvalidBetsDoNotChangeBalance();
    testInvalidNumbersDoNotChangeBalance();
    testOverflowDoesNotChangeBalance();
    testNegativeInitialBalanceIsRejected();
    testStatisticsAndStreaks();
    testStatisticsSaturateOnOverflow();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }

    std::cout << "All casino core tests passed.\n";
    return 0;
}