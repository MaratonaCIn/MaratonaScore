#include "MaratonaScore/core/services/RatingSystem.h"

#include "MaratonaScore/core/services/ContestProcessor.h"

namespace MaratonaScore {
namespace core {
namespace services {

RatingSystem::RatingSystem() {
    // Initialize with default config
}

bool RatingSystem::processScoreboard(const std::string& contest_id,
                                     const std::string& filename,
                                     const std::string& contest_type,
                                     int contest_duration_minutes) {
    // Determine contest order index based on how many contests of this type
    // have been processed
    int contest_order_index = 0;
    for (const auto& [id, pc] : processed_contests_) {
        if (pc.type == contest_type) {
            contest_order_index++;
        }
    }

    bool success = ContestProcessor::processScoreboard(
        contest_id, filename, contest_type, contest_duration_minutes,
        contest_order_index, config_, competitors_, processed_contests_, true);

    if (success) {
        recalculateAllScores();
    }

    return success;
}

void RatingSystem::recalculateAllScores() {
    for (auto& [user, comp] : competitors_) {
        // Calculate contest points
        comp.total_contest_points = 0.0;
        for (const auto& [contest_id, perf] : comp.contests) {
            comp.total_contest_points += perf.points_earned;
        }

        // Calculate homework points
        comp.total_homework_points = 0.0;
        for (const auto& [hw_id, perf] : comp.homeworks) {
            comp.total_homework_points += perf.points_earned;
        }

        // Calculate upsolving points
        comp.total_upsolving_points =
            comp.total_upsolving * config_.upsolving_points_per_problem;

        // Final score
        comp.final_score = comp.total_contest_points +
                           comp.total_homework_points +
                           comp.total_upsolving_points;
    }
}

}  // namespace services
}  // namespace core
}  // namespace MaratonaScore
