#pragma once
#ifndef ARCHIPELAGO_MAILER_H
#define ARCHIPELAGO_MAILER_H

#include "server.hpp"

class ArchipelagoMailer : public WorldScript
{
    public:
        ArchipelagoMailer();
        ~ArchipelagoMailer() = default;

        void OnStartup() override;
        void OnUpdate(uint32) override;
        void OnShutdown() override;
        void handleMessage(std::vector<std::string>*, bool);
        void sendStarters();
        void setFromLoad(uint8, uint8);
        bool loadJsonData();
        static ArchipelagoMailer* GetInstance() { return instance; }
    
    private:
        static ArchipelagoMailer* instance;
        bool enabled;
        uint32 update_time = 0;

        std::unique_ptr<server> serv;
        std::unique_ptr<std::thread> server_thread;
        boost::asio::io_context io_context;
        std::mutex data_mutex;
        std::vector<std::string> unsent_messages = {};

        uint8 m_prog_armors_sent = 0;
        uint8 m_prog_weap_sent = 0;
        const short PORT = 5679;
        const std::string PROG_LEVEL = "Progressive Levels";
        const std::string PROG_ARMOR = "Progressive Armor";
        const std::string PROG_WEAP = "Progressive Weapon";
        const std::string PORT_HOLE = "Portable Hole";
        const std::string GOLD = "1000 Gold";
        std::string m_class = "warrior";
        std::string m_spec = "prot";

        std::map<uint8,std::vector<uint32>> m_prog_armor_map = {};
        std::map<uint8,std::vector<uint32>> m_prog_weap_map = {};
        const std::vector<uint32> PORT_HOLE_VEC = {51809};

        bool sendMail(const uint32 money, const std::vector<uint32>& items);
        bool tryGrantLevels(uint8 levels);
        void saveToFile();
        void matchClassSpec(std::string);

};

void AddSC_archipelago_mailerscript();

#endif