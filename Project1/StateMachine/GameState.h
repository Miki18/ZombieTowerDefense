//State "GAME"
#pragma once
#include "StateManager.h"
#include "../Tower/GrassTile.h"
#include "../Monsters/PathPoint.h"
#include "../Tower/Tower.h"
#include "../Tower/CannonTower.h"
#include "../Monsters/Monster.h"
#include "../Monsters/MonsterActive.h"
#include "../Monsters/MonsterPassive.h"

class GameState: public StateManager
{
	public:
		GameState();

	private:
		//Grass tiles - grass Tiles on which towers can stand
		//Road tiles - road; monsters are walking on them
		//Path - info about path for monsters; it contains list of points and their list of possible successors
		const int TowerTypes = 2;
		int SelectedTower = 0;
		int TowerID = 1;

		double TimeUntilNextMonsterSpawns = 10;

		std::vector<GrassTile> grass_tile;

		nlohmann::json Level;

		//Road
		std::vector<sf::Texture> RoadTextures;
		std::vector<sf::RectangleShape> RoadTiles;

		//Path
		std::vector<sf::Vector2i> paths_startpoints;
		std::vector<sf::Vector2i> paths_endpoints;
		std::vector<PathPoints> paths;

		//Monsters
		std::vector<std::unique_ptr<Monster>> monsters;
		std::vector<std::string> monster_types;
		std::vector<sf::Texture> monster_sheets;
		sf::Texture ZombieTex;
		//Monsters variables
		int MonsterNumberInWave = 5;
		float timeCooldownInWave = 1;
		float timeBetweenWaves = 5;

		//Tower texture
		sf::Texture cannon_base;
		sf::Texture cannon_top;
		std::vector<std::unique_ptr<Tower>> towers;

		//Map has 1600 x 900 size => 32 x 18 tiles
		const int MapSize[2] = { 32, 17 };   //last on is for UI
		const int TilesSize = 50;

		//Load functions
		void GenerateGrassTiles();
		void LoadRoadTiles();
		void LoadLevelData();
		void LoadObjects();
		void LoadPaths();
		void LoadMonsters();
		void LoadTowerTextures();
		void LoadSettings();

		void Input(sf::RenderWindow& window, sf::Time time);
		void Update(sf::Time time);
		void Render(sf::RenderWindow& window);

		//Update
		void UpdateMonsters(sf::Time time);
		void GenerateMonsters(sf::Time time);
		void MoveMonsters(sf::Time time);
};

