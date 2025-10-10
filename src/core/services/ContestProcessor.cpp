#include "MaratonaScore/core/services/ContestProcessor.h"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "MaratonaScore/core/services/ScoreCalculator.h"

using json = nlohmann::json;

namespace MaratonaScore {
namespace core {
namespace services {

std::string ContestProcessor::getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    char buffer[20];
    std::sprintf(buffer, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon,
                 ltm->tm_mday);
    return std::string(buffer);
}

bool ContestProcessor::processScoreboard(
    const std::string& contest_id, const std::string& filename,
    const std::string& contest_type, int contest_duration_minutes,
    const models::ScoringConfig& config,
    std::map<std::string, models::CompetitorData>& competitors,
    std::map<std::string, models::ProcessedContest>& processed_contests) {
    // Open and parse JSON file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open scoreboard file: " << filename
                  << "\n";
        return false;
    }

    json scoreboard;
    try {
        file >> scoreboard;
    } catch (const json::parse_error& e) {
        std::cerr << "[ERROR] JSON parse error: " << e.what() << "\n";
        file.close();
        return false;
    }
    file.close();

    // Check if contest was already processed
    bool is_first_time =
        (processed_contests.find(contest_id) == processed_contests.end());

    if (is_first_time) {
        std::cout << "\n[NEW] Processing NEW contest: " << contest_id << " ("
                  << contest_type << ")\n";
    } else {
        std::cout << "\n[UPDATE] UPDATING contest: " << contest_id
                  << " (upsolving only)\n";
    }

    // Parse scoreboard and calculate stats
    std::vector<TempResult> results;
    int max_ontime_problems = 0;

    for (const auto& entry : scoreboard) {
        TempResult result;

        // Handle null user_name - use team_name as fallback
        if (entry["user_name"].is_null()) {
            result.user_name = entry["team_name"].get<std::string>();
        } else {
            result.user_name = entry["user_name"].get<std::string>();
        }

        result.team_name = entry["team_name"].get<std::string>();
        result.total_solved = entry["score"].get<int>();
        result.penalty = entry["penalty"].get<int>();

        // Calculate ontime_solved
        result.ontime_solved = 0;
        const json& problems = entry["problems"];

        for (auto it = problems.begin(); it != problems.end(); ++it) {
            const auto& problem_data = it.value();

            if (problem_data["solved"].get<bool>() &&
                !problem_data["time"].is_null()) {
                std::string time_str = problem_data["time"].get<std::string>();
                int hours = 0, minutes = 0, seconds = 0;
                std::sscanf(time_str.c_str(), "%d:%d:%d", &hours, &minutes,
                            &seconds);
                int total_minutes = hours * 60 + minutes;

                if (total_minutes <= contest_duration_minutes) {
                    result.ontime_solved++;
                }
            }
        }

        result.upsolving_count = result.total_solved - result.ontime_solved;
        max_ontime_problems =
            std::max(max_ontime_problems, result.ontime_solved);

        results.push_back(result);
    }

    // Re-rank based on ontime performance
    std::sort(results.begin(), results.end(),
              [](const TempResult& a, const TempResult& b) {
                  if (a.ontime_solved != b.ontime_solved)
                      return a.ontime_solved > b.ontime_solved;
                  return a.penalty < b.penalty;
              });

    for (size_t i = 0; i < results.size(); i++) {
        results[i].rank = static_cast<int>(i + 1);
    }

    int total_participants = static_cast<int>(results.size());

    // Update processed contest info
    models::ProcessedContest pc;
    pc.contest_id = contest_id;
    pc.type = contest_type;
    pc.duration_minutes = contest_duration_minutes;
    pc.total_participants = total_participants;
    pc.max_problems_solved = max_ontime_problems;
    pc.last_updated_date = getCurrentDate();

    if (is_first_time) {
        pc.first_processed_date = getCurrentDate();
    } else {
        pc.first_processed_date =
            processed_contests[contest_id].first_processed_date;
    }

    // Process each participant
    for (const auto& result : results) {
        pc.participants.insert(result.user_name);

        // Ensure competitor exists
        if (competitors.find(result.user_name) == competitors.end()) {
            models::CompetitorData new_comp;
            new_comp.user_name = result.user_name;
            new_comp.team_name = result.team_name;
            competitors[result.user_name] = new_comp;
        }

        auto& comp = competitors[result.user_name];
        comp.team_name =
            result.team_name;  // Update team name in case it changed

        if (is_first_time) {
            // First time processing - record performance and calculate points
            models::ContestPerformance perf;
            perf.problems_solved = result.ontime_solved;
            perf.rank = result.rank;
            perf.total_participants = total_participants;
            perf.max_problems_solved = max_ontime_problems;

            if (contest_type == "ontime") {
                perf.points_earned = ScoreCalculator::calculatePoints(
                    result.ontime_solved, result.rank, total_participants,
                    max_ontime_problems, config.contest_problems_weight,
                    config.contest_rank_bonus);
                comp.contests[contest_id] = perf;
                comp.contests_participated++;
            } else {
                perf.points_earned = ScoreCalculator::calculatePoints(
                    result.ontime_solved, result.rank, total_participants,
                    max_ontime_problems, config.homework_problems_weight,
                    config.homework_rank_bonus);
                comp.homeworks[contest_id] = perf;
                comp.homeworks_participated++;
            }

            // Track upsolving for this specific contest
            comp.upsolving_by_contest[contest_id] = result.upsolving_count;
            comp.total_upsolving += result.upsolving_count;

        } else {
            // Update only - recalculate upsolving for THIS contest
            int old_upsolving_for_contest = 0;
            if (comp.upsolving_by_contest.find(contest_id) !=
                comp.upsolving_by_contest.end()) {
                old_upsolving_for_contest =
                    comp.upsolving_by_contest[contest_id];
            }

            // Update the total upsolving count
            comp.total_upsolving = comp.total_upsolving -
                                   old_upsolving_for_contest +
                                   result.upsolving_count;
            comp.upsolving_by_contest[contest_id] = result.upsolving_count;
        }
    }

    processed_contests[contest_id] = pc;

    std::cout << "   Participants: " << total_participants << "\n";
    std::cout << "   Max problems solved: " << max_ontime_problems << "\n";
    std::cout << "[OK] Contest processed successfully!\n\n";

    return true;
}

}  // namespace services
}  // namespace core
}  // namespace MaratonaScore
