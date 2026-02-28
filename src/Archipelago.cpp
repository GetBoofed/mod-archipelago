/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AchievementMgr.h"
#include "Archipelago.h"
#include "Archipelago_command.h"
#include "Archipelago_mailer.h"
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <string>
#include <iostream>
#include <fstream>

using boost::asio::ip::tcp;

Archipelago* Archipelago::instance = nullptr;

Archipelago::Archipelago():
    PlayerScript("Archipelago"),
    m_io_context(),
    m_timer(m_io_context)
    {
        instance = this;
        enabled = sConfigMgr->GetOption<bool>("Archipelago.Enable", false);
    }

void Archipelago::OnPlayerAchievementComplete(Player* player, AchievementEntry const* achievement)
{
    if (!enabled) { return; }
    if (player->GetSession()->IsBot()) { return; }
    if (player->GetGUID() != m_player_guid) { return; }
    sendCheck(achievement->ID);
}

void Archipelago::sendHandshake(Player* player)
{
    if (player)
    {
        m_player_guid = player->GetGUID();
        std::string guid = std::to_string(m_player_guid.GetCounter());
        std::string message = "HANDSHAKE:" + guid;

        tryResync(player);

        trySendMessage(message);
    }
}

void Archipelago::tryResync(Player* player)
{
    std::ifstream saveFile("ArchipelagoSave.txt");

    if (saveFile.is_open())
    {
        std::string line = "";
        std::vector<std::string> lines = {};

        while (getline(saveFile,line))
        {
            lines.push_back(line);
        }
        saveFile.close();

        ObjectGuid::LowType _guid(std::stoi(lines[0]));

        if (m_player_guid.GetCounter() == _guid)
        {
            ArchipelagoMailer::GetInstance()->setFromLoad(std::stoi(lines[1]), std::stoi(lines[2]));
        }
    }

    if (player->GetLevel() == 1)
    {
        ArchipelagoMailer::GetInstance()->sendStarters();
    }
}

void Archipelago::sendCheck(int ach_id)
{
    std::string message = "CHECK:" + std::to_string(m_player_guid.GetCounter()) + ":" + std::to_string(ach_id);
    trySendMessage(message);
}

void Archipelago::trySendMessage(std::string message)
{
    tcp::resolver resolver(m_io_context);
    tcp::socket socket(m_io_context);

    try
    {
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "5678"));
        try
        {
            boost::asio::write(socket, boost::asio::buffer(message));
        }
        catch(const boost::system::error_code& ec)
        {
            retryConnection(message);
        }
                
    }
    catch (const boost::system::system_error& se)
    {
        retryConnection(message);
    }
}

void Archipelago::retryConnection(std::string message)
{
    m_timer.expires_after(boost::asio::chrono::seconds(5));
    m_timer.async_wait([this, message] (const boost::system::error_code& ec)
    {
        if (!ec)
        {
            trySendMessage(message);
        }
    });
}

// Add all scripts in one
void AddArchipelagoScripts()
{
    new Archipelago();
    AddSC_archipelago_commandscript();
    AddSC_archipelago_mailerscript();
}