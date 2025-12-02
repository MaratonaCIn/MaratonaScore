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

#include <exception>
#include <fstream>
#include <iostream>

#include "maratona_score/models/Contest.hpp"
#include "maratona_score/models/Scoreboard.hpp"
#include "maratona_score/parser/FinalParser.hpp"
#include "maratona_score/utils/Blacklist.hpp"
#include "maratona_score/utils/Settings.hpp"

using namespace MaratonaScore;

int main(int argc, char* argv[]) {
#ifdef DEPRECATED_CLI
    std::cerr << "\n";
    std::cerr << "⚠️  WARNING: This CLI is deprecated.\n";
    std::cerr << "   Please use 'MaratonaScoreCLI' instead for new features:\n";
    std::cerr << "     - Custom file naming support\n";
    std::cerr << "     - Multiple output formats (JSON, HTML, Markdown)\n";
    std::cerr << "     - Powerful inspection and analysis commands\n";
    std::cerr << "   This executable will be removed in v" NEW_VERSION "\n";
    std::cerr << "\n";
#endif

    std::string base_path = (argc > 1) ? argv[1] : "./data/";

    std::string settings_path = (argc > 2) ? argv[2] : "./settings/";

    std::string export_path = (argc > 3) ? argv[3] : "./scoreboard.csv";

    if (!base_path.empty() && base_path.back() != '/' &&
        base_path.back() != '\\') {
        base_path += "/";
    }
    if (!settings_path.empty() && settings_path.back() != '/' &&
        settings_path.back() != '\\') {
        settings_path += "/";
    }
    if (!export_path.empty() &&
        export_path.substr(export_path.size() - 4) != ".csv" &&
        export_path.back() != '/' && export_path.back() != '\\') {
        export_path += "/scoreboard.csv";
    }

    Settings::getInstance().loadFromFile(settings_path + "config.yaml");

    Blacklist::loadFromFile(settings_path + "blacklist.txt");

    Scoreboard scoreboard;

    for (int i = 0; i < Settings::getInstance().NUMBER_OF_CONTESTS; i++) {
        try {
            scoreboard.addContest(
                Contest(base_path + std::to_string(i + 1) + ".xlsx", CONTEST),
                i);
        } catch (const std::exception& e) {
            std::cerr << "Could not load contest " << (i + 1) << ": "
                      << e.what() << '\n';
        }

        try {
            scoreboard.addContest(
                Contest(base_path + "H" + std::to_string(i + 1) + ".xlsx",
                        HOMEWORK),
                i);
        } catch (const std::exception& e) {
            std::cerr << "Could not load homework " << (i + 1) << ": "
                      << e.what() << '\n';
        }
    }

    scoreboard.applyContestFiltering();

    try {
        scoreboard.addContest(FinalParser().parse(base_path + "finals.txt"),
                              Settings::getInstance().NUMBER_OF_CONTESTS);
    } catch (const std::exception& e) {
        std::cerr << "Could not load finals: " << e.what() << '\n';
    }

    std::ofstream out(export_path);
    scoreboard.renderCSV(out);

    return 0;
}
