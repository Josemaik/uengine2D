#pragma once

#include "Collider.hpp"

struct PixelsCollider : public Collider
{
	//Constructor
	PixelsCollider(const vec2f& _pixels_size, const vec2f& _pivot, const uint8_t* _pixels)
		:pixels_size(_pixels_size), sprite_pivot(_pivot), pixels(_pixels)
	{
	}

	~PixelsCollider() override
	{
		printf("PixelsCollider se ha destruido\n");
		delete[] pixels;
	}

	virtual bool collides(const Collider& other) const override;
	virtual bool collides(const vec2f& circlePos, float circleRadius) const override;
	virtual bool collides(const vec2f& rectPos, const vec2f& rectSize) const override;
	virtual bool collides(const vec2f& pixelPos, const vec2f& pixelsSize, const uint8_t* pixels) const override;

	void updatePosition(const vec2f& newPosition, const vec2f& newscale) override
	{
		//Actualizamos posicion con respecto al tamaño y pivote
		pixels_pos = newPosition - (pixels_size * sprite_pivot * newscale);
	}
	vec2f& getPosition() override
	{
		return pixels_pos;
	}
private:
	vec2f pixels_pos{};
	const vec2f pixels_size{}; //sprite size
	const uint8_t* pixels{nullptr};
	const vec2f sprite_pivot{};
};