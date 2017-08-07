//
// Created by monty on 25/10/16.
//

#ifndef NOUDAR_CORE_COMMANDS_CMOVEACTORCOMMAND_H
#define NOUDAR_CORE_COMMANDS_CMOVEACTORCOMMAND_H

namespace Knights {
    class CMoveActorCommand : public IGameCommand {
        EDirection mDirection;
        std::shared_ptr<CActor> mActor;
    public:
        CMoveActorCommand( std::shared_ptr<CGame> aGame, EDirection direction, std::shared_ptr<CActor> aActor );
        virtual std::string to_string() const override;
        virtual bool shouldEndTurn() override;
        virtual void execute() override;
    };
}

#endif //NOUDAR_CORE_ACTORMELEEATTACKACTION_H
