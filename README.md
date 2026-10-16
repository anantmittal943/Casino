# Casino

Casino is a C++20 terminal number-guessing game. A session can use Easy, Medium, Hard, or a custom number range, with a
fixed `$5,000` starting balance, bounded bets, range-based payouts, live statistics, and round-by-round history.

## Current Features

- Player name and balance tracking
- Easy (1-5), Medium (1-10), Hard (1-20), and custom (1-1000) ranges
- Fixed `$5,000` starting balance and enforced minimum/maximum bets
- Gross winning payout equals the selected range multiplied by the bet
- Random winning number generation
- Session win/loss, wager, payout, profit/loss, largest-win, and streak statistics
- In-memory round history and a final session summary
- CMake-based build configuration

## Requirements

- A C++20-compatible compiler
- CMake 4.1 or newer
- A terminal

## Build and Run

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

Run the executable:

```bash
./build/Casino
```

On Windows, the executable may be located at `build\Debug\Casino.exe` when using a multi-configuration generator, or at
`build\Casino.exe` with a single-configuration generator.

The project can also be opened directly in CLion using the included `CMakeLists.txt`.

## How to Play

1. Enter a player name and select a difficulty or custom range.
2. Start with a `$5,000` balance.
3. Use the menu to play, view statistics, inspect round history, or quit.
4. For each round, place a legal bet and guess a number in the configured range.

The minimum bet is `$1`. The maximum bet is the lesser of the starting balance and `$1,000,000`. A winning bet pays
`bet multiplied by the number range` in total, including the returned stake; for example, a `$100` win in Hard mode
pays `$2,000`.

## Current Limitations

Session history is kept in memory and is discarded when the program exits. Cumulative statistics saturate at their
numeric limits and report when saturation occurs. Money is represented as whole-number units; fractional currency
would require a separate representation.

The game logic has dependency-free CTest coverage. Run it with:

```bash
ctest --test-dir build --output-on-failure
```

## Development Roadmap

### Level 1: Reliable number-guessing game

- Correct betting and payout rules
- Robust input validation and stream recovery
- Balance and round summaries
- A menu for playing, viewing statistics, and quitting

### Level 2: Complete terminal casino

- Multiple games such as dice, blackjack, and roulette
- Game-specific betting rules
- Player profiles and session history
- Achievements and statistics
- Additional configurable betting limits

### Level 3: Maintainable C++ architecture

- Separate headers and implementation files
- Reusable game interfaces and components
- STL containers and algorithms
- File-based save/load with data validation
- Unit tests for betting, payouts, statistics, and edge cases
- Logging, exception handling, and automated builds

### Level 4: Portfolio features

- Leaderboards and persistent player profiles
- Session analytics and fairness simulations
- SQLite-backed storage
- A richer terminal interface
- Optional networking or multiplayer support

## Suggested Structure

As the project grows, the code can be split into components similar to:

```text
TerminalCasino/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Player.h
│   ├── Game.h
│   ├── NumberGuessingGame.h
│   ├── RandomGenerator.h
│   └── Statistics.h
├── src/
│   ├── main.cpp
│   ├── Player.cpp
│   ├── NumberGuessingGame.cpp
│   └── Statistics.cpp
└── tests/
    ├── PlayerTests.cpp
    └── BettingTests.cpp
```

The project should grow incrementally: make the existing game reliable and testable before adding additional casino
games or a more elaborate user interface.
