//State "GAME"
#pragma once
#include "StateManager.h"
#include "../Other/GrassTile.h"
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
		const int TowerTypes = 1;
		int SelectedTower = 0;
		int TowerID = 1;

		bool IsGamePaused = false;
		int Health = 5;
		int Money = 100;

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
		struct MonsterTypeValues
		{
			bool IsPassive;
			sf::Texture MonsterTex;
			float hp;
			float Speed;			
		};

		std::vector<std::unique_ptr<Monster>> monsters;
		std::vector<std::unique_ptr<MonsterTypeValues>> monster_types;
		double TimeUntilNextMonsterSpawns = 10;
		int MonsterID = 0;
		//Monsters variables
		int MonsterNumberInWave = 5;
		float timeCooldownInWave = 1;
		float timeBetweenWaves = 5;

		//Tower texture
		struct TowerTypeValues
		{
			float hp;
			float cooldown;
			float dmg;
			float radius;
			float price;
			sf::Vector2f bulletpoint;
			sf::Texture base;
			sf::Texture top;
		};
		std::vector<std::unique_ptr<Tower>> towers;
		std::vector<std::unique_ptr<TowerTypeValues>> towersvalues;

		//Map has 1600 x 900 size => 32 x 18 tiles
		const int MapSize[2] = { 32, 17 };   //last on is for UI

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

		//UI
		void ShowHealtAndMoney();
		void SelectTowerUI();

		//Monster
		void UpdateMonsters(sf::Time time);
		void GenerateMonsters(sf::Time time);
		void MoveMonsters(sf::Time time);
};

