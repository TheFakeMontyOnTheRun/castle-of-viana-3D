//
// Created by monty on 25/10/16.
//

#ifndef NOUDAR_CORE_COMMANDS_CENDTURNCOMMAND_H
#define NOUDAR_CORE_COMMANDS_CENDTURNCOMMAND_H

namespace Knights {
    class CEndTurnCommand: public IGameCommand {
    public:
        CEndTurnCommand( std::shared_ptr<CGame> aGame );
        virtual std::string to_string() const override;
        virtual bool shouldEndTurn() override;
        virtual void execute() override;
    };
}

#endif //NOUDAR_CORE_ACTORMELEEATTACKACTION_H
