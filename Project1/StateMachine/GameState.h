//State "GAME"
#pragma once
#include "StateManager.h"
#include "../Tower/Tower.h"
#include "../Tower/CannonTower.h"

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

		struct GrassTile
		{
			sf::Vector2i Position;
			int TowerID = 0;   // 0 means tile is empty
			bool IsDarker = false;
		};
		std::vector<GrassTile> grass_tile;

		nlohmann::json Level;

		//Road
		std::vector<sf::Texture> RoadTextures;
		std::vector<sf::RectangleShape> RoadTiles;

		//Monsters
		std::vector<std::string> monster_types;

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

		void Input(sf::RenderWindow& window, sf::Clock& DeltaClock);
		void Update(sf::Clock& DeltaClock);
		void Render(sf::RenderWindow& window);
};

