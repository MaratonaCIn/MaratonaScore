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

#ifndef MSCR_SETTINGS_HPP
#define MSCR_SETTINGS_HPP

#include <string>

#include "maratona_score/export.hpp"

namespace MaratonaScore {

class MARATONASCORE_API Settings {
   public:
    static Settings& getInstance();

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    void loadFromFile(const std::string& filename);

    // Time limits
    int CONTEST_TIME_LIMIT;
    int HOMEWORK_TIME_LIMIT;

    // Base values
    int CONTEST_BASE_VALUE;
    int HOMEWORK_BASE_VALUE;
    int UPSOLING_BASE_VALUE;

    // Score bonuses
    int CONTEST_SCORE_BONUS;
    int HOMEWORK_SCORE_BONUS;

    // Person bonuses
    int CONTEST_PERSON_BONUS;
    int HOMEWORK_PERSON_BONUS;

    // Contest settings
    int NUMBER_OF_CONTESTS;
    int IGNORE_WORST_CONTESTS;

   private:
    Settings();
    void setDefaultValues();
};

}  // namespace MaratonaScore

#endif  // MSCR_SETTINGS_HPP