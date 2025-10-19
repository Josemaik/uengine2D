#pragma once

#include "../utils/Vec2.hpp"

struct PhysicsComponent
{
	//getters and setters
	float getAngle() const { return angle; };
	const vec2f& getPivot() const { return pivot; }
	void setAngle(float _angle) { angle = _angle; }
	void setPivot(const vec2f& _pivot) { pivot = _pivot; }

	bool moving{ false };
	bool jumping{ false };
	bool isInAir{ false };
	int jumpTimer{};
	float startjump_y{};
	float jumpImpulse{-100.f};

	bool onground{ false };

	vec2f prevPosition{ 0.f,0.f };
	vec2f position{ 0,0 };
	vec2f scale{ 0,0 };
	vec2f pivot{ 0.0,0.0 };//pivote de rotación
	vec2f size{ 0.f,0.f };

	vec2f vel{ 0.f,0.f };
	float speed{0.5f};

	float gravity{ 200.f };

	float scale_velocity{};

	float angular_velocity{ 42.f };
	float angle{};
	float direction_rot{};
	float direction_scale{1.f};
};