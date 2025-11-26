# MaratonaScore Templates

This directory contains template files for configuring and running MaratonaScore.

## Directory Structure

```
templates/
├── settings/
│   ├── config.yaml      # Main configuration file
│   └── blacklist.txt    # Team exclusion list
├── data/
│   └── finals.txt       # Finals results template
└── README.md           # This file
```

## Files

### settings/config.yaml
Main configuration file that defines scoring rules and parameters:
- Time limits for contests and homework
- Base values for different activity types
- Score bonuses
- Contest settings

### settings/blacklist.txt
List of team IDs to exclude from scoring (one per line).
Lines starting with `#` are treated as comments.

### data/finals.txt
Finals results file containing team IDs and scores.

## Usage

1. Copy template directories to your working directory:
   ```bash
   mkdir -p data settings
   cp -r templates/settings/* settings/
   cp -r templates/data/* data/
   ```

2. Place your contest Excel files in the data directory:
   - Contest files: `1.xlsx`, `2.xlsx`, ..., `N.xlsx`
   - Homework files: `H1.xlsx`, `H2.xlsx`, ..., `HN.xlsx`
   - Finals file: `finals.txt`

3. Edit `settings/config.yaml` to match your scoring rules

4. Edit `settings/blacklist.txt` to add teams to exclude (if any)

5. Edit `data/finals.txt` with finals results (if applicable)

6. Run the program:
   ```bash
   # Using default paths (./data/ and ./settings/)
   maratona_score

   # Or specify custom paths
   maratona_score <data_path> <settings_path>

   # Example with custom paths
   maratona_score ./my_data/ ./my_settings/
   ```

The program will generate a `scoreboard.csv` file with the final results.
