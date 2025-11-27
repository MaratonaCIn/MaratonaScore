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


#ifndef MSCR_SCORE_GETSCORE_HPP
#define MSCR_SCORE_GETSCORE_HPP

#include "export.hpp"
#include "models/Performance.hpp"
#include "models/Contest.hpp"

namespace MaratonaScore {
    MARATONASCORE_API double getSolveScore(CONTEST_TYPE contestType, const Performance& performance, int contestIndex);
    MARATONASCORE_API double getUpsolveScore(const Performance& performance);
    MARATONASCORE_API double getRankBonus(CONTEST_TYPE contestType, int rank);

} // namespace MaratonaScore
#endif // MSCR_SCORE_GETSCORE_HPP