#ifndef MARATONASCORE_CORE_PERSISTENCE_DATABASE_MANAGER_H
#define MARATONASCORE_CORE_PERSISTENCE_DATABASE_MANAGER_H

#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"
#include "MaratonaScore/core/models/ProcessedContest.h"
#include "MaratonaScore/core/models/ScoringConfig.h"

namespace MaratonaScore {
namespace core {
namespace persistence {

/**
 * @brief Handles loading and saving data to JSON files
 *
 * Responsible for serialization/deserialization of all data structures.
 */
class DatabaseManager {
   public:
    /**
     * @brief Load database from JSON file
     *
     * @param filename Path to the database file
     * @param config Output parameter for scoring config
     * @param competitors Output parameter for competitors map
     * @param processed_contests Output parameter for processed contests map
     * @return true if successful, false otherwise
     */
    static bool loadDatabase(
        const std::string& filename, models::ScoringConfig& config,
        std::map<std::string, models::CompetitorData>& competitors,
        std::map<std::string, models::ProcessedContest>& processed_contests);

    /**
     * @brief Save database to JSON file
     *
     * @param filename Path to the database file
     * @param config Scoring configuration to save
     * @param competitors Competitors map to save
     * @param processed_contests Processed contests map to save
     * @return true if successful, false otherwise
     */
    static bool saveDatabase(
        const std::string& filename, const models::ScoringConfig& config,
        const std::map<std::string, models::CompetitorData>& competitors,
        const std::map<std::string, models::ProcessedContest>&
            processed_contests);
};

}  // namespace persistence
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_PERSISTENCE_DATABASE_MANAGER_H
