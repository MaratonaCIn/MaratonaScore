#ifndef MARATONASCORE_CORE_MODELS_PROCESSED_CONTEST_H
#define MARATONASCORE_CORE_MODELS_PROCESSED_CONTEST_H

#include <set>
#include <string>

namespace MaratonaScore {
namespace core {
namespace models {

/**
 * @brief Metadata about a processed contest
 *
 * Tracks when a contest was processed and key statistics about it.
 */
struct ProcessedContest {
    std::string contest_id;
    std::string type;  // "ontime" or "homework"
    int duration_minutes = 0;
    std::string first_processed_date;
    std::string last_updated_date;
    std::set<std::string> participants;
    int total_participants = 0;
    int max_problems_solved = 0;
};

}  // namespace models
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_MODELS_PROCESSED_CONTEST_H
