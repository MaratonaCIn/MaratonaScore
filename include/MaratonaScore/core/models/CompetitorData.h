#ifndef MARATONASCORE_CORE_MODELS_COMPETITOR_DATA_H
#define MARATONASCORE_CORE_MODELS_COMPETITOR_DATA_H

#include <map>
#include <string>

#include "ContestPerformance.h"

namespace MaratonaScore {
namespace core {
namespace models {

/**
 * @brief Complete data for a competitor
 *
 * Contains all performance data, scores, and statistics for a single competitor
 * across all contests, homeworks, and upsolving activities.
 */
struct CompetitorData {
    std::string user_name;
    std::string team_name;

    // Contest performances
    std::map<std::string, ContestPerformance> contests;
    std::map<std::string, ContestPerformance> homeworks;

    // Upsolving tracking (map of contest_id -> upsolving count for that
    // contest)
    std::map<std::string, int> upsolving_by_contest;
    int total_upsolving = 0;

    // Calculated scores
    double total_contest_points = 0.0;
    double total_homework_points = 0.0;
    double total_upsolving_points = 0.0;
    double final_score = 0.0;

    // Stats
    int contests_participated = 0;
    int homeworks_participated = 0;
};

}  // namespace models
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_MODELS_COMPETITOR_DATA_H
