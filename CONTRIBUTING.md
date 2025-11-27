# Contributing to MaratonaScore

Thank you for considering contributing to MaratonaScore! This document provides guidelines and information to help you contribute effectively.

---

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Environment](#development-environment)
- [Project Architecture](#project-architecture)
- [Coding Standards](#coding-standards)
- [Making Changes](#making-changes)
- [Testing](#testing)
- [Submitting Changes](#submitting-changes)

---

## Code of Conduct

Be respectful, inclusive, and constructive in all interactions. We aim to maintain a welcoming environment for contributors of all backgrounds and experience levels.

---

## Getting Started

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 10+, or MSVC 2019+)
- CMake 3.15+
- Git
- Basic understanding of competitive programming concepts

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:

```bash
git clone https://github.com/YOUR_USERNAME/MaratonaScore.git
cd MaratonaScore
```

3. Add the upstream repository:

```bash
git remote add upstream https://github.com/MaratonaCIn/MaratonaScore.git
```

---

## Development Environment

### Building for Development

Use debug builds during development for better debugging symbols:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Running Tests

Currently the project doesn't have automated tests, but you can test manually:

1. Place sample Excel files in `data/`
2. Run the executable:

```bash
cd build/bin
./maratona_scored ../../data/ ../../settings/
```

3. Verify the generated `scoreboard.csv`

### Recommended IDE Setup

- **Visual Studio Code**: Install C/C++ and CMake Tools extensions
- **CLion**: Native CMake support
- **Visual Studio**: Open as CMake project

---

## Project Architecture

### Overview

MaratonaScore uses a **library + executable** architecture:

```text
┌─────────────────┐
│ maratona_score  │  ← Thin executable wrapper
│   (main.cpp)    │
└────────┬────────┘
         │ links to
         ▼
┌──────────────────┐
│ MaratonaScoreLib │  ← Shared library (DLL/.so)
│  (all logic)     │
└─────────┬────────┘
          │ uses
          ▼
    ┌──────────┐
    │ OpenXLSX │  ← Statically linked
    │ yaml-cpp │
    └──────────┘
```

### Core Components

#### 1. **Models** ([src/include/models/](src/include/models/))

Data structures representing the problem domain:

- **Contest**: Represents a single contest or homework with performances
- **Contestant**: Individual or team participating in contests
- **Performance**: Single problem performance (solved/time/upsolving)
- **Scoreboard**: Aggregates all contests and generates final rankings

#### 2. **Parsers** ([src/include/parser/](src/include/parser/))

- **ScoreboardParser**: Parses vJudge Excel exports (`.xlsx`) into `Contest` objects
  - Handles timestamp parsing to detect upsolving
  - Extracts team information and problem submissions

- **FinalParser**: Parses finals results from text files

#### 3. **Scoring Logic** ([src/include/score/](src/include/score/))

- **getScore**: Core scoring algorithm that:
  - Calculates base points from problems solved
  - Applies time-based modifiers (in-time vs upsolving)
  - Adds participation and team size bonuses
  - Drops worst N contests

#### 4. **Utilities** ([src/include/utils/](src/include/utils/))

- **Settings**: YAML configuration loader
- **Blacklist**: Team exclusion manager
- **StringUtils**: String manipulation helpers

### DLL Export/Import System

The library uses `MARATONASCORE_API` macro ([export.hpp](src/include/export.hpp)) for cross-platform DLL exports:

```cpp
// When building the library
#define MARATONASCORE_API __declspec(dllexport)  // Windows
#define MARATONASCORE_API __attribute__((visibility("default")))  // Linux

// When using the library
#define MARATONASCORE_API __declspec(dllimport)  // Windows
```

**Important**: All public classes must use this macro:

```cpp
class MARATONASCORE_API Contest {
    // ...
};
```

---

## Coding Standards

### C++ Style

#### Naming Conventions

- **Classes/Structs**: `PascalCase`
  ```cpp
  class ScoreboardParser { };
  ```

- **Functions/Methods**: `camelCase`
  ```cpp
  void calculateScore();
  ```

- **Variables**: `snake_case`
  ```cpp
  int total_score = 0;
  ```

- **Constants**: `UPPER_SNAKE_CASE`
  ```cpp
  constexpr int MAX_CONTESTS = 100;
  ```

- **Namespaces**: `PascalCase`
  ```cpp
  namespace MaratonaScore { }
  ```

#### Code Organization

- **One class per file** (header + implementation)
- **Headers in `src/include/`**, implementations in `src/src/`
- **Include guards**: Use `#ifndef`/`#define` (not `#pragma once`)

```cpp
#ifndef MSCR_MODELS_CONTEST_HPP
#define MSCR_MODELS_CONTEST_HPP
// ...
#endif
```

#### Modern C++ Practices

- Use C++20 features where appropriate
- Prefer `auto` for complex types
- Use `const` and `constexpr` liberally
- Prefer smart pointers over raw pointers
- Use range-based for loops

```cpp
// Good
for (const auto& contestant : contestants) {
    processContestant(contestant);
}

// Avoid
for (size_t i = 0; i < contestants.size(); i++) {
    processContestant(contestants[i]);
}
```

### File Headers

All source files must include the Apache License boilerplate:

```cpp
//    Copyright 2025 MaratonaCIn
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
```

### CMake Standards

- Use target-based approach (not global variables)
- Document complex configurations with comments
- Include Apache license header in CMakeLists.txt

---

## Making Changes

### Workflow

1. **Create a feature branch**:

```bash
git checkout -b feature/your-feature-name
```

2. **Make your changes** following coding standards

3. **Build and test**:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
# Test manually with sample data
```

4. **Commit with descriptive messages**:

```bash
git add .
git commit -m "Add: Feature description

Detailed explanation of what changed and why."
```

### Commit Message Guidelines

Follow conventional commits format:

- `Add:` New feature or functionality
- `Fix:` Bug fix
- `Refactor:` Code refactoring (no functional change)
- `Docs:` Documentation changes
- `Build:` Build system or dependency changes
- `Test:` Adding or updating tests

**Example**:
```
Add: Support for multi-page Excel scoreboards

- Modified ScoreboardParser to handle multiple sheets
- Added worksheet index parameter to parse()
- Updated documentation with examples
```

### What to Contribute

#### Good First Issues

- Documentation improvements
- Code comments and clarifications
- Example data files
- Bug fixes with clear reproduction steps

#### Feature Ideas

- Unit tests (currently missing!)
- GUI wrapper for the library
- Additional export formats (JSON, HTML)
- Performance optimizations
- Better error messages
- Support for other scoreboard formats

#### Areas Needing Attention

- [ ] Comprehensive test suite
- [ ] Input validation and error handling
- [ ] Configuration validation
- [ ] Performance profiling for large datasets
- [ ] Cross-platform testing (Windows/Linux/macOS)

---

## Testing

### Manual Testing Checklist

Before submitting changes, test:

- [ ] **Build succeeds** on Release and Debug modes
- [ ] **Sample data processes** without errors
- [ ] **Scoreboard generates** valid CSV output
- [ ] **Configuration changes** are respected
- [ ] **Blacklist works** (excluded teams don't appear)
- [ ] **No memory leaks** (use valgrind on Linux or VS debugger on Windows)

### Adding Test Data

If your change affects parsing or scoring:

1. Add sample Excel files to `sample/` directory
2. Document expected behavior in comments
3. Verify output matches expectations

---

## Submitting Changes

### Pull Request Process

1. **Update your fork**:

```bash
git fetch upstream
git rebase upstream/main
```

2. **Push to your fork**:

```bash
git push origin feature/your-feature-name
```

3. **Create Pull Request** on GitHub:
   - Use a descriptive title
   - Reference related issues (`Fixes #123`)
   - Describe what changed and why
   - Include testing notes

### Pull Request Template

```markdown
## Description
Brief description of changes

## Motivation
Why is this change needed?

## Changes
- List of specific changes

## Testing
How was this tested?

## Checklist
- [ ] Code follows project style guidelines
- [ ] License headers included in new files
- [ ] Documentation updated if needed
- [ ] Builds successfully in Debug and Release
- [ ] Tested with sample data
```

### Review Process

- Maintainers will review your PR
- Address feedback by pushing new commits to your branch
- Once approved, your PR will be merged

---

## Questions?

- Open an [issue](https://github.com/MaratonaCIn/MaratonaScore/issues) for questions
- Tag with `question` label
- Be specific and include context

---

## License

By contributing, you agree that your contributions will be licensed under the Apache License 2.0.

---

**Thank you for contributing to MaratonaScore! 🎉**
