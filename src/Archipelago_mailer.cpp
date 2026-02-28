/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Archipelago.h"
#include "Archipelago_mailer.h"
#include "Mail.h"
#include "ScriptMgr.h"
#include "Player.h"
#include <map>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>

ArchipelagoMailer* ArchipelagoMailer::instance = nullptr;

ArchipelagoMailer::ArchipelagoMailer(): 
    WorldScript("ArchipelagoMailer") 
    {
        instance = this;
        enabled = sConfigMgr->GetOption<bool>("Archipelago.Enable", false);
    }

void ArchipelagoMailer::OnStartup()
{
    if (!enabled) { return; }

    serv = std::make_unique<server>(io_context, PORT);
    server_thread = std::make_unique<std::thread>([this](){
        auto work = boost::asio::make_work_guard(io_context);

        io_context.run();
    });
}

void ArchipelagoMailer::OnUpdate(uint32 diff)
{
    update_time += diff;
    if (update_time < 5000) { return; }
    if (!unsent_messages.empty()) { handleMessage(&unsent_messages, true); }
    std::unique_ptr<std::vector<std::string>> message_q = std::make_unique<std::vector<std::string>>();
    {
        std::lock_guard<std::mutex> lock(data_mutex);
        if (server::GetInstance()->message_queue->empty()) { return; }
        message_q.swap(server::GetInstance()->message_queue);
    }
    handleMessage(message_q.get(), false);
}

void ArchipelagoMailer::OnShutdown()
{
    io_context.stop();
    server_thread->join();
}

void ArchipelagoMailer::setFromLoad(uint8 armors, uint8 weaps)
{
    m_prog_armors_sent = armors;
    m_prog_weap_sent = weaps;
}

void ArchipelagoMailer::handleMessage(std::vector<std::string>* message_queue, bool old)
{
    for (const std::string& message : *message_queue)
    {
        bool sent = false;
        uint8 weap_sent = 0;
        uint8 armor_sent = 0;
        if (message == GOLD) {
            sent = sendMail(10000000, std::vector<uint32>());
        } else if (message == PROG_LEVEL) {
            sent = tryGrantLevels(5);
        } else if (message == PROG_ARMOR) {
            sent = sendMail(0, PROG_ARMOR_MAP.at(m_prog_armors_sent));
            armor_sent += 1;
        } else if (message == PROG_WEAP) {
            sent = sendMail(0, PROG_WEAP_MAP.at(m_prog_weap_sent));
            weap_sent += 1;
        } else if (message == PORT_HOLE) {
            sent = sendMail(0, PORT_HOLE_VEC);
        } else if (message == STARTER_LEVELS) {
            sent = tryGrantLevels(9);
        }

        if (sent) 
        {
            m_prog_weap_sent += weap_sent;
            m_prog_armors_sent += armor_sent;
            saveToFile();
            if (old)
            {
                remove(unsent_messages.begin(), unsent_messages.end(), message);
                unsent_messages.pop_back();
            }
        }
        else if (!old)
        {
            unsent_messages.push_back(message);
        }
    }
}

void ArchipelagoMailer::sendStarters()
{
    tryGrantLevels(9);
    sendMail(1000000, PROG_ARMOR_MAP.at(m_prog_armors_sent));
    sendMail(0, PROG_WEAP_MAP.at(m_prog_weap_sent));
    m_prog_armors_sent += 1;
    m_prog_weap_sent += 1;
    saveToFile();
}

bool ArchipelagoMailer::sendMail(const uint32 money, const std::vector<uint32>& items)
{
    ObjectGuid& player_guid = Archipelago::GetInstance()->GetPlayerGuid();
    MailDraft draft("Archipelago Reward", "A new reward has arrived from the multiworld!");
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    if (money > 0)
    {
        draft.AddMoney(money);
    }

    if (!items.empty())
    {
        for (int item_id : items)
        {
            if (Item* new_item = Item::CreateItem(item_id, 1))
            {
                new_item->SaveToDB(trans);
                draft.AddItem(new_item);
            }
            else
            {
                return false;
            }
        }
    }

    MailReceiver mailRec = nullptr;

    if (Player* player = ObjectAccessor::FindConnectedPlayer(player_guid))
    {
        MailReceiver mailRec(player);
    } else {
        MailReceiver mailRec(player_guid.GetCounter());
    }

    MailSender mailSend(MailMessageType::MAIL_CREATURE, 34337);

    draft.SendMailTo(trans, mailRec, mailSend, MAIL_CHECK_MASK_HAS_BODY, 0, 90);

    CharacterDatabase.CommitTransaction(trans);

    return true;
}

bool ArchipelagoMailer::tryGrantLevels(uint8 levels)
{   
    ObjectGuid& player_guid = Archipelago::GetInstance()->GetPlayerGuid();

    if (Player* player = ObjectAccessor::FindConnectedPlayer(player_guid))
    {
        uint8 maxLevel = 80;
        uint8 newLevel = std::min<uint8>(player->GetLevel() + levels, maxLevel);
        player->GiveLevel(newLevel);
        player->InitTalentForLevel();
        player->SetUInt32Value(PLAYER_XP, 0);
        return true;
    }
    else
    {
        return false;
    }
}

void ArchipelagoMailer::saveToFile()
{
    std::ofstream saveFile("ArchipelagoSave.txt");
    if (saveFile.is_open())
    {
        saveFile << std::to_string(Archipelago::GetInstance()->GetPlayerGuid().GetCounter()) << std::endl;
        saveFile << std::to_string(m_prog_armors_sent) << std::endl;
        saveFile << std::to_string(m_prog_weap_sent) << std::endl;
        saveFile.close();
    }
}

void AddSC_archipelago_mailerscript() { new ArchipelagoMailer(); }