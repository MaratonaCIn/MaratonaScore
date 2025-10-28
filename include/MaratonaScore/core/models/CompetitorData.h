#ifndef MARATONASCORE_CORE_MODELS_COMPETITOR_DATA_H
#define MARATONASCORE_CORE_MODELS_COMPETITOR_DATA_H

#include <map>
#include <string>

#include "ContestPerformance.h"

namespace MaratonaScore {
namespace core {
namespace models {

/**
 * @brief Status of a competitor in the ranking system
 *
 * - ACTIVE: Normal competitor, included in rankings
 * - BLACKLISTED: Excluded from rankings (monitors, non-competitors, etc.)
 * - GUEST: Participates but not in official ranking (optional visitors)
 */
enum class CompetitorStatus {
    ACTIVE,
    BLACKLISTED,
    GUEST
};

/**
 * @brief Convert CompetitorStatus enum to string
 */
inline std::string competitorStatusToString(CompetitorStatus status) {
    switch (status) {
        case CompetitorStatus::ACTIVE: return "active";
        case CompetitorStatus::BLACKLISTED: return "blacklisted";
        case CompetitorStatus::GUEST: return "guest";
        default: return "unknown";
    }
}

/**
 * @brief Convert string to CompetitorStatus enum
 */
inline CompetitorStatus stringToCompetitorStatus(const std::string& str) {
    if (str == "blacklisted") return CompetitorStatus::BLACKLISTED;
    if (str == "guest") return CompetitorStatus::GUEST;
    return CompetitorStatus::ACTIVE;  // default
}

/**
 * @brief Complete data for a competitor
 *
 * Contains all performance data, scores, and statistics for a single competitor
 * across all contests, homeworks, and upsolving activities.
 */
struct CompetitorData {
    std::string user_name;
    std::string team_name;

    // Competitor status for filtering rankings
    CompetitorStatus status = CompetitorStatus::ACTIVE;
    std::string blacklist_reason;  // Optional reason if blacklisted

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
