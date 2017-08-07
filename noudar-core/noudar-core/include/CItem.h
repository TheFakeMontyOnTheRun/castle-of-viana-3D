//
// Created by monty on 12/02/17.
//

#ifndef NOUDAR_CORE_CITEM_H
#define NOUDAR_CORE_CITEM_H

namespace Knights {


	//hate this...
	class CMap;
	class CActor;

	using CItemAction = std::function< void(std::shared_ptr<CActor>, std::shared_ptr<CMap>)>;

	const CItemAction kItemDoNothingAction = [](std::shared_ptr<CActor>, std::shared_ptr<CMap>){};
	using ItemView = char;

	class CItem {
	protected:
		std::string mName;
		ItemView mView;
        bool mConsumable = false;
		bool mCanBeDropped = false;
		CItemAction mItemAction = kItemDoNothingAction;
        CItemAction mItemPickAction = kItemDoNothingAction;
        CItemAction mItemDropAction = kItemDoNothingAction;
	public:
        CItem(std::string aName, ItemView aView, bool aConsumable, bool aCanBeDropped, const CItemAction& itemUseAction );
		CItem(std::string aName, ItemView aView, bool aConsumable, bool aCanBeDropped, const CItemAction& itemUseAction, const CItemAction& itemPickAction, const CItemAction& itemDropAction );
		CItem(std::string aName, ItemView aView );

        std::string to_string() const;

		void use(std::shared_ptr<CActor>, std::shared_ptr<CMap>);
		ItemView getView() const;
		bool isConsumable() const;
		bool canBeDropped() const;
	};

	std::ostream &operator<<(std::ostream &os, const CItem &action);

	std::string to_string(const CItem &action);


}


#endif //NOUDAR_CORE_CITEM_H
