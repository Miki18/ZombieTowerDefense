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
#include "../Other/Bullet.h"

class GameState: public StateManager
{
	public:
		GameState();

	private:
		//Grass tiles - grass Tiles on which towers can stand
		//Road tiles - road; monsters are walking on them
		//Path - info about path for monsters; it contains list of points and their list of possible successors
		bool IsGamePaused = false;
		bool ShowMessage = false;
		bool PlayerWin = false;
		bool PlayerLose = false;
		int Health = 10;
		int Money = 100;

		//Map has 1600 x 900 size => 32 x 17 tiles
		const int MapSize[2] = { 32, 17 };   //last on is for UI

		std::vector<GrassTile> grass_tile;

		nlohmann::json Level;

		//Tower Options
		struct TowerOptions
		{
			bool IsVisible = false;
			sf::CircleShape circle;
		};
		TowerOptions tower_options;

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
			int price;
		};

		std::vector<std::unique_ptr<Monster>> monsters;
		std::vector<std::unique_ptr<MonsterTypeValues>> monster_types;
		double TimeUntilNextMonsterSpawns = 10;
		int MonsterID = 0;
		//Monsters waves variables
		struct MonsterWavesSettings
		{
			float MinimumMonstersInWave;
			float PossibleAdditionalMonsters;
			float IncreasingPossibleNumber;
			float IncreasingMinimumNumber;
			int MonsterNumberInCurrentWave;

			float timeCooldownInWave;
			float timeBetweenWaves;
			float CooldownInWave;
			float BetweenWaves;
		};
		float LevelTime;
		MonsterWavesSettings MWS;
		std::vector<std::string> speech;

		//Tower variables
		const int TowerTypes = 1;
		int SelectedTower = 0;
		int TowerID = 1;

		struct TowerTypeValues
		{
			float hp;
			float cooldown;
			float dmg;
			float radius;
			float price;
			float bulletoffset;
			sf::Texture base;
			sf::Texture top;
		};
		std::vector<std::unique_ptr<Tower>> towers;
		std::vector<std::unique_ptr<TowerTypeValues>> towersvalues;

		std::vector<Bullet> bullets;

		//Load functions
		void GenerateGrassTiles();
		void LoadRoadTiles();
		void LoadLevelData();
		void LoadObjects();
		void LoadPaths();
		void LoadMonsters();
		void LoadTowerTextures();
		void LoadSettings();

		void ExitFuntion();

		void Input(sf::RenderWindow& window, sf::Time time);
		void Update(sf::Time time);
		void Render(sf::RenderWindow& window);

		//UI
		void ShowHealtAndMoney();
		void SelectTowerUI();
		void ShowPauseMessage();

		//Towers
		void UpdateTowers(sf::Time time);
		void DetectEnemies(int tower_number);

		//Bullets
		void UpdateBullets(sf::Time time);
		void Bullet_MonsterCollision();
		void RemoveUnusedBullets();

		//Monster
		void UpdateMonsters(sf::Time time);
		void GenerateMonsters(sf::Time time);
		void MoveMonsters(sf::Time time);
};

