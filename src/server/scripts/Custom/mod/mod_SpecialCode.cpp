#include "Custom/Dcl.h"


#define SQL_CODE "SELECT `CodeId`, `MailMoney`, `MaxCountForCharacter`, `MaxCountForAccount`, `MaxCountTotal`, `MailSubject`, `MailText` FROM `world_coded` WHERE `CodeStart` <= CURRENT_TIMESTAMP AND `CodeEnd` >= CURRENT_TIMESTAMP AND `CodeData` = '{}' LIMIT 1"
#define SQL_CODE_COUNT "SELECT COUNT(*) FROM `world_coded_history` WHERE `CodeId` = '{}'"
#define SQL_CODE_COUNT_ACCOUNT "SELECT COUNT(*) FROM `world_coded_history` WHERE `CodeId` = '{}' AND `AccountId` = '{}'"
#define SQL_CODE_COUNT_CHARACTER "SELECT COUNT(*) FROM `world_coded_history` WHERE `CodeId` = '{}' AND `CharacterGuid` = '{}'"
#define SQL_CODE_HISTORY "INSERT INTO `world_coded_history` (`CodeId`, `CharacterGuid`, `AccountId`, `SessionIp`) VALUES ('{}', '{}', '{}', '{}')"
#define SQL_CODE_ITEMS "SELECT `ItemId`, `ItemCount`, `ItemId1`, `ItemCount1`, `ItemId2`, `ItemCount2`, `ItemId3`, `ItemCount3`, `ItemId4`, `ItemCount4` FROM `world_coded_items` WHERE `CodeId` = '{}' AND (`ItemClassMask` & {} OR `ItemClassMask` = 0) AND (`ItemRaceMask` & {} OR `ItemRaceMask` = 0)"

bool SCEnable = false;

class Mod_SpecialCode_WorldScript : public WorldScript
{
    public:
        Mod_SpecialCode_WorldScript() : WorldScript("Mod_SpecialCode_WorldScript") { }

    void OnConfigLoad(bool /*reload*/)
    {
        SCEnable = sGameConfig->GetBoolConfig("SpecialCode.Enable");
    }
};

class Mod_SpecialCode_AllCreatureScript : public AllCreatureScript
{
    public:
        Mod_SpecialCode_AllCreatureScript() : AllCreatureScript("Mod_SpecialCode_AllCreatureScript") { }

