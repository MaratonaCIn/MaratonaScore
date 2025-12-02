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


#ifndef MSCR_PARSER_SCOREBOARDPARSER_HPP
#define MSCR_PARSER_SCOREBOARDPARSER_HPP

#include <string>

#include "maratona_score/export.hpp"
#include "maratona_score/models/Contest.hpp"
#include "maratona_score/utils/Settings.hpp"

namespace MaratonaScore {

class MARATONASCORE_API ScoreboardParser {
   public:
    Contest parse(const std::string& file_path, CONTEST_TYPE contestType);
};

}  // namespace MaratonaScore

#endif  // MSCR_PARSER_SCOREBOARDPARSER_HPP