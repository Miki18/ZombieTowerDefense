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
		//Tower tiles - tiles on grass on which towers can stand
		//Road tiles - road; monsters are walking on them
		//Path - info about path for monsters; it contains list of points and their list of possible successors
		const int TowerTypes = 2;
		int SelectedTower = 0;
		int TowerID = 1;

		struct TowerTile
		{
			sf::Vector2i Position;
			int TowerID = 0;   // 0 means tile is empty
			bool IsDarker = false;
		};
		std::vector<TowerTile> tower_tile;

		nlohmann::json Level;

		//Road
		std::vector<sf::Texture> RoadTextures;
		std::vector<sf::RectangleShape> RoadTiles;

		//Monsters
		std::vector<std::string> monster_types;

		//Tower texture
		sf::Texture cannon_base;
		sf::Texture cannon_top;
		std::vector<std::unique_ptr<Tower>> towers;   //check if shared would be better later

		//Map has 1600 x 900 size => 32 x 18 tiles
		const int MapSize[2] = { 32, 17 };   //last on is for UI
		const int TilesSize = 50;

		void Input(sf::RenderWindow& window, sf::Clock& DeltaClock);
		void Update(sf::Clock& DeltaClock);
		void Render(sf::RenderWindow& window);
};

