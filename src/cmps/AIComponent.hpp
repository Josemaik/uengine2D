#pragma once

struct AIComponent
{
	//ai propierties
	float detect_radius{ 10.f };
	bool playerdetected{ false };
	bool gotopatrolPoint{ false };
	vec2f patrol_position{};
	float MaxSpeed{ 0.2f };

	//Timer used by bees
	float countdown_throw{ 5.f };
	float elapsed_throw{ 1.f };
	//void plusDeltatime(float deltaTime, float& elapsed) { elapsed += deltaTime; };
	bool decreaseTimer(float deltaTime, float& elapsed) {
		elapsed += deltaTime;
		return elapsed >= countdown_throw ? true : false;
	}
};