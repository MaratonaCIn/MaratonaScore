# MaratonaScore v2.0.0

A point-based scoring system for competitive programming marathons and contests. This system processes Excel scoreboard files and generates comprehensive rankings for contestants across multiple contests and homework assignments.

## Features

- Process multiple contest and homework scoreboards from Excel files (.xlsx)
- Automatic score calculation and ranking
- Blacklist support for excluding specific contestants
- Finals scoring integration
- CSV export for final scoreboard
- Contest filtering capabilities

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.14 or higher
- Internet connection (for fetching OpenXLSX dependency)

## Dependencies

The project automatically fetches its dependencies via CMake FetchContent:

- [OpenXLSX](https://github.com/troldal/OpenXLSX) - For reading Excel files (linked as static library)

## Architecture

The project uses a **library + executable** architecture:

- **MaratonaScoreLib** (shared library/DLL)
  - Contains all business logic, models, parsers, and scoring
  - Statically links OpenXLSX internally
  - Output: `MaratonaScoreLib.dll` (Windows) or `MaratonaScoreLib.so` (Linux)

- **maratona_score** (executable)
  - Thin wrapper containing only [main.cpp](src/main.cpp)
  - Links against MaratonaScoreLib dynamically
  - Output: `maratona_score.exe` (Windows) or `maratona_score` (Linux)

## Project Structure

```
MaratonaScoreV2/
├── src/
│   ├── include/          # Header files
│   │   ├── models/       # Data models (Contest, Contestant, Performance, Scoreboard)
│   │   ├── parser/       # Excel and file parsers
│   │   ├── score/        # Scoring logic
│   │   ├── utils/        # Utility functions (Blacklist, StringUtils)
│   │   └── export.hpp    # DLL export/import macros
│   ├── src/              # Implementation files (compiled into MaratonaScoreLib)
│   └── main.cpp          # Entry point (compiled into executable)
├── sample/               # Sample Excel files for testing
│   └── settings/         # Configuration files
├── CMakeLists.txt        # Build configuration
└── README.md
```

## Building the Project

### Release Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Debug Build

Debug builds append a 'd' suffix to binaries:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

This generates `maratona_scored.exe` and `MaratonaScoreLibd.dll`.

### Build Output

After building, all executables and DLLs are in `build/bin/`:

```
build/
├── bin/                          # Ready-to-run distribution
│   ├── maratona_score.exe       # Main executable (Release)
│   ├── maratona_scored.exe      # Main executable (Debug)
│   ├── MaratonaScoreLib.dll     # Shared library (Release)
│   └── MaratonaScoreLibd.dll    # Shared library (Debug)
└── lib/                          # Import libraries
    ├── MaratonaScoreLib.dll.a
    └── MaratonaScoreLibd.dll.a
```

**Note:** Everything needed to run the program is in `build/bin/` - just the `.exe` and `.dll` files.

## Usage

### Input Files Structure

The program expects the following file structure in your data directory:

```
data/
├── blacklist.txt        # List of contestants to exclude (one per line)
├── 1.xlsx               # Contest 1 scoreboard
├── H1.xlsx              # Homework 1 scoreboard
├── 2.xlsx               # Contest 2 scoreboard
├── H2.xlsx              # Homework 2 scoreboard
├── ...
├── N.xlsx               # Contest N scoreboard
├── HN.xlsx              # Homework N scoreboard
└── finals.txt           # Finals results
```

### Running the Program

1. Update the `base_path` in [main.cpp](src/main.cpp:14) to point to your data directory
2. Build the project (see above)
3. Run from the `build/bin/` directory:

```bash
cd build/bin
./maratona_score        # Linux/macOS
.\maratona_score.exe    # Windows
```

The program will generate `scoreboard.csv` in the current working directory.

**Distribution:** To distribute the program, copy both files from `build/bin/`:

- `maratona_score.exe` (or `maratona_scored.exe` for debug)
- `MaratonaScoreLib.dll` (or `MaratonaScoreLibd.dll` for debug)

### Configuration

Configure the number of contests in [utils/Settings.hpp](src/include/utils/Settings.hpp):

```cpp
namespace Settings {
    constexpr int NUMBER_OF_CONTESTS = 10; // Adjust as needed
}
```

## Input File Format

### Excel Scoreboards (.xlsx)

Contest and homework Excel files should follow a standard competitive programming scoreboard format with:
- Contestant information (name, ID, etc.)
- Problem scores/submissions
- Total scores

### Blacklist (blacklist.txt)

Plain text file with one contestant identifier per line:
```
contestant1
contestant2
...
```

### Finals (finals.txt)

Special format for finals results (parsed by FinalParser).

## Output

The program generates a CSV file (`scoreboard.csv`) containing:
- Contestant rankings
- Individual contest scores
- Homework scores
- Final scores
- Overall ratings

## Development

### Key Components

- **Contest**: Represents a single contest or homework assignment
- **Contestant**: Represents a participant with their performance data
- **Performance**: Individual performance metrics
- **Scoreboard**: Aggregates all contests and generates rankings
- **ScoreboardParser**: Parses Excel files into Contest objects
- **FinalParser**: Parses finals results from text files
- **Blacklist**: Manages contestant exclusions

### Build System Details

The project uses CMake with:

- **C++20** standard
- **Shared library** architecture for better modularity
- **FetchContent** for dependency management
- **Debug postfix** ('d') for debug builds
- Optimized build settings (demos/tests disabled for dependencies)

### Adding New Features

1. Models are in [src/include/models/](src/include/models/)
2. Parsers are in [src/include/parser/](src/include/parser/)
3. Scoring logic is in [src/include/score/](src/include/score/)
4. All implementation goes into `MaratonaScoreLib` for reusability

## Troubleshooting

### Build Issues

If you encounter dependency fetching issues:
```bash
# Clear the build directory and rebuild
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Missing DLL Errors

If you get "MaratonaScoreLib.dll not found" errors:

- Ensure you're running the executable from `build/bin/` directory
- Or copy both the `.exe` and `.dll` to the same directory

### Excel File Errors

If a contest file fails to load:
- Verify the file exists and path is correct
- Check the Excel file format matches expected structure
- Review error messages in console output

## License

This project is part of the MaratonaCIn program.

## Contributing

Contributions are welcome! Please ensure:

- Code follows C++20 standards
- New features include appropriate error handling
- Changes maintain backward compatibility with existing data files
- DLL exports are properly handled (use `MARATONASCORE_API` macro if needed)

## Contact

For issues, questions, or contributions, please open an issue in the repository.
