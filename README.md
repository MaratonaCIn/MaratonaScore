# MaratonaScore

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/MaratonaCIn/MaratonaScore)
[![Build Status](https://github.com/MaratonaCIn/MaratonaScore/workflows/Build%20and%20Test/badge.svg)](https://github.com/MaratonaCIn/MaratonaScore/actions)
[![License](https://img.shields.io/badge/license-Apache%202.0-green.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-20-00599C.svg?logo=c%2B%2B)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.15+-064F8C.svg?logo=cmake)](https://cmake.org/)

> Performance-based scoring system for competitive programming marathons.

**MaratonaScore** automatically processes contest and homework scoreboards (Excel format exported from vJudge) and generates a unified ranking with weighted scores, ranking bonuses, and finals integration.

## 📌 About Version 2.0.0

This version represents a **complete redesign** with different architectural goals. Version 1.x was a JavaScript-based browser automation solution.

**Version 2.0.0** takes a different approach, built in C++20 with:

- Standalone architecture (shared library + executable)
- YAML-based configuration system
- Excel file processing from vJudge exports
- Cross-platform support (Windows, Linux, macOS)
- Enhanced configurability and extensibility

### Purpose and Use Case

**MaratonaScore** is actively used by **[MaratonaCIn](https://github.com/MaratonaCIn)** (the competitive programming team from CIn-UFPE) for their **team selection process**.

The system is designed for **extended marathon-style competitions** spanning multiple weeks, combining:

- **Regular contests** (typically held on vJudge)
- **Homework assignments** (extended practice problems)
- **Finals** (usually hosted on Codeforces for the top performers)

This format allows comprehensive evaluation of contestants over time, rewarding both contest performance and consistent problem-solving practice.

---

## 🚀 Quickstart

### 1. Build the project

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### 2. Prepare your data

Copy configuration templates:

```bash
mkdir -p data settings
cp templates/settings/* settings/
cp templates/data/* data/
```

Place your vJudge-exported Excel files in the `data/` folder:

- Contests: `1.xlsx`, `2.xlsx`, ..., `N.xlsx`
- Homeworks: `H1.xlsx`, `H2.xlsx`, ..., `HN.xlsx`
- Finals (optional): `finals.txt`

**Important**: Excel files must be exported from **vJudge** with:

- ✅ **Timestamps enabled** (to distinguish in-time vs upsolving submissions)
- ✅ **Upsolving active** (to track post-deadline problem solves)

### 3. Configure the system

Edit [`settings/config.yaml`](templates/settings/config.yaml) with your scoring rules:

```yaml
time_limits:
  contest: 300      # 5 hours
  homework: 9600    # ~1 week

base_values:
  contest: 4        # Base points per problem in contest
  homework: 2       # Base points per problem in homework
  upsolving: 1      # Points for upsolving

score_bonuses:
  contest: 20       # Ranking bonus for 1st place in contests
  homework: 10      # Ranking bonus for 1st place in homeworks

person_bonuses:
  contest: 5        # Number of top solvers who receive linearly decreasing bonus
  homework: 5       # 1st place gets full bonus, 2nd-Nth get progressively less

contest_settings:
  number_of_contests: 10
  ignore_worst_contests: 2  # Drop the 2 worst contests
```

### 4. Run with sample data (recommended for first-time users)

Test the installation with the included sample data:

```bash
cd build/bin
./maratona_score ../../sample/data/ ../../sample/settings/
```

This will process the sample contests and generate `scoreboard.csv` in `build/bin/`.

> **💡 Pro tip**: The `sample/` directory contains real example data that's perfect for:
>
> - Testing your installation
> - Understanding the expected file formats
> - Learning how the scoring system works
> - Debugging configuration changes

### 5. Run with your own data

Once you've prepared your data:

```bash
cd build/bin
./maratona_score ../../data/ ../../settings/
```

The final scoreboard will be generated as `scoreboard.csv` in the current directory.

---

## 💻 Usage

### Command-Line Arguments

```bash
maratona_score [data_path] [settings_path] [output_path]
```

**Arguments** (all optional):

1. **`data_path`**: Directory containing Excel files (default: `./data/`)
2. **`settings_path`**: Directory with config.yaml and blacklist.txt (default: `./settings/`)
3. **`output_path`**: Path for the output CSV file (default: `./scoreboard.csv`)

**Examples:**

```bash
# Use defaults (./data/, ./settings/, ./scoreboard.csv)
./maratona_score

# Custom data and settings directories
./maratona_score ./my_data/ ./my_settings/

# Custom everything including output file
./maratona_score ./2024_data/ ./2024_settings/ ./results/final_scores.csv

# Use sample data for testing
./maratona_score ../../sample/data/ ../../sample/settings/ ./test_output.csv
```

**Note**: Paths ending with `/` are treated as directories. If `output_path` is a directory, the file will be named `scoreboard.csv` within that directory.

---

## 📊 How It Works

The system calculates scores based on three factors:

### 1. **Problems Solved**

Each solved problem earns points depending on activity type and submission time:

- **During contest/homework**: `base_value` points (4 for contests, 2 for homeworks)
- **After deadline (upsolving)**: 1 point

### 2. **Ranking Bonuses**

Top N solvers receive linearly decreasing bonuses based on their placement:

- **Contests**: Top 5 get bonuses (1st: 20, 2nd: 16, 3rd: 12, 4th: 8, 5th: 4)
- **Homeworks**: Top 5 get bonuses (1st: 10, 2nd: 8, 3rd: 6, 4th: 4, 5th: 2)

The number of ranked positions (`person_bonuses`) and max bonus values (`score_bonuses`) are configurable.

### 3. **Worst Results Dropped**

The N worst contests are dropped (configurable), allowing contestants to have bad days without severely hurting their final score.

### Calculation Example

**Scenario**: A team solves 3 problems in a contest (within deadline) and finishes in 2nd place.

```text
Points from problems = 3 × 4 = 12 points
Ranking bonus (top 2) = 16 points  (1st: 20, 2nd: 16, 3rd: 12, 4th: 8, 5th: 4)
─────────────────────────────────────
Total = 28 points
```

---

## 📁 Expected Data Structure

```text
data/
├── 1.xlsx           # Contest 1
├── 2.xlsx           # Contest 2
├── H1.xlsx          # Homework 1
├── H2.xlsx          # Homework 2
├── ...
└── finals.txt       # Finals results (optional)

settings/
├── config.yaml      # System configuration
└── blacklist.txt    # Team IDs to exclude (one per line)
```

### Excel File Format (vJudge Export)

Excel files (`.xlsx`) **must be exported from vJudge** with the following settings:

- **✅ Enable timestamps**: Required to differentiate in-time submissions from upsolving
- **✅ Enable upsolving**: Required to track post-deadline problem solves

The exported scoreboards should follow ICPC-style format with:

- Team information columns (name, ID)
- Problem columns with submissions/acceptances
- Timestamp information

### Finals Format (Manual Entry)

The finals are typically hosted on **Codeforces** (not vJudge), so results must be entered manually in [`data/finals.txt`](templates/data/finals.txt).

**Format**: Plain text file with one team per line in the format:

```text
TeamID ProblemsSolved Penalty
```

**Example**:

```text
alice_team 5 245
bob_squad 4 180
charlie_code 3 120
```

**Fields**:

- `TeamID`: Team identifier (must match IDs in other files)
- `ProblemsSolved`: Number of problems solved during finals
- `Penalty`: Total penalty time (ICPC format)

**Notes**:

- Finals are optional - the system works without them
- This format allows integration of external platforms (Codeforces, AtCoder, etc.)
- The finals contest is added as a special contest after all regular contests
- Scores are calculated using the same rules as regular contests

### Blacklist

To exclude teams from ranking, add their IDs to [`settings/blacklist.txt`](templates/settings/blacklist.txt):

```text
# Lines starting with # are comments
TeamID123
TeamID456
```

---

## 🏗️ Building

### Requirements

- **C++20 compiler**: GCC 10+, Clang 10+, or MSVC 2019+
- **CMake**: 3.15 or higher
- **Internet connection**: For automatic dependency fetching

### Dependencies (managed automatically)

- [OpenXLSX](https://github.com/troldal/OpenXLSX) - Excel file reading
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) - YAML configuration parsing

### Release Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Debug Build

Debug builds append a `d` suffix to binaries:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
# Generates: maratona_scored.exe and MaratonaScoreLibd.dll
```

### Generated Files

```text
build/
├── bin/
│   ├── maratona_score(.exe)      # Main executable
│   └── MaratonaScoreLib.dll/.so  # Shared library
└── lib/
    └── MaratonaScoreLib.lib/.a   # Import library (Windows)
```

To distribute, copy both files from the `build/bin/` folder.

---

## 🗂️ Project Structure

```text
MaratonaScore/
├── src/
│   ├── include/              # Public headers
│   │   ├── models/           # Contest, Contestant, Performance, Scoreboard
│   │   ├── parser/           # ScoreboardParser, FinalParser
│   │   ├── score/            # Scoring calculation logic
│   │   ├── utils/            # Blacklist, Settings, StringUtils
│   │   └── export.hpp        # DLL export macros
│   ├── src/                  # Implementations (.cpp)
│   └── main.cpp              # Executable entry point
├── sample/                   # Sample data for testing
│   ├── data/                 # Example contest Excel files
│   └── settings/             # Example configuration files
├── templates/                # Configuration templates (for your own data)
├── CMakeLists.txt            # Root build configuration
└── README.md
```

### Architecture

The project uses a **library + executable** architecture:

- **MaratonaScoreLib** (DLL/shared library): Contains all business logic
- **maratona_score** (executable): Thin wrapper that calls the library

This allows reusing the library in other projects (GUI, tests, etc.).

---

## 🐛 Troubleshooting

### "MaratonaScoreLib.dll not found"

**Solution**: Run the program from inside `build/bin/` or copy the DLL to the same directory as the executable.

```bash
cd build/bin
./maratona_score ../../data/ ../../settings/
```

### Errors reading Excel files

**Common causes**:

- File doesn't exist or incorrect path
- Excel format incompatible (must be vJudge export with timestamps and upsolving)
- Corrupted file
- Missing timestamp columns (export without timestamps enabled)

**Solution**: Verify the path and ensure files are exported from vJudge with the correct settings.

### Build issues

**Solution**: Clean the build and try again:

```bash
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## 🤝 Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for details on:

- Setting up the development environment
- Code standards
- Submitting pull requests
- Internal project architecture

---

## 📄 License

This project is licensed under the [Apache License 2.0](LICENSE).

```text
Copyright 2025 MaratonaCIn

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
```

---

## 📧 Contact

To report bugs, suggest features, or ask questions, open an [issue](https://github.com/MaratonaCIn/MaratonaScore/issues) in the repository.

---

**Built with ❤️ by the [MaratonaCIn](https://github.com/MaratonaCIn) team**
