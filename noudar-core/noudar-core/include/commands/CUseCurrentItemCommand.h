//
// Created by monty on 13/02/17.
//

#ifndef NOUDAR_CORE_CUSECURRENTITEMCOMMAND_H
#define NOUDAR_CORE_CUSECURRENTITEMCOMMAND_H

namespace Knights {
	class CUseCurrentItemCommand : public IGameCommand {

		std::shared_ptr <CActor> mActor;
	public:
		CUseCurrentItemCommand( std::shared_ptr<CGame> aGame, std::shared_ptr <CActor> aActor );

		virtual std::string to_string() const override;

		virtual bool shouldEndTurn() override;

		virtual void execute() override;
	};
}

#endif //NOUDAR_CORE_CUSECURRENTITEMCOMMAND_H
