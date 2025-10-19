#pragma once

#include "Collider.hpp"

struct CircleCollider : public Collider
{
	CircleCollider(float _radius)
		:original_radius(_radius)
	{
		printf("CircleCollider creado\n");
	}
	~CircleCollider() override
	{
		printf("CircleCollider destruido\n");
	}
	virtual bool collides(const Collider& other) const override;
	virtual bool collides(const vec2f& circlePos, float circleRadius) const override;
	virtual bool collides(const vec2f& rectPos, const vec2f& rectSize) const override;
	virtual bool collides(const vec2f& pixelPos, const vec2f& pixelsSize, const uint8_t* pixels) const override;

	void updatePosition(const vec2f& newPosition, const vec2f& newscale) override
	{
		//Actualizamos posición
		circle_position = newPosition;
		//Actualizamos radio con respecto a la escala
		float scaleFactor = newscale.m_x;
		circle_radius = original_radius * scaleFactor;
	}
	vec2f& getPosition() override
	{
		return circle_position;
	}
private:
	vec2f circle_position{};
	float circle_radius{0.f};
	const float original_radius{};
};