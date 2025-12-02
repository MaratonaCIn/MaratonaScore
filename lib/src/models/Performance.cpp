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

#include "models/Performance.hpp"

namespace MaratonaScore {

// ProblemStatus implementations
ProblemStatus::ProblemStatus() : problem_status(NOT_ATTEMPTED), timeTaken(0), attempts(0) {}

ProblemStatus::ProblemStatus(PROBLEM_STATUS status, int time, int att)
    : problem_status(status), timeTaken(time), attempts(att) {}

PROBLEM_STATUS ProblemStatus::getStatus() const {
    return problem_status;
}

int ProblemStatus::getTimeTaken() const {
    return timeTaken;
}

int ProblemStatus::getAttempts() const {
    return attempts;
}

void ProblemStatus::setStatus(PROBLEM_STATUS status) {
    problem_status = status;
}

void ProblemStatus::setTimeTaken(int time) {
    timeTaken = time;
}

void ProblemStatus::setAttempts(int att) {
    attempts = att;
}

// Performance implementations
Performance::Performance()
    : rank(0), penalty(0), problems_solved(0), problems_attempted(0), problems_upsolved(0), bonus_score(0.0) {}

Performance::Performance(int r, int p)
    : rank(r), penalty(p), problems_solved(0), problems_attempted(0), problems_upsolved(0), bonus_score(0.0) {}

int Performance::getRank() const {
    return rank;
}

int Performance::getPenalty() const {
    return penalty;
}

int Performance::getProblemsSolved() const {
    return problems_solved;
}

int Performance::getProblemsAttempted() const {
    return problems_attempted;
}

int Performance::getProblemsUpsolved() const {
    return problems_upsolved;
}

double Performance::getBonusScore() const {
    return bonus_score;
}

const std::map<std::string, ProblemStatus>& Performance::getProblems() const {
    return problems;
}

void Performance::setRank(int r) {
    rank = r;
}

void Performance::setPenalty(int p) {
    penalty = p;
}

void Performance::setBonusScore(double bonus) {
    bonus_score = bonus;
}

void Performance::setProblemsUpsolved(int ups) {
    problems_upsolved = ups;
}

void Performance::addProblem(const std::string& problemId, const ProblemStatus& status) {
    problems[problemId] = status;

    if (status.getStatus() == SOLVED) {
        problems_solved++;
    }
    else if (status.getStatus() == UPSOLVED) {
        problems_upsolved++;
    }
    else if (status.getStatus() == ATTEMPTED) {
        problems_attempted++;
    }
}

bool Performance::operator<(const Performance& other) const {
    if (problems_solved != other.problems_solved) {
        return problems_solved > other.problems_solved;
    }
    return penalty < other.penalty;
}

} // namespace MaratonaScore