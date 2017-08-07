//
// Created by monty on 08/12/16.
//

#ifndef NOUDAR_CORE_COMMANDS_CLOADNEWLEVELCOMMAND_H
#define NOUDAR_CORE_COMMANDS_CLOADNEWLEVELCOMMAND_H

namespace Knights {
	class CLoadNewLevelCommand : public IGameCommand {
		int mMapSlot;
	public:
		CLoadNewLevelCommand( std::shared_ptr<CGame> aGame, int mapSlot );
		virtual std::string to_string() const override;
		virtual bool shouldEndTurn() override;
		virtual void execute() override;
	};
}


#endif //NOUDAR_CORE_CLOADNEWLEVELCOMMAND_H
