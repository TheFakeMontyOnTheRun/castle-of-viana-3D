//
// Created by monty on 25/10/16.
//

#ifndef NOUDAR_CORE_COMMANDS_IGAMECOMMAND_H
#define NOUDAR_CORE_COMMANDS_IGAMECOMMAND_H
namespace Knights {
    class IGameCommand {
        std::shared_ptr<CGame> mGame;
    public:
        IGameCommand( std::shared_ptr<CGame> aGame );
        std::shared_ptr<CGame> getGame();
        virtual bool shouldEndTurn() = 0;
        virtual void execute() = 0;
        virtual std::string to_string() const = 0;
    };

    std::ostream& operator<<(std::ostream& os, const IGameCommand& action);
    std::string to_string( const IGameCommand& action );
}
#endif //NOUDAR_CORE_ACTORACTION_H
