#include "RenderComponent.hpp"

void RenderComponent::setPosition(const vec2f& _pos)
{
	position = _pos;
}
void RenderComponent::setScale(const vec2f& _scale)
{
	scale = _scale;
}
void RenderComponent::setColor(float r, float g, float b, float a)
{
	color.m_r = r;
	color.m_g = g;
	color.m_b = b;
	color.m_a = a;
}
void RenderComponent::setBlend(lblend_t _mode)
{
	blendmode = _mode;
}
void RenderComponent::setOrientation(float _angle)
{
	angle = _angle;
}
void RenderComponent::setSize(vec2f Size)
{
	width =  Size.m_x;
	height = Size.m_y;
}