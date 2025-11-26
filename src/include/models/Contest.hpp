#ifndef MSCR_MODELS_CONTEST_HPP
#define MSCR_MODELS_CONTEST_HPP

#include "export.hpp"
#include <map>
#include <string>

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