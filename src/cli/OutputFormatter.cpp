#include "MaratonaScore/cli/OutputFormatter.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

namespace MaratonaScore {
namespace cli {

void OutputFormatter::printRankings(
    const std::map<std::string, core::models::CompetitorData>& competitors) {
    std::vector<core::models::CompetitorData> ranking;
    for (const auto& [user, comp] : competitors) {
        ranking.push_back(comp);
    }

    // Sort by final score
    std::sort(ranking.begin(), ranking.end(),
              [](const core::models::CompetitorData& a,
                 const core::models::CompetitorData& b) {
                  return a.final_score > b.final_score;
              });

    std::cout << "\n" << std::string(78, '=') << "\n";
    std::cout << "                    FINAL RANKING - TOTAL POINTS             "
                 "             \n";
    std::cout << std::string(78, '=') << "\n\n";

    std::cout << std::setw(4) << "Rank" << " " << std::setw(20) << std::left
              << "Team" << " " << std::setw(20) << std::left << "User" << " "
              << std::setw(8) << std::right << "Total" << " " << std::setw(8)
              << "Contest" << " " << std::setw(8) << "Homework" << " "
              << std::setw(8) << "Upsolve" << "\n";
    std::cout << std::string(90, '-') << "\n";

    for (size_t i = 0; i < ranking.size(); i++) {
        std::cout << std::setw(4) << (i + 1) << " " << std::setw(20)
                  << std::left << ranking[i].team_name.substr(0, 19) << " "
                  << std::setw(20) << std::left
                  << ranking[i].user_name.substr(0, 19) << " " << std::setw(8)
                  << std::right << std::fixed << std::setprecision(1)
                  << ranking[i].final_score << " " << std::setw(8)
                  << ranking[i].total_contest_points << " " << std::setw(8)
                  << ranking[i].total_homework_points << " " << std::setw(8)
                  << ranking[i].total_upsolving_points << "\n";
    }

    // Ranking by Contest Performance (Relative - based on rank bonus)
    std::cout << "\n" << std::string(78, '=') << "\n";
    std::cout << "              CONTEST PERFORMANCE - RELATIVE (Rank-based)    "
                 "             \n";
    std::cout << std::string(78, '=') << "\n\n";

    std::sort(ranking.begin(), ranking.end(),
              [](const core::models::CompetitorData& a,
                 const core::models::CompetitorData& b) {
                  return a.total_contest_points > b.total_contest_points;
              });

    for (size_t i = 0; i < ranking.size(); i++) {
        std::cout << std::setw(4) << (i + 1) << " " << std::setw(20)
                  << std::left << ranking[i].team_name.substr(0, 19) << " "
                  << std::setw(20) << std::left
                  << ranking[i].user_name.substr(0, 19) << " " << std::setw(8)
                  << std::right << std::fixed << std::setprecision(1)
                  << ranking[i].total_contest_points << " ("
                  << ranking[i].contests_participated << " contests)\n";
    }

    // Ranking by Absolute Performance (problems solved)
    std::cout << "\n" << std::string(78, '=') << "\n";
    std::cout << "              CONTEST PERFORMANCE - ABSOLUTE "
                 "(Problems-based)             \n";
    std::cout << std::string(78, '=') << "\n\n";

    // Calculate total problems solved in contests
    std::vector<std::pair<int, core::models::CompetitorData>> absolute_ranking;
    for (const auto& comp : ranking) {
        int total_problems = 0;
        for (const auto& [contest_id, perf] : comp.contests) {
            total_problems += perf.problems_solved;
        }
        absolute_ranking.push_back({total_problems, comp});
    }

    std::sort(absolute_ranking.begin(), absolute_ranking.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    for (size_t i = 0; i < absolute_ranking.size(); i++) {
        std::cout << std::setw(4) << (i + 1) << " " << std::setw(20)
                  << std::left
                  << absolute_ranking[i].second.team_name.substr(0, 19) << " "
                  << std::setw(20) << std::left
                  << absolute_ranking[i].second.user_name.substr(0, 19) << " "
                  << std::setw(8) << std::right << absolute_ranking[i].first
                  << " problems"
                  << " (" << absolute_ranking[i].second.contests_participated
                  << " contests)\n";
    }
}

void OutputFormatter::printConfig(const core::models::ScoringConfig& config) {
    std::cout << "\n" << std::string(78, '=') << "\n";
    std::cout << "                        SCORING CONFIGURATION                "
                 "             \n";
    std::cout << std::string(78, '=') << "\n\n";

    std::cout << "OnTime Contest (Saturday, 5h):\n";
    std::cout << "  - Problems weight: " << config.contest_problems_weight
              << " points\n";
    std::cout << "  - Contests to double: " << config.contests_to_double << "\n";
    std::cout << "  - Rank bonus (top " << config.rank_bonus_top_n << "): max "
              << config.rank_bonus_max << " points\n";
    std::cout << "  - Maximum (1st place, contest 1): "
              << (config.contest_problems_weight + config.rank_bonus_max)
              << " points\n\n";

    std::cout << "Homework (Weekly):\n";
    std::cout << "  - Problems weight: " << config.homework_problems_weight
              << " points\n";
    std::cout << "  - Contests to double: " << config.homework_contests_to_double << "\n";
    std::cout << "  - Rank bonus (top " << config.homework_rank_bonus_top_n << "): max "
              << config.homework_rank_bonus_max << " points\n";
    std::cout << "  - Maximum (1st place, homework 1): "
              << (config.homework_problems_weight + config.homework_rank_bonus_max)
              << " points\n\n";

    std::cout << "Upsolving:\n";
    std::cout << "  - Points per problem: "
              << config.upsolving_points_per_problem << " points\n\n";
}

void OutputFormatter::printUsage(const std::string& program_name) {
    std::cout << std::string(78, '=') << "\n";
    std::cout << "              MaratonaScore Rating System v2 - Point-Based   "
                 "               \n";
    std::cout << std::string(78, '=') << "\n\n";
    std::cout << "Usage:\n";
    std::cout << "  " << program_name << " <command> [arguments]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  process <contest_id> <scoreboard.json> <type> <duration>\n";
    std::cout
        << "      Process a contest (auto-detects first time vs update)\n";
    std::cout << "      contest_id: unique identifier (e.g., week1_contest, "
                 "week2_homework)\n";
    std::cout << "      type: ontime or homework\n";
    std::cout << "      duration: contest duration in minutes\n\n";
    std::cout << "  batch <config_file>\n";
    std::cout << "      Process multiple contests from config file\n\n";
    std::cout << "  convert <scoreboard.xlsx> <output.json>\n";
    std::cout << "      Convert single vJudge Excel scoreboard to JSON\n\n";
    std::cout << "  convert-batch <batch_file>\n";
    std::cout << "      Convert multiple Excel files (format: <excel> <json> "
                 "per line)\n\n";
    std::cout << "  rankings\n";
    std::cout << "      Display all rankings\n\n";
    std::cout << "  config\n";
    std::cout << "      Show current scoring configuration\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name
              << " convert scoreboard.xlsx scoreboard.json\n";
    std::cout << "  " << program_name << " convert-batch conversions.txt\n";
    std::cout << "  " << program_name
              << " process week1_contest scoreboard.json ontime 300\n";
    std::cout << "  " << program_name
              << " process week1_homework homework.json homework 10080\n";
    std::cout << "  " << program_name << " batch contests.txt\n";
    std::cout << "  " << program_name << " rankings\n";
}

}  // namespace cli
}  // namespace MaratonaScore
