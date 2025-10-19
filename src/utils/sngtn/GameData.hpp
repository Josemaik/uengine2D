#pragma once
#include <random>
#include "../Vec2.hpp"

struct GameData
{
	double mouse_x{};
	double mouse_y{};

	int coins_count{};

	//Ancho y Alto del nivel
	vec2f level_Size{0.f,0.f};
	//Player
	vec2f playerPos{ 0.f,0.f };
	float playerDirection{ 1.f };
	//Score
	int score{ 0 };
	//Game State
	bool lose{ false };
	bool victory{ false };
	//spawn pos
	std::vector<vec2f> spawnpositions{ vec2f{ 30.f,241.f},vec2f{1348.f,401.f },vec2f{3046.f,401.f} };
	vec2f current_spawnpos{ 30.f,241.f };
	void SetSpawnPos(vec2f& playerpos)
	{
		for (auto& spawn : spawnpositions)
		{
			float currentdist = 100000.f;
			float dist = playerpos.distance(spawn);
			if (spawn.m_x < playerpos.m_x && dist < currentdist)
			{
				currentdist = dist;
				current_spawnpos = spawn;
			}
		}
	}

	//Global Timer
	float countdown_spawn{ 5.f };
	float elapsed_spawn{ 1.f };
	//void plusDeltatime(float deltaTime, float& elapsed) { elapsed += deltaTime; };
	bool decreaseTimer(float deltaTime, float& elapsed) {
		elapsed += deltaTime;
		return elapsed >= countdown_spawn ? true : false;
	}

	//random function
	float GetRandomFloat(float min, float max)
	{
		// Crear el generador de números aleatorios
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(min, max);
		// Generar el número aleatorio
		return dist(gen);
	}
};