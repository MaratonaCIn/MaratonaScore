#include "MaratonaScore/cli/CLIHandler.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MaratonaScore/cli/ExcelConverter.h"
#include "MaratonaScore/cli/OutputFormatter.h"

namespace MaratonaScore {
namespace cli {

CLIHandler::CLIHandler() : database_filename_("ratings_v2.json") {}

bool CLIHandler::loadDatabase() {
    auto config = rating_system_.getConfig();
    std::map<std::string, core::models::CompetitorData> competitors;
    std::map<std::string, core::models::ProcessedContest> processed_contests;

    bool success = core::persistence::DatabaseManager::loadDatabase(
        database_filename_, config, competitors, processed_contests);

    if (success || competitors.empty()) {
        rating_system_.setConfig(config);
        rating_system_.setCompetitors(competitors);
        rating_system_.setProcessedContests(processed_contests);
        rating_system_.recalculateAllScores();
    }

    return success;
}

bool CLIHandler::saveDatabase() {
    return core::persistence::DatabaseManager::saveDatabase(
        database_filename_, rating_system_.getConfig(),
        rating_system_.getCompetitors(), rating_system_.getProcessedContests());
}

int CLIHandler::run(int argc, char* argv[]) {
    if (argc < 2) {
        OutputFormatter::printUsage(argv[0]);
        return 0;
    }

    loadDatabase();

    std::string command = argv[1];

    if (command == "process" && argc >= 6) {
        return handleProcessCommand(argc, argv);
    } else if (command == "batch" && argc >= 3) {
        return handleBatchCommand(argc, argv);
    } else if (command == "rankings") {
        return handleRankingsCommand();
    } else if (command == "config") {
        return handleConfigCommand();
    } else if (command == "edit-config" && argc >= 3) {
        return handleEditConfigCommand(argc, argv);
    } else if (command == "convert" && argc >= 4) {
        return handleConvertCommand(argc, argv);
    } else if (command == "convert-batch" && argc >= 3) {
        return handleConvertBatchCommand(argc, argv);
    } else {
        std::cerr << "[ERROR] Invalid command or missing arguments.\n";
        return 1;
    }
}

int CLIHandler::handleProcessCommand(int /* argc */, char* argv[]) {
    std::string contest_id = argv[2];
    std::string scoreboard_file = argv[3];
    std::string contest_type = argv[4];
    int duration = std::atoi(argv[5]);

    bool success = rating_system_.processScoreboard(contest_id, scoreboard_file,
                                                    contest_type, duration);
    if (success) {
        saveDatabase();
        OutputFormatter::printRankings(rating_system_.getCompetitors());
        return 0;
    }
    return 1;
}

int CLIHandler::handleBatchCommand(int /* argc */, char* argv[]) {
    std::string config_file = argv[2];
    std::ifstream file(config_file);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open config file: " << config_file
                  << "\n";
        return 1;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string contest_id, scoreboard_file, contest_type;
        int duration;

        if (iss >> contest_id >> scoreboard_file >> contest_type >> duration) {
            if (rating_system_.processScoreboard(contest_id, scoreboard_file,
                                                 contest_type, duration)) {
                count++;
            }
        }
    }

    file.close();

    std::cout << "\n[OK] Processed " << count << " contests from batch file.\n";
    saveDatabase();
    OutputFormatter::printRankings(rating_system_.getCompetitors());
    return 0;
}

int CLIHandler::handleRankingsCommand() {
    OutputFormatter::printRankings(rating_system_.getCompetitors());
    return 0;
}

int CLIHandler::handleConfigCommand() {
    OutputFormatter::printConfig(rating_system_.getConfig());
    return 0;
}

int CLIHandler::handleConvertCommand(int /* argc */, char* argv[]) {
    std::string excel_file = argv[2];
    std::string json_file = argv[3];

    if (ExcelConverter::convertExcelToJson(excel_file, json_file)) {
        return 0;
    }
    return 1;
}

int CLIHandler::handleConvertBatchCommand(int /* argc */, char* argv[]) {
    std::string batch_file = argv[2];
    int success_count = ExcelConverter::convertBatch(batch_file);
    return (success_count > 0) ? 0 : 1;
}

int CLIHandler::handleEditConfigCommand(int argc, char* argv[]) {
    std::string param = argv[2];

    auto config = rating_system_.getConfig();
    bool modified = false;

    if (param == "contest-weight" && argc >= 4) {
        config.contest_problems_weight = std::atof(argv[3]);
        modified = true;
    } else if (param == "contest-double" && argc >= 4) {
        config.contests_to_double = std::atoi(argv[3]);
        modified = true;
    } else if (param == "contest-bonus-top" && argc >= 4) {
        config.rank_bonus_top_n = std::atoi(argv[3]);
        modified = true;
    } else if (param == "contest-bonus-max" && argc >= 4) {
        config.rank_bonus_max = std::atof(argv[3]);
        modified = true;
    } else if (param == "homework-weight" && argc >= 4) {
        config.homework_problems_weight = std::atof(argv[3]);
        modified = true;
    } else if (param == "homework-double" && argc >= 4) {
        config.homework_contests_to_double = std::atoi(argv[3]);
        modified = true;
    } else if (param == "homework-bonus-top" && argc >= 4) {
        config.homework_rank_bonus_top_n = std::atoi(argv[3]);
        modified = true;
    } else if (param == "homework-bonus-max" && argc >= 4) {
        config.homework_rank_bonus_max = std::atof(argv[3]);
        modified = true;
    } else if (param == "upsolving-points" && argc >= 4) {
        config.upsolving_points_per_problem = std::atof(argv[3]);
        modified = true;
    } else {
        std::cerr << "[ERROR] Invalid parameter or missing value.\n\n";
        std::cout << "Usage: " << argv[0] << " edit-config <parameter> <value>\n\n";
        std::cout << "Parameters:\n";
        std::cout << "  contest-weight <num>      - Contest base weight (default: 100)\n";
        std::cout << "  contest-double <num>      - Contests to double (default: 11)\n";
        std::cout << "  contest-bonus-top <num>   - Top N for bonus (default: 10)\n";
        std::cout << "  contest-bonus-max <num>   - Max bonus (default: 20)\n";
        std::cout << "  homework-weight <num>     - Homework base weight (default: 50)\n";
        std::cout << "  homework-double <num>     - Homeworks to double (default: 11)\n";
        std::cout << "  homework-bonus-top <num>  - Top N for bonus (default: 5)\n";
        std::cout << "  homework-bonus-max <num>  - Max bonus (default: 10)\n";
        std::cout << "  upsolving-points <num>    - Points per problem (default: 5)\n";
        return 1;
    }

    if (modified) {
        rating_system_.setConfig(config);
        rating_system_.recalculateAllScores();
        saveDatabase();

        std::cout << "\n[OK] Configuration updated!\n\n";
        OutputFormatter::printConfig(config);
        return 0;
    }

    return 1;
}

}  // namespace cli
}  // namespace MaratonaScore
