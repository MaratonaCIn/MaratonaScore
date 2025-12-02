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

#include "models/Contestant.hpp"

namespace MaratonaScore {

void Contestant::addScoreContest(double s) {
    scoreContest += s;
    fixScore();
}

void Contestant::addScoreHomework(double s) {
    scoreHomework += s;
    fixScore();
}

void Contestant::addScoreUpsolved(double s) {
    scoreUpsolved += s;
    fixScore();
}

void Contestant::addScoreBonus(double s) {
    scoreBonus += s;
    fixScore();
}

double Contestant::getScoreContest() const {
    return scoreContest;
}

double Contestant::getScoreHomework() const {
    return scoreHomework;
}

double Contestant::getScoreUpsolved() const {
    return scoreUpsolved;
}

double Contestant::getScoreBonus() const {
    return scoreBonus;
}

double Contestant::getTotalScore() const {
    return score;
}

void Contestant::fixScore() {
    score = scoreContest + scoreHomework + scoreUpsolved + scoreBonus;
}

double Contestant::ContestScore::total() const {
    return solve + bonus;
}

} // namespace MaratonaScore
