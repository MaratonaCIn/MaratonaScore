#ifndef MSCR_MODELS_PERFORMANCE_HPP
#define MSCR_MODELS_PERFORMANCE_HPP

#include "export.hpp"
#include <string>
#include <map>

namespace MaratonaScore {

enum PROBLEM_STATUS {
    NOT_ATTEMPTED,
    ATTEMPTED,
    SOLVED,
    UPSOLVED
};

class MARATONASCORE_API ProblemStatus {
  public:
    ProblemStatus();
    ProblemStatus(PROBLEM_STATUS status, int time, int att);
    ~ProblemStatus() = default;

    PROBLEM_STATUS getStatus() const;
    int getTimeTaken() const;
    int getAttempts() const;

    void setStatus(PROBLEM_STATUS status);
    void setTimeTaken(int time);
    void setAttempts(int att);

  private:
    PROBLEM_STATUS problem_status;
    int timeTaken;
    int attempts;

}; // class ProblemStatus

class MARATONASCORE_API Performance {
  public:
    Performance();
    Performance(int r, int p);
    ~Performance() = default;

    int getRank() const;
    int getPenalty() const;
    int getProblemsSolved() const;
    int getProblemsAttempted() const;
    int getProblemsUpsolved() const;
    double getBonusScore() const;
    const std::map<std::string, ProblemStatus>& getProblems() const;

    void setRank(int r);
    void setPenalty(int p);
    void setBonusScore(double bonus);
    void addProblem(const std::string& problemId, const ProblemStatus& status);
    void setProblemsUpsolved(int ups);

    const bool operator < (const Performance& other) const;

  private:
    int rank;
    int penalty;
    int problems_solved;
    int problems_attempted;
    int problems_upsolved;
    double bonus_score;

    std::map<std::string, ProblemStatus> problems;
}; // class Performance

} // namespace MaratonaScore

#endif  // MSCR_MODELS_PERFORMANCE_HPP