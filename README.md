# Archipelago Module
`mod-archipelago` is an [AzerothCore](https://www.azerothcore.org/) module that allows for participation in Archipelago Multiworld Randomizer games.

The Archipelago AP world can be found [here](https://github.com/GetBoofed/Archipelago).

Your task is to run dungeons for locations checks. Completing any dungeon and gaining the achievement for it is considered the check location, so this will require a new character.

This games Archipelago items:
- Progressive Levels (5 levels at a time)
- Progressive Armor (Level 10, 20, 30...)
- Progressive Weapons (Level 10, 20, 30...)
- Portable Hole (Additional bag space)
- 1000 Gold
- NOTE: All rewards, except levels, are sent via the in-game mail system. You must visit a mailbox to claim your items. (Azerothcore features a .mail command that can open your mailbox anywhere)

Location checks:
- All classic, outlands, outlands heroic, and northrend dungeon completion achievements.
- Optionally northrend heroic dungeons.
- NOTE: Any dungeon achievement that requires multiple wings will only give 1 check. (Ex. Scarlet Monastery or Dire Maul)

Completion Goal:
- Complete Halls of Reflection by Escaping the Lich King.
- Optionally, Heroic Halls of Reflection. Heroic Victory can be set in your Archipelago player options, and will add all northrend heroic dungeons to the location check pool.

## Important
Not all classes are available currently. Check [here](https://github.com/GetBoofed/mod-archipelago/blob/master/data/mail_items.json) to see which class/spec combo has items filled in. I will add more options later, or if you wish to contribute, please feel free.

You must start an new character at level 1. When you have your Archipelago client runnning and connected to the lobby your playing in, you MUST activate this module. This can be done by typing in the command .archipelago start (or .arch start). This will give the tell the game who the Archipelago rewards go to, level you up to 10 and mail you some starting gear.

I recommend using bots added via the `.playerbots bot addclass CLASSNAME` command, as they can re-rolled via the `.playerbots bot init=rare` command when you level up.

If you get the `entered too many instances recently` message, this can be configured in the Azerothcore `worldserver.conf` file at the line `AccountInstancesPerHour = 5`. Change the number then use the command `.reload config` to apply. No need to restart the server.

## Installation
Follow the installation guide for the [Playerbots Module](https://github.com/mod-playerbots/mod-playerbots).
Before building the server, clone this source code into the module folder:
```bash
cd azerothcore-wotlk/modules
git clone https://github.com/GetBoofed/mod-archipelago.git --branch=master
```

For more information, refer to the [AzerothCore Installation Guide](https://www.azerothcore.org/wiki/installation) and [Installing a Module](https://www.azerothcore.org/wiki/installing-a-module) pages.

## Contributing
I only program as a hobby with no formal training. If you see some awful code and know a better way to do it, please feel free to make pull request. If you have any errors or crashes, please open a github issue and be as detailed as possible. Include the names of any other modules you have installed as well.

## Acknowledgements
None of this would be possible without the brilliant people at both [AzerothCore](https://www.azerothcore.org/) and [Archipelago](https://archipelago.gg/) and their contributors.
