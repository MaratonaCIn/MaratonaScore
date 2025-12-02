#ifndef MSCR_CLI_COMMANDS_COMMAND_HPP
#define MSCR_CLI_COMMANDS_COMMAND_HPP

namespace MaratonaScore::CLI{
    class Command{
    public:
        virtual ~Command() = default;

        virtual void execute() = 0;
    };
}

#endif // MSCR_CLI_COMMANDS_COMMAND_HPP