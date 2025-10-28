#ifndef MARATONASCORE_CLI_CLI_HANDLER_H
#define MARATONASCORE_CLI_CLI_HANDLER_H

#include <string>

#include "MaratonaScore/core/persistence/DatabaseManager.h"
#include "MaratonaScore/core/services/RatingSystem.h"

namespace MaratonaScore {
namespace cli {

/**
 * @brief Handles command-line interface operations
 *
 * Parses arguments, coordinates rating system and database operations,
 * and manages the CLI workflow.
 */
class CLIHandler {
   public:
    CLIHandler();

    /**
     * @brief Run the CLI application
     *
     * @param argc Argument count
     * @param argv Argument values
     * @return Exit code (0 for success)
     */
    int run(int argc, char* argv[]);

   private:
    core::services::RatingSystem rating_system_;
    std::string database_filename_;

    bool loadDatabase();
    bool saveDatabase();

    int handleProcessCommand(int argc, char* argv[]);
    int handleBatchCommand(int argc, char* argv[]);
    int handleRankingsCommand();
    int handleConfigCommand();
    int handleEditConfigCommand(int argc, char* argv[]);
    int handleConvertCommand(int argc, char* argv[]);
    int handleConvertBatchCommand(int argc, char* argv[]);
};

}  // namespace cli
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CLI_CLI_HANDLER_H
