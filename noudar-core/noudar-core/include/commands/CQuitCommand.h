//
// Created by monty on 25/10/16.
//

#ifndef NOUDAR_CORE_COMMANDS_CQUITCOMMAND_H
#define NOUDAR_CORE_COMMANDS_CQUITCOMMAND_H

namespace Knights {
    class CQuitCommand : public IGameCommand {
    public:
        CQuitCommand( std::shared_ptr<CGame> aGame );
        virtual std::string to_string() const override;
        virtual bool shouldEndTurn() override;
        virtual void execute() override;
    };
}

#endif //NOUDAR_CORE_ACTORMELEEATTACKACTION_H
