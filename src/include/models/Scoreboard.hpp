#ifndef MSCR_MODELS_SCOREBOARD_HPP
#define MSCR_MODELS_SCOREBOARD_HPP

#include "export.hpp"
#include "models/Contestant.hpp"
#include "models/Contest.hpp"

#include <map>
#include <string>
#include <ostream>

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

}; // class Scoreboard


} // namespace MaratonaScore

#endif  // MSCR_MODELS_SCOREBOARD_HPP