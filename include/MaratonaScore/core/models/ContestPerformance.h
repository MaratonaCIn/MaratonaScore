#ifndef MARATONASCORE_CORE_MODELS_CONTEST_PERFORMANCE_H
#define MARATONASCORE_CORE_MODELS_CONTEST_PERFORMANCE_H

namespace MaratonaScore {
namespace core {
namespace models {

/**
 * @brief Performance data for a single contest/homework
 */
struct ContestPerformance {
    int problems_solved = 0;
    int rank = 0;
    int total_participants = 0;
    int max_problems_solved = 0;  // Best performance in this contest
    double points_earned = 0.0;
};

}  // namespace models
}  // namespace core
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CORE_MODELS_CONTEST_PERFORMANCE_H
