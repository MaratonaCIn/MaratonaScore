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

#ifndef MSCR_MODELS_SCOREBOARD_HPP
#define MSCR_MODELS_SCOREBOARD_HPP

#include <map>
#include <ostream>
#include <string>

#include "maratona_score/export.hpp"
#include "maratona_score/models/Contest.hpp"
#include "maratona_score/models/Contestant.hpp"
namespace MaratonaScore {

class MARATONASCORE_API Scoreboard {
   public:
    Scoreboard() = default;
    ~Scoreboard() = default;

    void addContest(const Contest& contest, int index);
    void applyContestFiltering();
    friend std::ostream& operator<<(std::ostream& os, const Scoreboard& sb);

    void renderCSV(std::ostream& os) const;

   protected:
    std::map<std::string, Contestant> contestants;

};  // class Scoreboard

}  // namespace MaratonaScore

#endif  // MSCR_MODELS_SCOREBOARD_HPP