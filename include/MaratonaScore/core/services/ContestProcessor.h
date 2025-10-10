#ifndef MARATONASCORE_CORE_SERVICES_CONTEST_PROCESSOR_H
#define MARATONASCORE_CORE_SERVICES_CONTEST_PROCESSOR_H

#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"
#include "MaratonaScore/core/models/ProcessedContest.h"
#include "MaratonaScore/core/models/ScoringConfig.h"

namespace MaratonaScore {
namespace core {
namespace services {

/**
 * @brief Processes contest scoreboards and updates competitor data
 *
 * Handles parsing JSON scoreboards, calculating scores, and updating
 * competitor and contest records.
 */
class ContestProcessor {
   public:
    /**
     * @brief Process a contest scoreboard
     *
     * @param contest_id Unique identifier for the contest
     * @param filename Path to the scoreboard JSON file
     * @param contest_type "ontime" or "homework"
     * @param contest_duration_minutes Duration of the contest
     * @param config Scoring configuration
     * @param competitors Map of competitors (will be updated)
     * @param processed_contests Map of processed contests (will be updated)
     * @return true if successful, false otherwise
     */
    static bool processScoreboard(
        const std::string& contest_id, const std::string& filename,
        const std::string& contest_type, int contest_duration_minutes,
        const models::ScoringConfig& config,
        std::map<std::string, models::CompetitorData>& competitors,
        std::map<std::string, models::ProcessedContest>& processed_contests);

   private:
    struct TempResult {
        std::string user_name;
        std::string team_name;
        int ontime_solved;
        int total_solved;
        int upsolving_count;
        int rank;
        int penalty;
    };

    static std::string getCurrentDate();
};

}  // namespace services
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_SERVICES_CONTEST_PROCESSOR_H
