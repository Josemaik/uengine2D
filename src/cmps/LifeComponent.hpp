#pragma once

struct LifeComponent
{
	//timer to die
	float countdown_die{ 5.f };
	float elapsed_die{ 1.f };
	//void plusDeltatime(float deltaTime, float& elapsed) { elapsed += deltaTime; };
	bool decreaseTimer(float deltaTime, float& elapsed) {
		elapsed += deltaTime;
		return elapsed >= countdown_die ? true : false;
	}

	bool dead{ false };

	int Lifes{ 1 };
};