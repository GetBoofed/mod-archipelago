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
        const std::string STARTER_ITEMS = "Starter Items";
        const std::string STARTER_LEVELS = "Starter Levels";
        const std::string SYNC = "Sync";

        const std::map<uint8,std::vector<uint32>> PROG_ARMOR_MAP = {
            //   head   neck   shldr  cape   chest  wrist  hand   waist   leg   boot   ring1  trinket1  
            {0,{ /**/   /**/   /**/   5111,  6195,  /**/   2980,  832,   2978,  3302,  8350,  /**/}},

            {1,{ 1282,  30419, 3231,  6314,  6642,  5943,  12994, 6460,  15470, 14742, 1076,  21568}},

            {2,{ 3836,  13084, 3841,  14593, 7688,  14759, 14764, 14761, 3843,  13124, 34227, /**/}},

            {3,{ 10763, 21755, 7928,  20219, 10775, 9432,  13071, 13145, 7926,  14549, 13095, 21567}},

            {4,{ 22223, 15799, 20517, 11677, 11678, 12550, 8314,  11703, 17711, 22270, 12548, 17759}},

            {5,{ 16963, 22732, 16961, 21710, 16966, 16959, 16964, 16960, 16962, 16965, 19432, 23040}},

            {6,{ 30974, 34178, 30980, 34190, 30976, 34442, 30970, 34547, 30978, 34568, 34837, 34427}},

            {7,{ 51221, 50682, 51224, 50718, 51220, 50611, 51222, 50691, 51223, 54579, 50622, 50349}},
        };
        const std::map<uint8,std::vector<uint32>> PROG_WEAP_MAP = {
            //  weap   shield  ring2   trink2  ranged
            {0,{826,   3651,   8350,   /**/    /**/}},
            {1,{1292,  6320,   12054,  /**/    /**/}},
            {2,{6692,  4975,   7686,   /**/    /**/}},
            {3,{2815,  7726,   9642,   744     /**/}},
            {4,{11702, 1203,   15855,  11811   /**/}},
            {5,{22816, 23043,  21393,  18815   /**/}},
            {6,{32254, 34185,  30083,  34428   /**/}},
            {7,{51947, 50729,  50642,  54591   /**/}},
        };
        const std::vector<uint32> PORT_HOLE_VEC = {51809};

        bool sendMail(const uint32 money, const std::vector<uint32>& items);
        bool tryGrantLevels(uint8 levels);
        void saveToFile();

};

void AddSC_archipelago_mailerscript();

#endif