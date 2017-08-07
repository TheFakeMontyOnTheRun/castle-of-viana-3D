//
// Created by monty on 22-03-2017.
//

#ifndef NOUDAR_CORE_CSTORAGEITEM_H
#define NOUDAR_CORE_CSTORAGEITEM_H

namespace Knights {
    class CStorageItem : public CItem {
        int mAmount = 0;
        std::string mOriginalName;
    public:
        CStorageItem(const std::string &aName, ItemView aView, bool aConsumable, bool aCanBeDropped, const CItemAction &itemAction, const int initialAmount );

        void empty();
        int getAmount();
        int add( int amount );
    };
}

#endif //NOUDAR_CORE_CSTORAGEITEM_H
