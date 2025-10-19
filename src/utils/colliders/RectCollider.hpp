#pragma once

#include "Collider.hpp"

struct RectCollider : public Collider
{
	RectCollider(const vec2f& pivot,const vec2f& size, const vec2f& spritePos)
		:rect_position(spritePos), rect_pivot(pivot), rect_original_size(size),rect_Size(size)
	{
		printf("RectCollider creado\n");
	}
	~RectCollider() override
	{
		printf("RectCollider destruido\n");
	}
	virtual bool collides(const Collider& other) const override;
	virtual bool collides(const vec2f& circlePos, float circleRadius) const override;
	virtual bool collides(const vec2f& rectPos, const vec2f& rectSize) const override;
	virtual bool collides(const vec2f& pixelPos, const vec2f& pixelsSize, const uint8_t* pixels) const override;

	void updatePosition(const vec2f& newPosition, const vec2f& newscale) override
	{
		//Actualizar posición con respecto al pivote ,escala y size
		rect_position = newPosition - (rect_original_size * rect_pivot * newscale);
		// Actualizar el tamaño escalado del rectángulo
		rect_Size = rect_original_size * newscale;
	}
	vec2f& getPosition() override
	{
		return rect_position;
	}
private:
	vec2f rect_position{};
	vec2f rect_Size{};
	const vec2f rect_pivot{};
	const vec2f rect_original_size{};
};