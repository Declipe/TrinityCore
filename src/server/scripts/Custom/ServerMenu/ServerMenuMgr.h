#ifndef SERVER_MENU_H
#define SERVER_MENU_H

#define UNIQUE_MENU_ID 123

#define GetCustomText(a, b, c)    a->GetSession()->GetSessionDbLocaleIndex() == LOCALE_ruRU ? b : c


class sServerMenu
{
public:
    static sServerMenu* instance()
    {
        static sServerMenu* instance = new sServerMenu();
        return instance;
    }

    //void Openb(Player* /*player*/);
    void OpenBankSlot(Player* player);
    void GossipHelloMenu(Player* player);
    bool CanOpenMenu(Player* player);
};

#define sServerMenuMgr sServerMenu::instance()
#endif
