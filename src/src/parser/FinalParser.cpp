#include "parser/FinalParser.hpp"

#include "score/getScore.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace MaratonaScore {
    
Contest FinalParser::parse(const std::string& file_path) {
    Contest contest(CONTEST);
    contest.setId("FINALS");

    std::ifstream f_in(file_path);
    if (!f_in.is_open()) {
        throw std::runtime_error("Could not open finals file: " + file_path);
    }

    std::string teamID = "";

    std::vector<std::tuple<int, int , std::string>> temp_performances;

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

} // namespace MaratonaScore
