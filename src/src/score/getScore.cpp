#include "score/getScore.hpp"

#include <cmath>

#include "utils/Settings.hpp"

namespace MaratonaScore {

double getSolveScore(CONTEST_TYPE contestType, const Performance& performance,
                     int contestIndex) {
    double problem_value;
    if (contestType == CONTEST) {
        problem_value = Settings::getInstance().CONTEST_BASE_VALUE *
                        pow(2, double(contestIndex) /
                                   Settings::getInstance().NUMBER_OF_CONTESTS);
    } else if (contestType == HOMEWORK) {
        problem_value = Settings::getInstance().HOMEWORK_BASE_VALUE *
                        pow(2, double(contestIndex) /
                                   Settings::getInstance().NUMBER_OF_CONTESTS);
    }

    return performance.getProblemsSolved() * problem_value;
}

double getUpsolveScore(const Performance& performance) {
    return performance.getProblemsUpsolved() *
           Settings::getInstance().UPSOLING_BASE_VALUE;
}

double getRankBonus(CONTEST_TYPE contestType, int rank) {
    if (rank < 1 || rank > Settings::getInstance().CONTEST_PERSON_BONUS) {
        return 0.0;
    }

    double baseBonus;
    int personBonus;

    if (contestType == CONTEST) {
        baseBonus = Settings::getInstance().CONTEST_SCORE_BONUS;
        personBonus = Settings::getInstance().CONTEST_PERSON_BONUS;
    } else if (contestType == HOMEWORK) {
        baseBonus = Settings::getInstance().HOMEWORK_SCORE_BONUS;
        personBonus = Settings::getInstance().HOMEWORK_PERSON_BONUS;
    } else {
        return 0.0;
    }

    double bonus = baseBonus * std::max(0.0, (double)(personBonus - rank + 1) /
                                                 personBonus);

    return bonus;
}

}  // namespace MaratonaScore