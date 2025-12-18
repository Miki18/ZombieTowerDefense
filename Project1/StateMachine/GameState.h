//State "GAME"
#pragma once
#include "StateManager.h"
#include "../Other/GrassTile.h"
#include "../Monsters/PathPoint.h"
#include "../Tower/Tower.h"
#include "../Tower/CannonTower.h"
#include "../Tower/SniperTower.h"
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
		bool IsMessageVisible = false;
		bool IsPlayerWin = false;
		bool IsPlayerLose = false;
		int Health = 10;
		int Money = 1000;

		int price_offset = 5;

		//Map has 1600 x 900 size => 32 x 17 tiles
		const int MapSize[2] = { 32, 17 };   //last on is for UI

		std::vector<GrassTile> grass_tile;

		nlohmann::json Level;

		//Tower Options
		struct TowerOptions
		{
			bool IsVisible = false;
			int SelectedTowerID = 0;
			sf::CircleShape circle;
		};
		TowerOptions tower_options;

		//Road
		struct RoadTileData
		{
			sf::RectangleShape shape;
			bool IsBridge;
		};
		sf::Texture BridgeTex;
		std::vector<sf::Texture> RoadTextures;
		std::vector<RoadTileData> RoadTiles;
		std::vector<sf::CircleShape> BlackHole;
		std::vector<sf::Sprite> RedArrows;

		//Path
		std::vector<sf::Vector2i> paths_startpoints;
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
		int MonsterID = 0;
		//Monsters waves variables
		struct MonsterWavesSettings
		{
			float MinimumMonstersInWave;  //min number of monsters in this wave
			float PossibleAdditionalMonsters;  //MWS.MonsterNumberInCurrentWave = MWS.MinimumMonstersInWave + rand() % int(MWS.PossibleAdditionalMonsters);
			float IncreasingPossibleNumber;  //MWS.PossibleAdditionalMonsters += MWS.IncreasingPossibleNumber;
			float IncreasingMinimumNumber;   //MWS.MinimumMonstersInWave += MWS.IncreasingMinimumNumber;
			int MonsterNumberInCurrentWave;  //Number of monsters in current wave to spawn (if this == 0 then a new waves starts)

			//To check cooldown substract delta time from time... variable
			float timeCooldownInWave; //time between monsters in single wave 
			float timeBetweenWaves; //time between waves 
			float CooldownInWave; //store cooldown in wave, so I can later timeCooldownInWave = CooldownInWave
			float BetweenWaves; //store time between waves, so I can later timeBetweenWaves = BetweenWaves
		};
		float LevelTime;
		MonsterWavesSettings MWS;
		std::vector<std::string> speech;

		//Tower variables
		const int TowerTypes = 2;
		int SelectedTower = 0;
		int TowerID = 1;

		struct TowerTypeValues
		{
			float hp;
			float IncreaseHp;

			float cooldown;
			float IncreaseCooldown;

			float dmg;
			float IncreaseDmg;

			float radius;
			float IncreaseRadius;

			int price;
			float UpgradePrice;
			float IncreaseUpgradePrice;

			float bulletoffset;
			float bulletspeed;
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
		void LoadPaths();
		void LoadMonsters();
		void LoadTowerTextures();
		void LoadSettings();

		void ExitFuntion();

		void Input(sf::RenderWindow& window, sf::Time time);
		void Update(sf::Time time);
		void Render(sf::RenderWindow& window);

		void RemoveBridge(int index);

		//UI
		void ShowHealtAndMoney();
		void SelectTowerUI();
		void TowerUI(int towersindex);
		void ShowMessage();
		void CustomTextMessages(std::string UpperText, std::string LowerText, sf::Vector2f picSize);

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

