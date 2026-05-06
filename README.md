# About the Game

__Monster Tower Defense__ is a simple tower defense game. Player's goal is to defeat monsters by placing and upgrading towers.<br>
_The game is still in development - current version: 1.5.0._

WARNING: Description is outdated!

## Game contains
  - 6 levels
  - 6 towers
  - 4 monsters
  - some unique mechanics

## Towers
Player can build 6 different towers. Each towers can be upgraded up to 5th level. When tower is selected, player can:
 - Sell it
 - Upgrade
 - Repair

There are 6 towers available:
 - Cannon tower,
 - Sniper tower,
 - Laser tower,
 - Double tower,
 - Big tower,
 - Goldmine

Goldmine is a specific type of tower - instead shooting, it generates gold per second.

## Monsters
In the game, the player can encounter 2 types of monsters:
- Passive - they walk from start to end
- Active - when they spot a tower, they stop and attack it until it is destroyed
There are 4 monsters available:
- zombie - passive monster
- worm - active monster
- fast zombie - passive monster
- golem - passive monster

## Levels
The game has 6 unique levels (3 tutorial + 3 levels). To unlock level player has to complete tutorials.

## Mechanics
The game contains 4 interesting mechanics:
1. Active monsters - some monsters can shoot and will try to destroy player's tower
2. Gold System - some levels contain gold tiles that allow player to place a goldmine - special "tower" that generates gold instead of shooting to monsters. When monster destroys goldmine, the ruins appear  . If player wants to build something on that tile again, the player must remove the ruins first.
3. Brigdes - bridges are a road segments that player can remove to force monsters to take longer path
4. Preview - player can see tower's range before placing it

# Requirements

Game is available only for windows 10/11.<br>
Minimum screen resolution: 1600x900<br>
Storage: 20MB<br>

# Controls

LMB - select/place tower<br>
RMB - deselect tower<br>
P - Pause<br>
Q - Show preview<br>

# Project Structure (& technical info)

Game was made with:
- C++ 17
- SFML3 (C++ library)
- ImGui (C++ library)
- Blender - used for creating sprites

## Data Structure
All data are stored in 'Resources' folder.

Program stores:
- Monsters stats,
- Towers stats,
- Level data,
- Players' profiles
as json files in 'Resources/Content'. They are loaded from and saved to JSON files using the nlohmann library.

The program also includes .png sprite assets, which are used by the following game objects:
- Towers,
- Roads,
- Monsters,
- UI,
- Bridges,
- Gold Tiles
They are located in 'Resources/Visuals' and are handled using sfml library.

There are also .wav and .mp3 assets stored in 'Resources/Sounds' and the program loads them using sfml library.

## Programming structure
Main class is State Machine class. It has State Manager class with 2 subclass:
- Main Menu
- Game
The State Machine is responsible for managing variables that need to be shared between the game state and the menu state.

The most important classes:
- Tower.h
- Monster.h
- GrassTile.h

Tower class has as many subclass as there are tower types. Each tower has its own subclass.

Monster has 2 subclasses: MonsterActive and MonsterPassive. When new monster is loaded, system check whether it is passive or active and creates appropriate class.

GrassTile.h is a structure that contains info about tile. They serve as the foundation of the entire tower‑placement system.

# Links

Full details about game content can be found on [Wiki](https://monster-tower-defense.fandom.com/wiki/Monster_Tower_Defense_Wiki)<br>
Game is available to download on [itchio](https://miki18games.itch.io/monster-tower-defense)
