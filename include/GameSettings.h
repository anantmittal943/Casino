#ifndef CASINO_GAME_SETTINGS_H
#define CASINO_GAME_SETTINGS_H

namespace casino {
    enum class Difficulty {
        Easy,
        Medium,
        Hard,
        Custom
    };

    constexpr int minimumCustomRange = 2;
    constexpr int maximumCustomRange = 1'000;

    struct GameSettings {
        int maxNumber = 10;
        long long startingBalance = 5'000;
        long long minBet = 1;
        long long maxBet = 1'000;
    };

    constexpr bool isValidCustomRange(long long maxNumber) noexcept {
        return maxNumber >= minimumCustomRange &&
               maxNumber <= maximumCustomRange;
    }

    constexpr bool setDifficulty(
        GameSettings &settings,
        Difficulty difficulty,
        long long customRange = 0) noexcept {
        switch (difficulty) {
            case Difficulty::Easy:
                settings.maxNumber = 5;
                return true;
            case Difficulty::Medium:
                settings.maxNumber = 10;
                return true;
            case Difficulty::Hard:
                settings.maxNumber = 20;
                return true;
            case Difficulty::Custom:
                if (!isValidCustomRange(customRange)) {
                    return false;
                }
                settings.maxNumber = static_cast<int>(customRange);
                return true;
        }
        return false;
    }
} // namespace casino

#endif // CASINO_GAME_SETTINGS_H
