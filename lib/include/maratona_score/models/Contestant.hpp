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

#ifndef MSCR_MODELS_CONTESTANT_HPP
#define MSCR_MODELS_CONTESTANT_HPP

#include <map>
#include <string>

#include "maratona_score/export.hpp"
#include "maratona_score/models/Performance.hpp"

namespace MaratonaScore {

class MARATONASCORE_API Contestant {
    friend class Scoreboard;

   public:
    void addScoreContest(double s);
    void addScoreHomework(double s);
    void addScoreUpsolved(double s);
    void addScoreBonus(double s);

    double getScoreContest() const;
    double getScoreHomework() const;
    double getScoreUpsolved() const;
    double getScoreBonus() const;
    double getTotalScore() const;

   protected:
    std::map<std::string, Performance*> ContestsPerformance;

    struct ContestScore {
        double solve;
        double bonus;
        double total() const;
    };

    std::map<std::string, ContestScore> contestScores;

   private:
    std::string id;
    std::string name;
    double score = 0.0;
    double scoreContest = 0.0;
    double scoreHomework = 0.0;
    double scoreUpsolved = 0.0;
    double scoreBonus = 0.0;
    double totalProblemsSolved = 0.0;
    double problemsSolved = 0.0;
    double problemsUpsolved = 0.0;

    void fixScore();

};  // class Contestant

};  // namespace MaratonaScore

#endif  // MSCR_MODELS_CONTESTANT_HPP