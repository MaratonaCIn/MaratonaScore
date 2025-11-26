#ifndef MSCR_UTILS_STRING_UTILS_HPP
#define MSCR_UTILS_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <vector>

namespace MaratonaScore {

int timeStringToMinutes(const std::string& timeStr);
std::string trim(const std::string &s);

}  // namespace MaratonaScore

#endif  // MSCR_UTILS_STRING_UTILS_HPP