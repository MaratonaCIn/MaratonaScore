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

#include "models/Scoreboard.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include "score/getscore.hpp"
#include "utils/Blacklist.hpp"
#include "utils/Settings.hpp"

namespace MaratonaScore {

void Scoreboard::addContest(const Contest& contest, int index) {
    for (auto [teamID, performance] : contest.getPerformances()) {
        contestants[teamID].ContestsPerformance[contest.getId()] = &performance;

        if (contest.getType() == CONTEST) {
            double solve = getSolveScore(contest.getType(), performance, index);
            double upsolve = getUpsolveScore(performance);
            double bonus = performance.getBonusScore();

            contestants[teamID].addScoreContest(solve);
            contestants[teamID].addScoreUpsolved(upsolve);
            contestants[teamID].addScoreBonus(bonus);

            contestants[teamID].contestScores[contest.getId()] = {solve, bonus};
        }

        if (contest.getType() == HOMEWORK) {
            double solve = getSolveScore(contest.getType(), performance, index);
            double upsolve = getUpsolveScore(performance);
            double bonus = performance.getBonusScore();

            contestants[teamID].addScoreHomework(solve);
            contestants[teamID].addScoreUpsolved(upsolve);
            contestants[teamID].addScoreBonus(bonus);
        }
    }
}

void Scoreboard::renderCSV(std::ostream& os) const {
    os << "Team ID,Total Contest Score,Total Homework Score,Total Upsolved "
          "Score,Bonus Score,Overall Score\n";

    // Create sorted list of contestants by total score (descending)
    std::vector<std::pair<std::string, const Contestant*>> sortedContestants;

    for (const auto& [teamID, contestant] : contestants) {
        if (!Blacklist::isBlacklisted(teamID)) {
            sortedContestants.push_back({teamID, &contestant});
        }
    }

    std::sort(sortedContestants.begin(), sortedContestants.end(),
              [](const auto& a, const auto& b) {
                  return a.second->getTotalScore() > b.second->getTotalScore();
              });

    for (const auto& [teamID, contestant] : sortedContestants) {
        os << teamID << "," << contestant->getScoreContest() << ","
           << contestant->getScoreHomework() << ","
           << contestant->getScoreUpsolved() << ","
           << contestant->getScoreBonus() << "," << contestant->getTotalScore()
           << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Scoreboard& sb) {
    for (auto& [teamID, contestant] : sb.contestants) {
        if (Blacklist::isBlacklisted(teamID)) {
            continue;
        }

        os << "Contestant ID: " << teamID << "\n";
        os << "  Total Contest Score: " << contestant.getScoreContest() << "\n";
        os << "  Total Homework Score: " << contestant.getScoreHomework()
           << "\n";
        os << "  Total Upsolved Score: " << contestant.getScoreUpsolved()
           << "\n";
        os << "  Bonus Score: " << contestant.getScoreBonus() << "\n";
        os << "  Overall Score: " << contestant.getTotalScore() << "\n";
    }

    return os;
}

void Scoreboard::applyContestFiltering() {
    if (Settings::getInstance().IGNORE_WORST_CONTESTS == 0) return;

    for (auto& [teamID, contestant] : contestants) {
        std::vector<std::pair<std::string, double>> allContestScores;

        for (int i = 1; i <= Settings::getInstance().NUMBER_OF_CONTESTS; i++) {
            std::string contestId = std::to_string(i);

            auto it = contestant.contestScores.find(contestId);
            if (it != contestant.contestScores.end()) {
                allContestScores.push_back({contestId, it->second.total()});
            } else {
                allContestScores.push_back({contestId, 0.0});
            }
        }

        std::sort(
            allContestScores.begin(), allContestScores.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });

        double solveToSubtract = 0.0;
        double bonusToSubtract = 0.0;

        for (int i = 0; i < Settings::getInstance().IGNORE_WORST_CONTESTS;
             i++) {
            const std::string& contestId = allContestScores[i].first;

            auto it = contestant.contestScores.find(contestId);
            if (it != contestant.contestScores.end()) {
                solveToSubtract += it->second.solve;
                bonusToSubtract += it->second.bonus;
            }
        }

        contestant.scoreContest -= solveToSubtract;
        contestant.scoreBonus -= bonusToSubtract;
        contestant.fixScore();
    }
}

}  // namespace MaratonaScore