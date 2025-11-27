//    Copyright 2025 MaratonaCIn
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.


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