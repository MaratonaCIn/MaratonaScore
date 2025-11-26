#include "utils/Blacklist.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

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

void Blacklist::clear() {
    blacklistedTeams.clear();
}

} // namespace MaratonaScore