    void AllCreatureCode(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/, const char* code)
    {
        if (!SCEnable || !player || !*code)
            return;

        std::string std_code(code);
        CharacterDatabase.EscapeString(std_code);
        QueryResult result = CharacterDatabase.PQuery(SQL_CODE, std_code.c_str());
        if (!result)
        {
            CloseGossipMenuFor(player);
            return;
        }

        uint32 codeId = (*result)[0].GetUInt32();
        std::string subject = (*result)[5].GetString();
        std::string text = (*result)[6].GetString();
        int32 mailMoney = (*result)[1].GetInt32();
        uint8 maxCharacters = (*result)[2].GetUInt8();
        uint8 maxAccount = (*result)[3].GetUInt8();
        uint8 maxTotal = (*result)[4].GetUInt8();
        uint16 currentTotal = 0;
        uint16 currentAccount = 0;
        uint16 currentCharacter = 0;

        if (maxTotal > 0)
        {
            result = CharacterDatabase.PQuery(SQL_CODE_COUNT, codeId);
            if (result)
                currentTotal = (*result)[0].GetUInt16();

            if (currentTotal >= maxTotal)
            {
                CloseGossipMenuFor(player);
                return;
            }
        }

        uint32 accountId = player->GetSession()->GetAccountId();

        if (maxAccount > 0)
        {
            result = CharacterDatabase.PQuery(SQL_CODE_COUNT_ACCOUNT, codeId, accountId);
            if (result)
                currentAccount = (*result)[0].GetUInt16();

            if (currentAccount >= maxAccount)
            {
                CloseGossipMenuFor(player);
                return;
            }
        }

        uint32 guid = player->GetGUID();

        if (maxCharacters > 0)
        {
            result = CharacterDatabase.PQuery(SQL_CODE_COUNT_CHARACTER, codeId, guid);
            if (result)
                currentCharacter = (*result)[0].GetUInt16();

            if (currentCharacter >= maxCharacters)
            {
                CloseGossipMenuFor(player);
                return;
            }
        }

        CharacterDatabase.PExecute(SQL_CODE_HISTORY, codeId, guid, accountId, player->GetSession()->GetRemoteAddress().c_str());

        MailDraft draft(subject, text);
        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        //SQLTransaction trans = CharacterDatabase.BeginTransaction();

        result = CharacterDatabase.PQuery(SQL_CODE_ITEMS, codeId, player->GetClassMask(), player->GetRaceMask());

        if (result)
        {
            typedef std::pair<uint32, uint32> ItemPair;
            typedef std::list< ItemPair > ItemPairs;
            ItemPairs items;

            do
            {
                Field* fields = result->Fetch();

                uint32 itemId = fields[0].GetUInt32();
                uint16 itemCount = fields[1].GetUInt32();
                uint32 itemId1 = fields[2].GetUInt32();
                uint16 itemCount1 = fields[3].GetUInt32();
                uint32 itemId2 = fields[4].GetUInt32();
                uint16 itemCount2 = fields[5].GetUInt32();
                uint32 itemId3 = fields[6].GetUInt32();
                uint16 itemCount3 = fields[7].GetUInt16();
                uint32 itemId4 = fields[8].GetUInt32();
                uint16 itemCount4 = fields[9].GetUInt16();

                ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemId);
                if (!itemProto)
                    continue;
                /**/
                ItemTemplate const* itemProto1 = sObjectMgr->GetItemTemplate(itemId1);
                if (!itemProto1)
                    continue;
                ItemTemplate const* itemProto2 = sObjectMgr->GetItemTemplate(itemId2);
                if (!itemProto2)
                    continue;
                ItemTemplate const* itemProto3 = sObjectMgr->GetItemTemplate(itemId3);
                if (!itemProto3)
                    continue;
                ItemTemplate const* itemProto4 = sObjectMgr->GetItemTemplate(itemId4);
                if (!itemProto4)
                    continue;

                if (itemCount < 1 || (itemProto->MaxCount > 0 && itemCount > uint32(itemProto->MaxCount)))
                    continue;

                if (itemCount1 < 1 || (itemProto1->MaxCount > 0 && itemCount1 > uint32(itemProto1->MaxCount)))
                    continue;

                if (itemCount2 < 1 || (itemProto2->MaxCount > 0 && itemCount2 > uint32(itemProto2->MaxCount)))
                    continue;
                if (itemCount3 < 1 || (itemProto3->MaxCount > 0 && itemCount3 > uint32(itemProto3->MaxCount)))
                    continue;
                if (itemCount4 < 1 || (itemProto4->MaxCount > 0 && itemCount4 > uint32(itemProto4->MaxCount)))
                    continue;

                while (itemCount > itemProto->GetMaxStackSize())
                {
                    items.push_back(ItemPair(itemId, itemProto->GetMaxStackSize()));
                    itemCount -= itemProto->GetMaxStackSize();
                }

                while (itemCount1 > itemProto->GetMaxStackSize())
                {
                    items.push_back(ItemPair(itemId1, itemProto1->GetMaxStackSize()));
                    itemCount1 -= itemProto1->GetMaxStackSize();
                }

                while (itemCount2 > itemProto->GetMaxStackSize())
                {
                    items.push_back(ItemPair(itemId2, itemProto2->GetMaxStackSize()));
                    itemCount2 -= itemProto2->GetMaxStackSize();
                }

                while (itemCount3 > itemProto3->GetMaxStackSize())
                {
                    items.push_back(ItemPair(itemId3, itemProto3->GetMaxStackSize()));
                    itemCount3 -= itemProto3->GetMaxStackSize();
                }

                while (itemCount4 > itemProto4->GetMaxStackSize())
                {
                    items.push_back(ItemPair(itemId4, itemProto4->GetMaxStackSize()));
                    itemCount4 -= itemProto4->GetMaxStackSize();
                }

                items.push_back(ItemPair(itemId, itemCount));

                items.push_back(ItemPair(itemId1, itemCount1));

                items.push_back(ItemPair(itemId2, itemCount2));

                items.push_back(ItemPair(itemId3, itemCount3));

                items.push_back(ItemPair(itemId4, itemCount4));

                if (items.size() > MAX_MAIL_ITEMS)
                {
                    CloseGossipMenuFor(player);
                    return;
                }
            }
            while (result->NextRow());

            for (ItemPairs::const_iterator itr = items.begin(); itr != items.end(); ++itr)
            {
                if (Item* item = Item::CreateItem(itr->first, itr->second, 0))
                {
                    item->SaveToDB(trans);
                    draft.AddItem(item);
                }
            }
        }

        if (mailMoney > 0)
            draft.AddMoney(mailMoney);
        else
            draft.AddCOD(mailMoney*-1);

        draft.SendMailTo(trans, MailReceiver(player, guid), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM));
        CharacterDatabase.CommitTransaction(trans);
        CloseGossipMenuFor(player);
    }

};

void AddSC_Mod_SpecialCode()
{
    new Mod_SpecialCode_WorldScript();
    new Mod_SpecialCode_AllCreatureScript();
}
