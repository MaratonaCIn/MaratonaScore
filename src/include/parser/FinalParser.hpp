#ifndef MSCR_PARSER_SCOREBOARDPARSER_HPP
#define MSCR_PARSER_SCOREBOARDPARSER_HPP

#include <string>

#include "export.hpp"
#include "models/Contest.hpp"
#include "utils/Settings.hpp"

namespace MaratonaScore {

class MARATONASCORE_API FinalParser {
   public:
    Contest parse(const std::string& file_path);
};

}  // namespace MaratonaScore

#endif  // MSCR_PARSER_SCOREBOARDPARSER_HPP