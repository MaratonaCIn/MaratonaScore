#include "models/Contest.hpp"
#include "parser/ScoreboardParser.hpp"

namespace MaratonaScore {

Contest::Contest() : id(""), type(CONTEST) {}

Contest::Contest(CONTEST_TYPE type) : type(type) {}

Contest::Contest(const std::string& file, CONTEST_TYPE contestType) {
    *this = ScoreboardParser().parse(file, contestType);
}

const std::string& Contest::getId() const {
    return id;
}

CONTEST_TYPE Contest::getType() const {
    return type;
}

const std::map<std::string, Performance>& Contest::getPerformances() const {
    return performances;
}

void Contest::setId(const std::string& contestId) {
    id = contestId;
}

void Contest::setType(CONTEST_TYPE contestType) {
    type = contestType;
}

void Contest::addPerformance(const std::string& teamID, const Performance& perf) {
    performances[teamID] = perf;
}

} // namespace MaratonaScore
