#ifndef NOUDAR_CORE_CMONSTER_H
#define NOUDAR_CORE_CMONSTER_H
namespace Knights {

    class CMap;

    using CMonsterUpdateCallback = std::function< void(std::shared_ptr <CMap> map, std::shared_ptr<CActor> me)>;

    class CMonster : public CCharacter {
        CMonsterUpdateCallback mUpdateCallback;
        int mViewRange;
    public:
        CMonster( std::shared_ptr<CCharacterArchetype> aArchetype, std::shared_ptr<CTeam> aTeam, ActorId aId, int viewRange, CMonsterUpdateCallback updateCallback);
        CMonster( std::shared_ptr<CCharacterArchetype> aArchetype, std::shared_ptr<CTeam> aTeam, ActorId aId, int viewRange );

        void update(std::shared_ptr <CMap> map) override;

        bool actOn(int newX, int newY, std::shared_ptr <CMap> map);

        bool dealWith( std::shared_ptr<CMap> map, int x, int y );
    };
}
#endif
