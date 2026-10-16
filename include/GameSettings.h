#ifndef CASINO_GAME_SETTINGS_H
#define CASINO_GAME_SETTINGS_H

namespace casino {
    struct GameSettings {
        int maxNumber = 10;
        long long startingBalance = 5'000;
        long long minBet = 1;
        long long maxBet = 1'000;
    };
} // namespace casino

#endif // CASINO_GAME_SETTINGS_H
