#include "RectCollider.hpp"

#include "checkfunctions.hpp"

bool RectCollider::collides(const Collider& _other) const
{
	return _other.collides(rect_position,rect_Size);
}

bool RectCollider::collides(const vec2f& _circlePos, float _circleRadius) const
{
	return checkfunc::checkCircleRect(_circlePos, _circleRadius, rect_position, rect_Size);
}

bool RectCollider::collides(const vec2f& _rectPos, const vec2f& _rectSize) const
{
	return checkfunc::checkRectRect(rect_position, rect_Size, _rectPos, _rectSize);
}

bool RectCollider::collides(const vec2f& pixelPos, const vec2f& pixelsSize, const uint8_t* pixels) const
{
	return checkfunc::checkPixelsRect(pixelPos, pixelsSize, pixels, rect_position, rect_Size);
}