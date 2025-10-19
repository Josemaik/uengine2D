#include "CircleCollider.hpp"

#include "checkfunctions.hpp"

bool CircleCollider::collides(const Collider& _other) const
{
	return _other.collides(circle_position,circle_radius);
}

bool CircleCollider::collides(const vec2f& _circlePos, float _circleRadius) const
{
	return checkfunc::checkCircleCircle(circle_position, circle_radius, _circlePos, _circleRadius);
}

bool CircleCollider::collides(const vec2f& _rectPos, const vec2f& _rectSize) const
{
	return checkfunc::checkCircleRect(circle_position, circle_radius, _rectPos, _rectSize);
}

bool CircleCollider::collides(const vec2f& pixelPos, const vec2f& pixelsSize, const uint8_t* pixels) const
{
	return checkfunc::checkCirclePixels(circle_position, circle_radius, pixelPos, pixelsSize, pixels);
}