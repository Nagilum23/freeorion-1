
#ifndef _FREEORION_COMBAT_SYSTEM_H_
#define _FREEORION_COMBAT_SYSTEM_H_

#include <vector>

class Empire;
class Fleet;
class Planet;

struct CombatAssets
{
    Empire* owner;
    std::vector<Fleet*> fleets;
    std::vector<Planet*> planets;
};


class CombatSystem
{
public:
    
    /**
        Resolves a battle in the given system, taking into account
        fleets and defensive bases.  Ships and fleets will either
        be destroyed, or will retreat, as per the 0.1 requirements doc.
        Systems may change hands as well.
    */
    void ResolveCombat( std::vector<CombatAssets> assets );
    


};

#endif


