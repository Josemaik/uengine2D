#pragma once

#include "../utils/Vec2.hpp"

struct ScrollComponent
{
public:
	/*ScrollComponent(float _scrollRatio, vec2f _scrollSpeed)
		:scrollRatio(_scrollRatio), scrollSpeed(_scrollSpeed)
	{}*/
	//getters
	float getScrollRatio() const { return scrollRatio;}
	const vec2f& getScrollSpeed() const { return scrollSpeed; }
	const vec2f& getscrollOffset() const { return scrollOffset; }
	//setters
	void setScrollRatio(float ratio)
	{
		scrollRatio = ratio;
	}
	void setScrollSpeed(const vec2f& speed)
	{
		scrollSpeed = speed;
	}


	float scrollRatio{};
	vec2f scrollSpeed{0.f,0.f};
	vec2f scrollOffset{ 0.f,0.f };
};

