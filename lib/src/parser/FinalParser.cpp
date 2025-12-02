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

#include "parser/FinalParser.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "score/getScore.hpp"

namespace MaratonaScore {

Contest FinalParser::parse(const std::string& file_path) {
    Contest contest(CONTEST);
    contest.setId("FINALS");

    std::ifstream f_in(file_path);
    if (!f_in.is_open()) {
        throw std::runtime_error("Could not open finals file: " + file_path);
    }

    std::string teamID = "";

    std::vector<std::tuple<int, int, std::string>> temp_performances;

    while (f_in >> teamID) {
        if (teamID[0] == '#') continue;
        int problemsSolved = 0;
        int penalty = 0;

        f_in >> problemsSolved >> penalty;

        temp_performances.emplace_back(problemsSolved, penalty, teamID);
    }

    f_in.close();

    std::sort(temp_performances.begin(), temp_performances.end(),
              [](const auto& a, const auto& b) {
                  if (std::get<0>(a) != std::get<0>(b)) {
                      return std::get<0>(a) > std::get<0>(b);
                  }
                  return std::get<1>(a) < std::get<1>(b);
              });

    int rank = 1;
    for (const auto& [problemsSolved, penalty, teamID] : temp_performances) {
        Performance performance(rank, penalty);
        performance.setProblemsUpsolved(0);
        performance.setBonusScore(getRankBonus(CONTEST, rank));

        for (int i = 0; i < problemsSolved; ++i) {
            char problem_char = static_cast<char>(i + 'A');
            std::string problem_id(1, problem_char);
            ProblemStatus status(SOLVED, 0, 0);
            performance.addProblem(problem_id, status);
        }

        contest.addPerformance(teamID, performance);
        rank++;
    }

    return contest;
}

}  // namespace MaratonaScore
