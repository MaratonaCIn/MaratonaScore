#ifndef MSCR_SETTINGS_HPP
#define MSCR_SETTINGS_HPP

#include "export.hpp"

#include <string>

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

} // namespace MaratonaScore

#endif  // MSCR_SETTINGS_HPP