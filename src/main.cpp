/**
 * @file main.cpp
 * @brief Entry point for MaratonaCIn Rating System CLI application
 *
 * This is a clean CLI entry point that delegates all logic to the CLIHandler.
 * The core business logic is independent of this interface, allowing for
 * future GUI implementations.
 */

#include "MaratonaScore/cli/CLIHandler.h"

int main(int argc, char* argv[]) {
    MaratonaScore::cli::CLIHandler handler;
    return handler.run(argc, argv);
}
