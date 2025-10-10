#ifndef MARATONASCORE_CLI_EXCEL_CONVERTER_H
#define MARATONASCORE_CLI_EXCEL_CONVERTER_H

#include <string>

namespace MaratonaScore {
namespace cli {

/**
 * @brief Handles Excel to JSON conversion using Python script
 *
 * This class provides a C++ wrapper around the Python conversion script.
 */
class ExcelConverter {
   public:
    /**
     * @brief Convert Excel file to JSON using Python script
     *
     * @param excel_path Path to input Excel file
     * @param json_path Path to output JSON file
     * @param python_script_path Path to the Python conversion script (optional)
     * @return true if successful, false otherwise
     */
    static bool convertExcelToJson(const std::string& excel_path,
                                   const std::string& json_path,
                                   const std::string& python_script_path = "");

    /**
     * @brief Batch convert multiple Excel files to JSON
     *
     * @param batch_file Path to batch configuration file
     * @return Number of successful conversions
     */
    static int convertBatch(const std::string& batch_file);

   private:
    static std::string findPythonScript();
    static bool fileExists(const std::string& path);
};

}  // namespace cli
}  // namespace MaratonaScore

#endif  // MARATONASCORE_CLI_EXCEL_CONVERTER_H
