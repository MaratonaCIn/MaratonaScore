#ifndef MARATONASCORE_CORE_SERVICES_RATING_SYSTEM_H
#define MARATONASCORE_CORE_SERVICES_RATING_SYSTEM_H

#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"
#include "MaratonaScore/core/models/ProcessedContest.h"
#include "MaratonaScore/core/models/ScoringConfig.h"

namespace MaratonaScore {
namespace core {
namespace services {

/**
 * @brief Main rating system coordinating all operations
 *
 * This class manages competitors, processed contests, and coordinates
 * all scoring and ranking operations.
 */
class RatingSystem {
   public:
    RatingSystem();

    /**
     * @brief Process a contest scoreboard
     */
    bool processScoreboard(const std::string& contest_id,
                           const std::string& filename,
                           const std::string& contest_type,
                           int contest_duration_minutes);

    /**
     * @brief Recalculate all competitor scores
     *
     * Should be called after any changes to competitor data or config.
     */
    void recalculateAllScores();

    /**
     * @brief Reassign contest order indices chronologically
     *
     * Orders contests by first_processed_date and reassigns indices
     * starting from 0. Should be called after loading from database.
     */
    void reassignContestOrderIndices();

    // Getters
    const std::map<std::string, models::CompetitorData>& getCompetitors()
        const {
        return competitors_;
    }
    const std::map<std::string, models::ProcessedContest>&
    getProcessedContests() const {
        return processed_contests_;
    }
    const models::ScoringConfig& getConfig() const { return config_; }

    // Setters
    void setConfig(const models::ScoringConfig& config) { config_ = config; }
    void setCompetitors(
        const std::map<std::string, models::CompetitorData>& competitors) {
        competitors_ = competitors;
    }
    void setProcessedContests(
        const std::map<std::string, models::ProcessedContest>& contests) {
        processed_contests_ = contests;
    }

   private:
    models::ScoringConfig config_;
    std::map<std::string, models::CompetitorData> competitors_;
    std::map<std::string, models::ProcessedContest> processed_contests_;
};

}  // namespace services
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_SERVICES_RATING_SYSTEM_H
