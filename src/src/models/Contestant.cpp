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
