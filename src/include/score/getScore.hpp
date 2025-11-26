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