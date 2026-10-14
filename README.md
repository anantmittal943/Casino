# Casino

Casino is a small C++20 command-line game built as a starting point for a larger casino project. The current version
lets a player create a session with a starting balance of `$1,000`, place bets, and guess a randomly generated number
from 1 to 10.

## Current Features

- Player name and balance tracking
- Number guessing game with a configurable range in the source
- Random winning number generation
- Bet wins and losses
- Replay loop until the player chooses to stop or runs out of money
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

1. Enter a player name.
2. Enter a bet amount.
3. Guess a number from 1 to 10.
4. Choose whether to play another round.

The current implementation starts every session with `$1,000`.

## Current Limitations

This is intentionally an early version. Before expanding the game, the following correctness and design issues should be
addressed:

- Validate negative, zero, non-numeric, and out-of-range input.
- Deduct the bet before calculating a win payout and document whether payouts are gross winnings or net profit.
- Reuse one random-number engine instead of creating one for every round.
- Encapsulate player state rather than exposing `name` and `balance` as public fields.
- Return a structured round result instead of combining input, game rules, and output in `playRound()`.
- Use a money representation with appropriate overflow checks.
- Track wins, losses, net profit, largest win, and round history.

## Development Roadmap

### Level 1: Reliable number-guessing game

- Correct betting and payout rules
- Robust input validation and stream recovery
- Configurable number ranges and difficulty settings
- Balance and round summaries
- A menu for playing, viewing statistics, changing settings, and quitting

### Level 2: Complete terminal casino

- Multiple games such as dice, blackjack, and roulette
- Game-specific betting rules
- Player profiles and session history
- Achievements and statistics
- Configurable starting balances and betting limits

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
