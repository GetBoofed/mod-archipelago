#pragma once
#ifndef ARCHIPELAGO_H
#define ARCHIPELAGO_H

#include "Player.h"
#include "ScriptMgr.h"
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

class Archipelago : public PlayerScript
{
public:
    Archipelago();
    ~Archipelago() = default;

    static Archipelago* GetInstance() { return instance; }
    ObjectGuid& GetPlayerGuid() { return m_player_guid; }

    void OnPlayerAchievementComplete(Player* player, AchievementEntry const* achievement) override;
    void sendHandshake(Player* player);
    void tryResync(Player*);

private:
    bool enabled;
    boost::asio::io_context m_io_context;
    boost::asio::steady_timer m_timer;
    static Archipelago* instance;
    ObjectGuid m_player_guid;

    void sendCheck(int ach_id);
    void trySendMessage(std::string message);
    void retryConnection(std::string message);
};

#endif