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
 */
struct ScoringConfig {
    // Contest OnTime (Saturday, 5 hours) - MOST IMPORTANT
    double contest_problems_weight = 100.0;  // Max points from solving problems
    double contest_rank_bonus = 50.0;        // Max bonus from good rank

    // Homework (Weekly) - IMPORTANT, but less than contest
    double homework_problems_weight = 50.0;
    double homework_rank_bonus = 25.0;

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
