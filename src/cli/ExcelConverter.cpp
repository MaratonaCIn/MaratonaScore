#include "MaratonaScore/cli/ExcelConverter.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace MaratonaScore {
namespace cli {

bool ExcelConverter::fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::string ExcelConverter::findPythonScript() {
    // Try common locations relative to executable
    const char* possible_paths[] = {
        "scripts/vJudgeParse.py", "../scripts/vJudgeParse.py",
        "../../scripts/vJudgeParse.py", "./vJudgeParse.py", "vJudgeParse.py"};

    for (const auto& path : possible_paths) {
        if (fileExists(path)) {
            return path;
        }
    }

    return "";
}

bool ExcelConverter::convertExcelToJson(const std::string& excel_path,
                                        const std::string& json_path,
                                        const std::string& python_script_path) {
    // Check if Excel file exists
    if (!fileExists(excel_path)) {
        std::cerr << "[ERROR] Excel file not found: " << excel_path << "\n";
        return false;
    }

    // Find Python script
    std::string script_path =
        python_script_path.empty() ? findPythonScript() : python_script_path;

    if (script_path.empty()) {
        std::cerr
            << "[ERROR] Python conversion script (vJudgeParse.py) not found.\n";
        std::cerr << "        Please ensure vJudgeParse.py is in the scripts/ "
                     "directory or current directory.\n";
        return false;
    }

    std::cout << "[INFO] Using Python script: " << script_path << "\n";
    std::cout << "[INFO] Converting Excel to JSON...\n";

    // Build command
    std::string command = "python \"" + script_path + "\" \"" + excel_path +
                          "\" \"" + json_path + "\"";

    std::cout << "[INFO] Running: " << command << "\n";

    // Execute Python script
    int result = std::system(command.c_str());

    if (result == 0) {
        // Verify output file was created
        if (fileExists(json_path)) {
            std::cout << "[OK] Successfully converted to: " << json_path
                      << "\n";
            return true;
        } else {
            std::cerr
                << "[ERROR] Conversion completed but output file not found: "
                << json_path << "\n";
            return false;
        }
    } else {
        std::cerr << "[ERROR] Python script failed with code: " << result
                  << "\n";
        std::cerr << "        Make sure Python and required packages (pandas, "
                     "openpyxl) are installed.\n";
        std::cerr << "        Install with: pip install pandas openpyxl\n";
        return false;
    }
}

int ExcelConverter::convertBatch(const std::string& batch_file) {
    std::ifstream file(batch_file);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open batch file: " << batch_file
                  << "\n";
        return 0;
    }

    std::cout << "\n" << std::string(78, '=') << "\n";
    std::cout
        << "           BATCH EXCEL TO JSON CONVERSION                    \n";
    std::cout << std::string(78, '=') << "\n\n";

    std::string line;
    int count = 0;
    int success_count = 0;
    int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        count++;

        // Parse line: excel_file json_file (supports quoted strings)
        std::istringstream iss(line);
        std::string excel_file, json_file;

        // Parse first field (may be quoted)
        iss >> std::ws;  // Skip leading whitespace
        if (iss.peek() == '"') {
            // Read quoted string
            iss.get();  // Skip opening quote
            std::getline(iss, excel_file, '"');
        } else {
            // Read unquoted string
            iss >> excel_file;
        }

        // Parse second field (may be quoted)
        iss >> std::ws;  // Skip whitespace between fields
        if (iss.peek() == '"') {
            // Read quoted string
            iss.get();  // Skip opening quote
            std::getline(iss, json_file, '"');
        } else {
            // Read unquoted string
            iss >> json_file;
        }

        if (!excel_file.empty() && !json_file.empty()) {
            std::cout << "[" << count << "] Converting: " << excel_file
                      << " -> " << json_file << "\n";

            if (convertExcelToJson(excel_file, json_file)) {
                success_count++;
            } else {
                std::cerr << "    [FAILED] Line " << line_number
                          << " in batch file\n";
            }
            std::cout << "\n";
        } else {
            std::cerr << "[WARNING] Invalid format at line " << line_number
                      << ": " << line << "\n";
            std::cerr << "          Expected: <excel_file> <json_file>\n\n";
        }
    }

    file.close();

    std::cout << std::string(78, '=') << "\n";
    std::cout << "BATCH CONVERSION COMPLETE\n";
    std::cout << "Total: " << count << " | Success: " << success_count
              << " | Failed: " << (count - success_count) << "\n";
    std::cout << std::string(78, '=') << "\n\n";

    return success_count;
}

}  // namespace cli
}  // namespace MaratonaScore
