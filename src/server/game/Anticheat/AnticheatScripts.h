#ifndef SC_ACSCRIPTS_H
#define SC_ACSCRIPTS_H

#include "ScriptMgr.h"

class TC_GAME_API AnticheatScripts: public PlayerScript
{
    public:
        AnticheatScripts();

        void OnLogout(Player* player);
        void OnLogin(Player* player,bool);
};

#endif