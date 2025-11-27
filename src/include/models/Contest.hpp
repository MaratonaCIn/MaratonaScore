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

#ifndef MSCR_MODELS_CONTEST_HPP
#define MSCR_MODELS_CONTEST_HPP

#include <map>
#include <string>

#include "export.hpp"
#include "models/Performance.hpp"

namespace MaratonaScore {

enum CONTEST_TYPE { CONTEST, HOMEWORK };

class MARATONASCORE_API Contest {
   public:
    Contest();
    Contest(CONTEST_TYPE type);
    Contest(const std::string& file, CONTEST_TYPE contestType);
    Contest(const Contest& other) = default;
    ~Contest() = default;

    const std::string& getId() const;
    CONTEST_TYPE getType() const;
    const std::map<std::string, Performance>& getPerformances() const;

    void setId(const std::string& contestId);
    void setType(CONTEST_TYPE contestType);
    void addPerformance(const std::string& teamID, const Performance& perf);

   private:
    std::string id;
    CONTEST_TYPE type;
    std::map<std::string, Performance> performances;
};  // class Contest

}  // namespace MaratonaScore

#endif  // MSCR_MODELS_CONTEST_HPP