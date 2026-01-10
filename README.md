# \# ZombieTowerDefense

# 

# Current version: 1.2.2c

# 

# \# About the Game

# 

# Simple tower defense game. Player has to place towers (and upgrades them) to defeat zombies.<br>

# It was made with cpp using sfml and ImGui. All textures was made in blender.<br>

# Game is still in development.

# 

# \# Content

# 

# \*\*Game contains\*\*:

# &nbsp; - 2 levels

# &nbsp; - 2 towers

# &nbsp; - one monster - zombie

# &nbsp; - remove bridge mechanic

# 

# \*\*Towers\*\*:

# &nbsp; - Cannon Tower - basic tower with mid range and damage

# &nbsp; - Sniper Tower - high range, but low damage and long cooldown. Good tower to place where Cannon would be useless.

# Each tower can be upgraded up to 5th level.

# 

# \*\*Remove bridge\*\* is an unique mechanic that allows player to remove road and forces monsters to choose longer path. Player can remove only bridges and it costs 100 gold.

# 

# \# Requirements

# 

# Game is available only for windows 10/11.<br>

# Minimum screen resolution: 1600x900<br>

# Storage: 12MB<br>

# 

# \# Controls

# 

# LMB - select/place tower<br>

# RMB - deselect tower<br>

# P - Pause<br>

# 

# \# Project Structure (\& technical info)

# 

# Project bases on game loop and on state machine with 2 states:

# &nbsp; - Menu

# &nbsp; - Game

# 

# In folder:

# &nbsp; - Monsters - there are files with monster class and its 2 subclasses,

# &nbsp; - Towers - there are files with Tower class and its subclasses - each type of tower has its own subclass,

# &nbsp; - State Machine - there are files with state machine and 2 states - game state and menu state,

# &nbsp; - Resources - it has 2 subfolders

# &nbsp;   - Content - there are files with json data for tower, monsters, players profiles etc,

# &nbsp;   - Visuals - it contains all textures for UI, towers, monsters etc,

# &nbsp; - Others - it has other files that not fit with other folders

# Every folder can has additional files related to its main content.

# 

# \# Download game

# 

# Game is available to download here: https://miki18games.itch.io/monster-tower-defense

# 

