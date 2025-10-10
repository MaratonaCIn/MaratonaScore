#ifndef MARATONASCORE_CORE_SERVICES_SCORE_CALCULATOR_H
#define MARATONASCORE_CORE_SERVICES_SCORE_CALCULATOR_H

#include "MaratonaScore/core/models/ScoringConfig.h"

namespace MaratonaScore {
namespace core {
namespace services {

/**
 * @brief Calculates scores based on performance metrics
 *
 * Pure function class for score calculations, independent of data storage.
 */
class ScoreCalculator {
   public:
    /**
     * @brief Calculate points for a contest performance
     *
     * @param problems_solved Number of problems solved by competitor
     * @param rank Competitor's rank in the contest
     * @param total_participants Total number of participants
     * @param max_problems Maximum problems solved by any competitor
     * @param problems_weight Maximum points for solving problems
     * @param rank_bonus Maximum points for good rank
     * @return Total points earned
     */
    static double calculatePoints(int problems_solved, int rank,
                                  int total_participants, int max_problems,
                                  double problems_weight, double rank_bonus);
};

}  // namespace services
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_SERVICES_SCORE_CALCULATOR_H
