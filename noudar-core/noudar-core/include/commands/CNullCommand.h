//
// Created by monty on 29/10/16.
//

#ifndef NOUDAR_CORE_COMMANDS_CNULLCOMMAND_H
#define NOUDAR_CORE_COMMANDS_CNULLCOMMAND_H

namespace Knights {
    class CNullCommand : public IGameCommand {
    public:
        CNullCommand(std::shared_ptr <CGame> aGame);

        virtual std::string to_string() const override;

        virtual bool shouldEndTurn() override;

        virtual void execute() override;
    };
}

#endif //NOUDAR_CORE_CNULLCOMMAND_H
