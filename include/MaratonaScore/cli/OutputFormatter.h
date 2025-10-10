#ifndef MARATONASCORE_CLI_OUTPUT_FORMATTER_H
#define MARATONASCORE_CLI_OUTPUT_FORMATTER_H

#include <map>
#include <string>

#include "MaratonaScore/core/models/CompetitorData.h"
#include "MaratonaScore/core/models/ScoringConfig.h"

namespace MaratonaScore {
namespace cli {

/**
 * @brief Formats and displays rankings and statistics
 *
 * Handles all console output formatting for the CLI application.
 */
class OutputFormatter {
   public:
    /**
     * @brief Print all rankings (final, contest, and absolute)
     */
    static void printRankings(
        const std::map<std::string, core::models::CompetitorData>& competitors);

    /**
     * @brief Print the current scoring configuration
     */
    static void printConfig(const core::models::ScoringConfig& config);

    /**
     * @brief Print usage/help information
     */
    static void printUsage(const std::string& program_name);
};

}  // namespace cli
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CLI_OUTPUT_FORMATTER_H
