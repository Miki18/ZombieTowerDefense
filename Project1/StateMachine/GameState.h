//State "GAME"
#pragma once
#include "StateManager.h"
#include "../Other/GrassTile.h"
#include "../Monsters/PathPoint.h"
#include "../Tower/Tower.h"
#include "../Tower/CannonTower.h"
#include "../Tower/SniperTower.h"
#include "../Tower/Goldmine.h"
#include "../Tower/LaserTower.h"
#include "../Tower/DoubleTower.h"
#include "../Tower/BigTower.h"
#include "../Tower/MortarTower.h"
#include "../Tower/Ultimate.h"
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
		bool AreYouSure = false;
		bool IsMessageVisible = false;
		bool IsPlayerWin = false;
		bool IsPlayerLose = false;
		bool PlayerSurr = false;
		int Health = 10;
		int Money = 0;
		float LastWaveInfo = 0;

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
			int dmg = 0;
			int health = 0;
			int radius = 0;
			float cooldown = 0;
		};
		TowerOptions tower_options;

		//Road
		struct RoadTileData
		{
			sf::RectangleShape shape;
			bool IsBridge;
		};
		sf::Texture BridgeTex;
		sf::Texture GoldBarTex;
		std::vector<sf::Texture> RoadTextures;
		std::vector<RoadTileData> RoadTiles;
		std::vector<sf::CircleShape> BlackHole;
		std::vector<sf::Sprite> RedArrows;
		std::vector<sf::Sprite> GoldBars;

		//Path
		std::vector<sf::Vector2i> paths_startpoints;
		std::vector<PathPoints> paths;

		//Monsters
		struct MonsterTypeValues
		{
			std::string name;
			bool IsPassive;
			sf::Texture MonsterTex;
			float hp;
			float Speed;
			int price;
			sf::Vector2f texSize;
			sf::Texture MonsterTexAttack;
			float range;
			sf::Vector2f bulletoffset;
			float bulletspeed;
			int Dmg;
		};

		std::vector<std::unique_ptr<Monster>> monsters;
		std::vector<std::unique_ptr<MonsterTypeValues>> monster_types;
		int MonsterID = 0;
		int Stats[7] = {};
		//Monsters waves variables
		struct MonsterWavesSettings
		{
			float PreWaveWaiting = 0.0f;
			float Cooldown = 0.0f;
			float BaseCooldown = 0.0f;
			std::vector<int> monsters;
		};
		std::vector<MonsterWavesSettings> MWS;
		int wave_number = 0;
		std::vector<std::string> speech;

		//Tower variables
		int SelectedTower = 0;
		int TowerID = 1;

		struct TowerTypeValues
		{
			std::string name;

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

		sf::Texture RuinsTex;
		sf::Sprite RuinsSprite;

		std::vector<Bullet> bullets;

		//preview
		struct Preview
		{
			bool ShowPreview = false;
			sf::CircleShape Small;
			sf::CircleShape Big;
			sf::Vector2i Tile;
		};
		Preview preview;

		sf::RectangleShape bottom_cover;

		//Sounds
		sf::SoundBuffer TowerSound;
		sf::SoundBuffer MonsterSound;
		sf::SoundBuffer LastWaveSound;
		sf::Sound LastWaveS;
		sf::Music Music;

		//Load functions
		void GenerateGrassTiles();
		void LoadRoadTiles();
		void LoadLevelData();
		void LoadPaths();
		void LoadMonsters();
		void LoadTowerTextures();
		void LoadSettings();
		void LoadMonsterWaves();
		void LoadGold();
		void LoadGoldTexture();
		void LoadSounds();
		void SaveProgress(bool IsSuccess);

		void ExitFuntion();

		void Input(sf::RenderWindow& window, sf::Time time);
		void Update(sf::RenderWindow& window, sf::Time time);
		void Render(sf::RenderWindow& window);

		void RemoveBridge(int index);

		//UI
		void ShowHealtAndMoney();
		void SelectTowerUI();
		void TowerUI(int towersindex);
		void ShowMessage();
		void CalculatePreview();
		void UpdatePreview();
		void CustomTextMessages(std::string UpperText, std::string LowerText, sf::Vector2f picSize);

		//Towers
		void UpdateTowers(sf::Time time);
		void DetectEnemies(int tower_number);
		template <typename T>
		void PlaceTower(int ind, int Tile);

		//Bullets
		void UpdateBullets(sf::Time time);
		void Bullet_TowersCollision();
		void Bullet_MonstersCollision();
		void RemoveUnusedBullets();

		//Monster
		void UpdateMonsters(sf::Time time);
		void GenerateMonsters(sf::Time time);
		bool MonsterShoot(int monster_index);
};

