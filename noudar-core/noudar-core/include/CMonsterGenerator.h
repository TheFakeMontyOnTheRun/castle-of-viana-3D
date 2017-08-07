#ifndef NOUDAR_CORE_CMONSTERGENERATOR_H
#define NOUDAR_CORE_CMONSTERGENERATOR_H

namespace Knights {
    class CMonsterGenerator : public CActor {
        std::shared_ptr<CCharacterArchetype> mArchetypeToBuild;
        std::shared_ptr<CTeam> mTeam;
    public:
        CMonsterGenerator(std::shared_ptr<CCharacterArchetype> aAchertypeToBuild, std::shared_ptr<CTeam> aTeam, ActorId aId, int aliveForTurns);

        void update(std::shared_ptr<CMap> map) override;
    };
}
#endif
