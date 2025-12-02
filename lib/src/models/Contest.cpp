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

#include "models/Contest.hpp"

#include "parser/ScoreboardParser.hpp"

namespace MaratonaScore {

Contest::Contest() : id(""), type(CONTEST) {}

Contest::Contest(CONTEST_TYPE type) : type(type) {}

Contest::Contest(const std::string& file, CONTEST_TYPE contestType) {
    *this = ScoreboardParser().parse(file, contestType);
}

const std::string& Contest::getId() const { return id; }

CONTEST_TYPE Contest::getType() const { return type; }

const std::map<std::string, Performance>& Contest::getPerformances() const {
    return performances;
}

void Contest::setId(const std::string& contestId) { id = contestId; }

void Contest::setType(CONTEST_TYPE contestType) { type = contestType; }

void Contest::addPerformance(const std::string& teamID,
                             const Performance& perf) {
    performances[teamID] = perf;
}

}  // namespace MaratonaScore
