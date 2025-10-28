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
 * New hybrid system: Base Points × Compound Multiplier + Rank Bonus
 */
class ScoreCalculator {
   public:
    /**
     * @brief Calculate compound interest multiplier
     *
     * Formula: 2^(contest_index / contests_to_double)
     * Example: index=0, n=11 → 1.0x
     *          index=5, n=11 → 1.43x
     *          index=10, n=11 → 1.95x
     *
     * @param contest_index 0-based chronological index of contest
     * @param contests_to_double Number of contests to double points
     * @return Multiplier for base points
     */
    static double calculateCompoundMultiplier(int contest_index,
                                              int contests_to_double);

    /**
     * @brief Calculate rank bonus with linear decrease
     *
     * Formula: max_bonus - (max_bonus / top_n) × (rank - 1) for rank <= top_n
     *          0 for rank > top_n
     * Example: top_n=10, max=20 → 1st:20, 2nd:18, ..., 10th:2, 11th+:0
     *
     * @param rank Competitor's rank (1-based)
     * @param top_n How many people get bonus
     * @param max_bonus Maximum bonus (1st place gets this)
     * @return Bonus points for this rank
     */
    static double calculateRankBonus(int rank, int top_n, double max_bonus);

    /**
     * @brief Calculate total points for a contest performance (NEW HYBRID
     * SYSTEM)
     *
     * Formula: (problems_solved/max_problems × base_weight × multiplier) +
     * rank_bonus
     *
     * @param problems_solved Number of problems solved by competitor
     * @param rank Competitor's rank in the contest
     * @param max_problems Maximum problems solved by any competitor
     * @param base_weight Base weight for this contest type
     * @param contest_index 0-based chronological index of contest
     * @param contests_to_double Number of contests to double points
     * @param rank_bonus_top_n How many people get rank bonus
     * @param rank_bonus_max Maximum rank bonus
     * @return Total points earned
     */
    static double calculatePointsHybrid(int problems_solved, int rank,
                                        int max_problems, double base_weight,
                                        int contest_index,
                                        int contests_to_double,
                                        int rank_bonus_top_n,
                                        double rank_bonus_max);

    /**
     * @brief Calculate points for a contest performance (LEGACY SYSTEM)
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
