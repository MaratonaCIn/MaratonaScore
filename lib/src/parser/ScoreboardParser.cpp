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

#include "parser/ScoreboardParser.hpp"

#include <OpenXLSX.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "score/getScore.hpp"
#include "utils/Blacklist.hpp"
#include "utils/StringUtils.hpp"

namespace MaratonaScore {

std::string cell_to_string(const OpenXLSX::XLCellValue& val) {
    try {
        switch (val.type()) {
            case OpenXLSX::XLValueType::String:
                return val.get<std::string>();
            case OpenXLSX::XLValueType::Integer:
                return std::to_string(val.get<int64_t>());
            case OpenXLSX::XLValueType::Float:
                return std::to_string(val.get<double>());
            case OpenXLSX::XLValueType::Boolean:
                return val.get<bool>() ? "true" : "false";
            case OpenXLSX::XLValueType::Empty:
            case OpenXLSX::XLValueType::Error:
            default:
                return "";
        }
    } catch (...) {
        return "";
    }
}

long long timeToTotalMinutes_noms(const std::string& timeStr) {
    std::stringstream ss(timeStr);
    std::string segment;
    long long parts[4] = {0, 0, 0, 0};  // Days, Hours, Minutes, Seconds

    int i = 0;
    while (std::getline(ss, segment, ':') && i < 4) {
        try {
            parts[i] = std::stoll(segment);
        } catch (...) {
            parts[i] = 0;
        }
        i++;
    }

    long long totalMinutes = (parts[0] * 24 * 60) + (parts[1] * 60) + parts[2];
    return totalMinutes;
}

int penaltyFromString(const std::string& penaltyStr) {
    size_t firstColon = penaltyStr.find(':');

    if (firstColon == std::string::npos || firstColon == 0) {
        return 0;
    }

    for (size_t i = firstColon; i > 0; --i) {
        std::string penaltyPart = penaltyStr.substr(0, i);
        std::string timePart = penaltyStr.substr(i);

        try {
            long long penaltyValue = std::stoll(penaltyPart);
            long long timeValue = timeToTotalMinutes_noms(timePart);

            if (penaltyValue == timeValue) {
                return static_cast<int>(penaltyValue);
            }
        } catch (const std::invalid_argument& e) {
        } catch (const std::out_of_range& e) {
        }
    }

    try {
        return std::stoi(penaltyStr);
    } catch (...) {
        return 0;
    }
}

Contest ScoreboardParser::parse(const std::string& file_path,
                                CONTEST_TYPE contestType) {
    int TIME_LIMIT;

    if (contestType == CONTEST) {
        TIME_LIMIT = Settings::getInstance().CONTEST_TIME_LIMIT;
    } else if (contestType == HOMEWORK) {
        TIME_LIMIT = Settings::getInstance().HOMEWORK_TIME_LIMIT;
    } else {
        throw std::invalid_argument("Invalid contest type");
    }

    OpenXLSX::XLDocument doc;
    doc.open(file_path);
    auto wks = doc.workbook().worksheet(doc.workbook().worksheetNames().at(0));

    uint32_t row_count = wks.rowCount();
    uint32_t col_count = wks.columnCount();

    Contest contest(contestType);
    std::vector<std::pair<Performance, std::string>> temp_performances;

    for (uint32_t r = 2; r <= row_count; ++r) {
        try {
            std::string team_raw = cell_to_string(wks.cell(r, 2).value());

            if (team_raw.empty()) continue;

            std::string teamID =
                team_raw.substr(team_raw.find('(') + 1,
                                team_raw.find(')') - team_raw.find('(') - 1);
            std::string team_name = team_raw.substr(0, team_raw.find('('));

            int problems = stoi(cell_to_string(wks.cell(r, 3).value()));

            std::string penalty_raw = cell_to_string(wks.cell(r, 4).value());
            int penalty = penaltyFromString(penalty_raw);
            int real_penalty = 0;

            Performance performance(0, penalty);

            for (uint32_t c = 5; c <= col_count; ++c) {
                std::string cellValue = cell_to_string(wks.cell(r, c).value());

                if (trim(cellValue).empty()) {
                    continue;
                }

                ProblemStatus status;

                if (cellValue.find('(') != std::string::npos) {
                    size_t pos_start = cellValue.find('(') + 1;
                    size_t pos_end = cellValue.find(')');
                    size_t count = pos_end - pos_start;
                    std::string number_part =
                        cellValue.substr(pos_start, count);

                    status.setStatus(ATTEMPTED);
                    status.setAttempts(std::abs(std::stoi(number_part)));
                    status.setTimeTaken(0);
                }

                cellValue = cellValue.substr(0, cellValue.find('('));

                if (!cellValue.empty()) {
                    int problemPenalty = timeStringToMinutes(cellValue);

                    if (problemPenalty <= TIME_LIMIT) {
                        status.setStatus(SOLVED);
                        real_penalty += problemPenalty;
                        real_penalty += status.getAttempts() * 20;
                    } else {
                        status.setStatus(UPSOLVED);
                    }
                    status.setTimeTaken(problemPenalty);
                }

                char problem_char = static_cast<char>((c - 5) + 'A');
                std::string problem_id(1, problem_char);

                performance.addProblem(problem_id, status);
            }
            performance.setPenalty(real_penalty);
            performance.setProblemsUpsolved(problems -
                                            performance.getProblemsSolved());
            temp_performances.emplace_back(performance, teamID);

        } catch (const std::exception& e) {
            std::cerr << "[WARNING] Pulando linha " << r
                      << ". Erro: " << e.what() << "\n";
        }
    }

    doc.close();

    sort(temp_performances.begin(), temp_performances.end());

    std::vector<std::pair<Performance, std::string>> filtered_performances;
    int newRank = 1;

    for (auto& [performance, teamID] : temp_performances) {
        if (!Blacklist::isBlacklisted(teamID)) {
            performance.setRank(newRank);

            if (newRank <= Settings::getInstance().CONTEST_PERSON_BONUS) {
                double bonus = getRankBonus(contestType, newRank);
                performance.setBonusScore(bonus);
            }

            newRank++;
            filtered_performances.push_back({performance, teamID});
        }
    }

    for (const auto& [performance, teamID] : filtered_performances) {
        contest.addPerformance(teamID, performance);
    }
    return contest;
}

}  // namespace MaratonaScore