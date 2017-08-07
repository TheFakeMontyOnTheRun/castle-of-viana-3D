//
// Created by monty on 12/02/17.
//

#ifndef NOUDAR_CORE_CCYCLEPREVIOUSITEMCOMMAND_H
#define NOUDAR_CORE_CCYCLEPREVIOUSITEMCOMMAND_H

namespace Knights {
	class CCyclePreviousItemCommand : public IGameCommand {

		std::shared_ptr<CActor> mActor;
	public:
		CCyclePreviousItemCommand( std::shared_ptr<CGame> aGame, std::shared_ptr<CActor> aActor );
		virtual std::string to_string() const override;
		virtual bool shouldEndTurn() override;
		virtual void execute() override;
	};
}

#endif //NOUDAR_CORE_CCYCLEPREVIOUSITEMCOMMAND_H
