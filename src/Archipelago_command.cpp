/*
    MIT License

    Copyright (c) 2018 José González

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "Archipelago.h"
#include "Chat.h"
#include "ChatCommand.h"
#include "ScriptMgr.h"

using namespace Acore::ChatCommands;

class archipelago_commandscript : public CommandScript
{
public:
    archipelago_commandscript() : CommandScript("archipelago_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable archipelagoCommandTable = {
            {"start",   HandleStartCommand,         SEC_PLAYER,     Console::No},
            {"",        HandleArchipelagoCommand,   SEC_PLAYER,     Console::No}
        };

        static ChatCommandTable commandTable = {
            {"archipelago", archipelagoCommandTable},
            {"arch",        archipelagoCommandTable}
        };

        return commandTable;
    }

    static bool HandleArchipelagoCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
        {
            handler->SendSysMessage(LANG_CMD_SYNTAX);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("USAGE: .archipelago start - sends your GUID to the archipelago client and starts sending and receiving checks");
        return true;
    }

    static bool HandleStartCommand(ChatHandler* handler, char const* args)
    {
        Player* player = handler->GetSession()->GetPlayer();
        Archipelago* instance = Archipelago::GetInstance();
        if (instance)
        {
            instance->sendHandshake(player);
            return true;
        }
        return false;
    }
};

void AddSC_archipelago_commandscript() { new archipelago_commandscript(); }