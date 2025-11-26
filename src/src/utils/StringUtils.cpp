#include "utils/StringUtils.hpp"

namespace MaratonaScore {

int timeStringToMinutes(const std::string& timeStr) {
    if (timeStr.empty()) {
        return 0;
    }

    std::stringstream ss(timeStr);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ss, token, ':')) {
        parts.push_back(token);
    }

    int days = 0, hours = 0, minutes = 0, seconds = 0;
    int n = parts.size();

    try {
        if (n >= 1) seconds = std::stoi(parts[n - 1]);
        if (n >= 2) minutes = std::stoi(parts[n - 2]);
        if (n >= 3) hours = std::stoi(parts[n - 3]);
        if (n >= 4) days = std::stoi(parts[n - 4]);
    } catch (const std::exception&) {
        return 0;
    }

    return (days * 24 * 60) + (hours * 60) + minutes + (seconds > 30 ? 1 : 0);
}

std::string trim(const std::string &s) {
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){ return std::isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c){ return std::isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

}  // namespace MaratonaScore
