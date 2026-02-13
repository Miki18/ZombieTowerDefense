#pragma once
#include "Monster.h"
class MonsterActive : public Monster
{
	public:
		MonsterActive(sf::Texture tex, float hp, float speed, int price, int TileSize, int& P_HP, int& P_Money, std::vector<sf::Vector2i>& starts, std::vector<PathPoints>& paths, int MonsterID, sf::Vector2f texSize, sf::Texture tex2, float Range, sf::Vector2f BulletPos, int Damage);
		~MonsterActive() {};
		bool IsShooter() override;
		void ResetCooldown(float cooldown);
		float GetRange();
		float GetCooldown();
		int GetDmg();
		sf::Vector2f getBulletStartPosition();

		void MonsterUpdate(sf::Time time);
		void LowerCooldown(sf::Time time);
		void ChangeAnimation(bool A) override;  //0 is shoot, 1 is walking
		void PlayAnimation(sf::Time time) override;
	private:
		float UntilNextTex = float(1.f / 7.f);   //because we have 7 frames per second
		int frame = 0;
		sf::Texture Attack_texture;
		float range;
		sf::Vector2f Bulletspawn;
		int Dmg;
		float Cooldown = 0.5f;
		bool MonsterState;
		void MonsterWalk(sf::Time time);
};