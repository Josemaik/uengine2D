#pragma once

#include "../utils/Vec4.hpp"
#include "../utils/Vec2.hpp"
#include <litegfx.h>

struct RenderComponent
{
	//getters
	const vec2f& getPosition() const { return position; }
	const vec2f& getScale() const { return scale; }
	float getRed() const { return color.m_r; }
	float getGreen() const { return color.m_g; }
	float getBlue() const { return color.m_b; }
	float getAlpha() const { return color.m_a; } 
	//const ltex_t* getText() const { return texture; }
	lblend_t getBlend() const { return blendmode; }
	//frame multiplicado por escala
	vec2f getSize() const 
	{ 
		/*return vec2f{ static_cast<float>(texture->width), static_cast<float>(texture->height) }*/;//multiplicar por escala
		return vec2f{ width,height };
	}
	vec2f getPivot() { return pivot; }

	//setters
	void setPosition(const vec2f& pos);
	void setScale(const vec2f& scale);
	void setColor(float r, float g, float b, float a);
	//void setTexture(const  ltex_t* _newtext);
	void setBlend(lblend_t _mode);
	void setOrientation(float _angle);
	void setSize(vec2f Size);

	vec2f position{0,0};
	vec2f scale{0,0};
	vec4f color{ 0.0f, 0.0f, 0.0f, 0.0f };
	float angle{0.f};
	//Textura
	//const ltex_t* texture;
	uint8_t fontype{ 0 };
	lblend_t blendmode{};
	float width{}, height{};
	// Coordenadas UV
	float u0{0.f}, v0{0.f}, u1{0.f}, v1{0.f};
	//pivote
	vec2f pivot{ 0.f,0.f };
	//Flags
	bool drawcollision{ false };
};