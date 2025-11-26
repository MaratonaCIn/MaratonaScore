#ifndef MSCR_UTILS_BLACKLIST_HPP
#define MSCR_UTILS_BLACKLIST_HPP

#include "export.hpp"
#include <string>
#include <set>

namespace MaratonaScore {

class MARATONASCORE_API Blacklist {
public:
    static void loadFromFile(const std::string& filepath);
    static bool isBlacklisted(const std::string& teamID);
    static const std::set<std::string>& getBlacklistedTeams();
    static void clear();

private:
    static std::set<std::string> blacklistedTeams;
};

} // namespace MaratonaScore

#endif // MSCR_UTILS_BLACKLIST_HPP
