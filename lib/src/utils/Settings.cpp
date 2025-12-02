//    Copyright 2025 MaratonaCIn
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include "utils/Settings.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <stdexcept>

namespace MaratonaScore {

Settings::Settings() { setDefaultValues(); }

Settings& Settings::getInstance() {
    static Settings instance;
    return instance;
}

void Settings::setDefaultValues() {
    // Time limits
    CONTEST_TIME_LIMIT = 300;
    HOMEWORK_TIME_LIMIT = 9600;

    // Base values
    CONTEST_BASE_VALUE = 4;
    HOMEWORK_BASE_VALUE = 2;
    UPSOLING_BASE_VALUE = 1;

    // Score bonuses
    CONTEST_SCORE_BONUS = 20;
    HOMEWORK_SCORE_BONUS = 10;

    // Person bonuses
    CONTEST_PERSON_BONUS = 5;
    HOMEWORK_PERSON_BONUS = 5;

    // Contest settings
    IGNORE_WORST_CONTESTS = 2;
    NUMBER_OF_CONTESTS = 10;
}

void Settings::loadFromFile(const std::string& filename) {
    try {
        YAML::Node config = YAML::LoadFile(filename);

        // Load time limits
        if (config["time_limits"]) {
            if (config["time_limits"]["contest"]) {
                CONTEST_TIME_LIMIT = config["time_limits"]["contest"].as<int>();
            }
            if (config["time_limits"]["homework"]) {
                HOMEWORK_TIME_LIMIT =
                    config["time_limits"]["homework"].as<int>();
            }
        }

        // Load base values
        if (config["base_values"]) {
            if (config["base_values"]["contest"]) {
                CONTEST_BASE_VALUE = config["base_values"]["contest"].as<int>();
            }
            if (config["base_values"]["homework"]) {
                HOMEWORK_BASE_VALUE =
                    config["base_values"]["homework"].as<int>();
            }
            if (config["base_values"]["upsolving"]) {
                UPSOLING_BASE_VALUE =
                    config["base_values"]["upsolving"].as<int>();
            }
        }

        // Load score bonuses
        if (config["score_bonuses"]) {
            if (config["score_bonuses"]["contest"]) {
                CONTEST_SCORE_BONUS =
                    config["score_bonuses"]["contest"].as<int>();
            }
            if (config["score_bonuses"]["homework"]) {
                HOMEWORK_SCORE_BONUS =
                    config["score_bonuses"]["homework"].as<int>();
            }
        }

        // Load person bonuses
        if (config["person_bonuses"]) {
            if (config["person_bonuses"]["contest"]) {
                CONTEST_PERSON_BONUS =
                    config["person_bonuses"]["contest"].as<int>();
            }
            if (config["person_bonuses"]["homework"]) {
                HOMEWORK_PERSON_BONUS =
                    config["person_bonuses"]["homework"].as<int>();
            }
        }

        // Load contest settings
        if (config["contest_settings"]) {
            if (config["contest_settings"]["ignore_worst_contests"]) {
                IGNORE_WORST_CONTESTS =
                    config["contest_settings"]["ignore_worst_contests"]
                        .as<int>();
            }
            if (config["contest_settings"]["number_of_contests"]) {
                NUMBER_OF_CONTESTS =
                    config["contest_settings"]["number_of_contests"].as<int>();
            }
        }

        std::cout << "Configuration loaded successfully from: " << filename
                  << std::endl;
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading configuration file: " << e.what()
                  << std::endl;
        std::cerr << "Using default values." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Using default values." << std::endl;
    }
}

}  // namespace MaratonaScore
