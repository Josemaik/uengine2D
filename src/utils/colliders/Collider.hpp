#pragma once

#include "../Vec2.hpp"

struct Collider
{
	virtual ~Collider() = default; // Destructor virtual

	virtual bool collides(const Collider& other) const = 0;
	virtual bool collides(const vec2f& circlePos, float circleRadius) const = 0;
	virtual bool collides(const vec2f& rectPos, const vec2f& rectSize) const = 0;
	virtual bool collides(const vec2f& pixelPos, const vec2f& pixelsSize, const uint8_t* pixels) const = 0;

	// Nuevo método para actualizar posición del collider
	virtual void updatePosition(const vec2f& newPosition, const vec2f& newscale) = 0;
	virtual vec2f& getPosition() = 0;
};