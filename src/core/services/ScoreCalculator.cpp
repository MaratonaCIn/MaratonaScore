#include "MaratonaScore/core/services/ScoreCalculator.h"

#include <cmath>

namespace MaratonaScore {
namespace core {
namespace services {

double ScoreCalculator::calculateCompoundMultiplier(int contest_index,
                                                    int contests_to_double) {
    if (contests_to_double <= 0) {
        return 1.0;  // Avoid division by zero, no growth
    }

    // Formula: 2^(contest_index / contests_to_double)
    // Example: index=0, n=11 → 2^(0/11) = 2^0 = 1.0
    //          index=5, n=11 → 2^(5/11) = 1.43
    //          index=10, n=11 → 2^(10/11) = 1.95
    double exponent = static_cast<double>(contest_index) / contests_to_double;
    return std::pow(2.0, exponent);
}

double ScoreCalculator::calculateRankBonus(int rank, int top_n,
                                           double max_bonus) {
    // No bonus if rank is outside top N
    if (rank > top_n || top_n <= 0) {
        return 0.0;
    }

    // Linear decrease from max_bonus to 0
    // Formula: max_bonus - (max_bonus / top_n) × (rank - 1)
    // Example: top_n=10, max=20
    //   rank 1:  20 - (20/10) × 0  = 20
    //   rank 2:  20 - (20/10) × 1  = 18
    //   rank 5:  20 - (20/10) × 4  = 12
    //   rank 10: 20 - (20/10) × 9  = 2
    //   rank 11: 0 (outside top_n)
    double decrement = max_bonus / top_n;
    return max_bonus - decrement * (rank - 1);
}

double ScoreCalculator::calculatePointsHybrid(
    int problems_solved, int rank, int max_problems, double base_weight,
    int contest_index, int contests_to_double, int rank_bonus_top_n,
    double rank_bonus_max) {
    // Base points from problems solved
    double base_points = 0.0;
    if (max_problems > 0) {
        base_points = base_weight *
                      (static_cast<double>(problems_solved) / max_problems);
    }

    // Apply compound interest multiplier
    double multiplier =
        calculateCompoundMultiplier(contest_index, contests_to_double);
    double problems_score = base_points * multiplier;

    // Add rank bonus
    double rank_score = calculateRankBonus(rank, rank_bonus_top_n, rank_bonus_max);

    return problems_score + rank_score;
}

double ScoreCalculator::calculatePoints(int problems_solved, int rank,
                                        int total_participants,
                                        int max_problems,
                                        double problems_weight,
                                        double rank_bonus) {
    // LEGACY SYSTEM: kept for backward compatibility
    // Performance based on problems solved (0 to problems_weight)
    double problems_score = 0.0;
    if (max_problems > 0) {
        problems_score = problems_weight *
                         (static_cast<double>(problems_solved) / max_problems);
    }

    // Bonus based on rank (0 to rank_bonus)
    double rank_score = 0.0;
    if (total_participants > 1) {
        rank_score = rank_bonus * (1.0 - static_cast<double>(rank - 1) /
                                             (total_participants - 1));
    }

    return problems_score + rank_score;
}

}  // namespace services
}  // namespace core
}  // namespace MaratonaScore
