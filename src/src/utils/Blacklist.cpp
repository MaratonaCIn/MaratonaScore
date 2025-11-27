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

#include "utils/Blacklist.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace MaratonaScore {

std::set<std::string> Blacklist::blacklistedTeams;

void Blacklist::loadFromFile(const std::string& filepath) {
    blacklistedTeams.clear();

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[WARNING] Blacklist file not found: " << filepath
                  << ". Proceeding without blacklist.\n";
        return;
    }

    std::string teamID;
    while (std::getline(file, teamID)) {
        // Trim whitespace
        teamID.erase(0, teamID.find_first_not_of(" \t\r\n"));
        teamID.erase(teamID.find_last_not_of(" \t\r\n") + 1);

        if (!teamID.empty() && teamID[0] != '#') {
            blacklistedTeams.insert(teamID);
        }
    }

    file.close();
    std::cout << "[INFO] Loaded " << blacklistedTeams.size()
              << " blacklisted team(s)\n";
}

bool Blacklist::isBlacklisted(const std::string& teamID) {
    return blacklistedTeams.find(teamID) != blacklistedTeams.end();
}

const std::set<std::string>& Blacklist::getBlacklistedTeams() {
    return blacklistedTeams;
}

void Blacklist::clear() { blacklistedTeams.clear(); }

}  // namespace MaratonaScore
