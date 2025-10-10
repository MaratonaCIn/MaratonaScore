#include "MaratonaScore/core/services/ScoreCalculator.h"

namespace MaratonaScore {
namespace core {
namespace services {

double ScoreCalculator::calculatePoints(int problems_solved, int rank,
                                        int total_participants,
                                        int max_problems,
                                        double problems_weight,
                                        double rank_bonus) {
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
