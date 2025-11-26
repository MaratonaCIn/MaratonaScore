#ifndef MSCR_MODELS_CONTESTANT_HPP
#define MSCR_MODELS_CONTESTANT_HPP

#include "export.hpp"
#include "models/Performance.hpp"

#include <string>
#include <map>

namespace MaratonaScore{

class MARATONASCORE_API Contestant {
  friend class Scoreboard;
  public:
    void addScoreContest(double s);
    void addScoreHomework(double s);
    void addScoreUpsolved(double s);
    void addScoreBonus(double s);

    double getScoreContest() const;
    double getScoreHomework() const;
    double getScoreUpsolved() const;
    double getScoreBonus() const;
    double getTotalScore() const;

    protected:
      std::map<std::string, Performance*> ContestsPerformance;

      struct ContestScore {
          double solve;
          double bonus;
          double total() const;
      };

      std::map<std::string, ContestScore> contestScores;

  private:
    std::string id;
    std::string name;
    double score = 0.0;
    double scoreContest = 0.0;
    double scoreHomework = 0.0;
    double scoreUpsolved = 0.0;
    double scoreBonus = 0.0;
    double totalProblemsSolved = 0.0;
    double problemsSolved = 0.0;
    double problemsUpsolved = 0.0;

    void fixScore();

  }; // class Contestant

}; // namespace MaratonaScore

#endif  // MSCR_MODELS_CONTESTANT_HPP