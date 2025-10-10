#include "MaratonaScore/core/persistence/DatabaseManager.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace MaratonaScore {
namespace core {
namespace persistence {

bool DatabaseManager::loadDatabase(
    const std::string& filename, models::ScoringConfig& config,
    std::map<std::string, models::CompetitorData>& competitors,
    std::map<std::string, models::ProcessedContest>& processed_contests) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[WARNING] Database file not found. Starting with empty "
                     "database.\n";
        return false;
    }

    json db;
    try {
        file >> db;
    } catch (const json::parse_error& e) {
        std::cerr << "[ERROR] Database JSON parse error: " << e.what() << "\n";
        file.close();
        return false;
    }
    file.close();

    // Load config if exists
    if (db.contains("config")) {
        config.contest_problems_weight =
            db["config"]["contest_problems_weight"];
        config.contest_rank_bonus = db["config"]["contest_rank_bonus"];
        config.homework_problems_weight =
            db["config"]["homework_problems_weight"];
        config.homework_rank_bonus = db["config"]["homework_rank_bonus"];
        config.upsolving_points_per_problem =
            db["config"]["upsolving_points_per_problem"];
    }

    // Load competitors
    if (db.contains("competitors")) {
        for (const auto& [user, data] : db["competitors"].items()) {
            models::CompetitorData comp;
            comp.user_name = user;
            comp.team_name = data["team_name"];
            comp.total_upsolving = data["total_upsolving"];
            comp.contests_participated = data["contests_participated"];
            comp.homeworks_participated = data["homeworks_participated"];

            // Load upsolving by contest
            if (data.contains("upsolving_by_contest")) {
                for (const auto& [contest_id, count] :
                     data["upsolving_by_contest"].items()) {
                    comp.upsolving_by_contest[contest_id] = count;
                }
            }

            // Load contest performances
            if (data.contains("contests")) {
                for (const auto& [contest_id, perf_data] :
                     data["contests"].items()) {
                    models::ContestPerformance perf;
                    perf.problems_solved = perf_data["problems_solved"];
                    perf.rank = perf_data["rank"];
                    perf.total_participants = perf_data["total_participants"];
                    perf.max_problems_solved = perf_data["max_problems_solved"];
                    perf.points_earned = perf_data["points_earned"];
                    comp.contests[contest_id] = perf;
                }
            }

            // Load homework performances
            if (data.contains("homeworks")) {
                for (const auto& [hw_id, perf_data] :
                     data["homeworks"].items()) {
                    models::ContestPerformance perf;
                    perf.problems_solved = perf_data["problems_solved"];
                    perf.rank = perf_data["rank"];
                    perf.total_participants = perf_data["total_participants"];
                    perf.max_problems_solved = perf_data["max_problems_solved"];
                    perf.points_earned = perf_data["points_earned"];
                    comp.homeworks[hw_id] = perf;
                }
            }

            competitors[user] = comp;
        }
    }

    // Load processed contests
    if (db.contains("processed_contests")) {
        for (const auto& [contest_id, contest_data] :
             db["processed_contests"].items()) {
            models::ProcessedContest pc;
            pc.contest_id = contest_id;
            pc.type = contest_data["type"];
            pc.duration_minutes = contest_data["duration_minutes"];
            pc.first_processed_date = contest_data["first_processed_date"];
            pc.last_updated_date = contest_data["last_updated_date"];
            pc.total_participants = contest_data["total_participants"];
            pc.max_problems_solved = contest_data["max_problems_solved"];

            for (const auto& p : contest_data["participants"]) {
                pc.participants.insert(p);
            }

            processed_contests[contest_id] = pc;
        }
    }

    std::cout << "[OK] Loaded " << competitors.size() << " competitors and "
              << processed_contests.size() << " contests from database.\n";
    return true;
}

bool DatabaseManager::saveDatabase(
    const std::string& filename, const models::ScoringConfig& config,
    const std::map<std::string, models::CompetitorData>& competitors,
    const std::map<std::string, models::ProcessedContest>& processed_contests) {
    json db;

    // Save config
    db["config"]["contest_problems_weight"] = config.contest_problems_weight;
    db["config"]["contest_rank_bonus"] = config.contest_rank_bonus;
    db["config"]["homework_problems_weight"] = config.homework_problems_weight;
    db["config"]["homework_rank_bonus"] = config.homework_rank_bonus;
    db["config"]["upsolving_points_per_problem"] =
        config.upsolving_points_per_problem;

    // Save competitors
    for (const auto& [user, comp] : competitors) {
        json comp_data;
        comp_data["team_name"] = comp.team_name;
        comp_data["total_upsolving"] = comp.total_upsolving;
        comp_data["contests_participated"] = comp.contests_participated;
        comp_data["homeworks_participated"] = comp.homeworks_participated;

        // Save upsolving by contest
        for (const auto& [contest_id, count] : comp.upsolving_by_contest) {
            comp_data["upsolving_by_contest"][contest_id] = count;
        }

        // Save contest performances
        for (const auto& [contest_id, perf] : comp.contests) {
            comp_data["contests"][contest_id]["problems_solved"] =
                perf.problems_solved;
            comp_data["contests"][contest_id]["rank"] = perf.rank;
            comp_data["contests"][contest_id]["total_participants"] =
                perf.total_participants;
            comp_data["contests"][contest_id]["max_problems_solved"] =
                perf.max_problems_solved;
            comp_data["contests"][contest_id]["points_earned"] =
                perf.points_earned;
        }

        // Save homework performances
        for (const auto& [hw_id, perf] : comp.homeworks) {
            comp_data["homeworks"][hw_id]["problems_solved"] =
                perf.problems_solved;
            comp_data["homeworks"][hw_id]["rank"] = perf.rank;
            comp_data["homeworks"][hw_id]["total_participants"] =
                perf.total_participants;
            comp_data["homeworks"][hw_id]["max_problems_solved"] =
                perf.max_problems_solved;
            comp_data["homeworks"][hw_id]["points_earned"] = perf.points_earned;
        }

        db["competitors"][user] = comp_data;
    }

    // Save processed contests
    for (const auto& [contest_id, pc] : processed_contests) {
        db["processed_contests"][contest_id]["type"] = pc.type;
        db["processed_contests"][contest_id]["duration_minutes"] =
            pc.duration_minutes;
        db["processed_contests"][contest_id]["first_processed_date"] =
            pc.first_processed_date;
        db["processed_contests"][contest_id]["last_updated_date"] =
            pc.last_updated_date;
        db["processed_contests"][contest_id]["total_participants"] =
            pc.total_participants;
        db["processed_contests"][contest_id]["max_problems_solved"] =
            pc.max_problems_solved;
        db["processed_contests"][contest_id]["participants"] = json::array();
        for (const auto& p : pc.participants) {
            db["processed_contests"][contest_id]["participants"].push_back(p);
        }
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open database file for writing: "
                  << filename << "\n";
        return false;
    }

    file << db.dump(2);
    file.close();

    std::cout << "[OK] Saved database with " << competitors.size()
              << " competitors.\n";
    return true;
}

}  // namespace persistence
}  // namespace core
}  // namespace MaratonaScore
