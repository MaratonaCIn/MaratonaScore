#ifndef MARATONASCORE_CORE_MODELS_SCORING_CONFIG_H
#define MARATONASCORE_CORE_MODELS_SCORING_CONFIG_H

namespace MaratonaScore {
namespace core {
namespace models {

/**
 * @brief Configuration for the scoring system
 *
 * This struct contains all the weights and bonuses used to calculate
 * competitor scores across different contest types and activities.
 *
 * New hybrid system: Base points × Compound Interest Multiplier + Rank Bonus
 * - Compound Interest: Points grow exponentially over contest sequence
 * - Rank Bonus: Top N competitors get linearly decreasing bonus
 */
struct ScoringConfig {
    // Contest OnTime (Saturday, 5 hours) - MOST IMPORTANT
    double contest_problems_weight = 100.0;  // Base weight for problems solved

    // Compound Interest Parameters
    // Formula: multiplier = 2^(contest_index / contests_to_double)
    // Default: 11 contests to double points (contest 11 = 2× contest 1)
    int contests_to_double = 11;

    // Rank Bonus Parameters
    // Top N competitors get bonus: max_bonus - (max_bonus/top_n) × (rank-1)
    // Example: top_n=10, max=20 → 1st:20, 2nd:18, ..., 10th:2, 11th+:0
    int rank_bonus_top_n = 10;       // How many people get bonus
    double rank_bonus_max = 20.0;    // Maximum bonus (1st place)

    // Homework (Weekly) - IMPORTANT, but less than contest
    double homework_problems_weight = 50.0;
    int homework_contests_to_double = 11;
    int homework_rank_bonus_top_n = 5;
    double homework_rank_bonus_max = 10.0;

    // Upsolving - BONUS points, fixed value per problem
    double upsolving_points_per_problem = 5.0;

    // Minimum participation (0 = no minimum)
    int min_contests_required = 0;
    int min_homeworks_required = 0;
};

}  // namespace models
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_MODELS_SCORING_CONFIG_H
