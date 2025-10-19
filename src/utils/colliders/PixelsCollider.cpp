#include "PixelsCollider.hpp"

#include "checkfunctions.hpp"


bool PixelsCollider::collides(const Collider& _other) const
{
	return _other.collides(pixels_pos, pixels_size,pixels);
}

bool PixelsCollider::collides(const vec2f& _circlePos, float _circleRadius) const
{
	return checkfunc::checkCirclePixels(_circlePos, _circleRadius, pixels_pos, pixels_size,pixels);
}

bool PixelsCollider::collides(const vec2f& _rectPos, const vec2f& _rectSize) const
{
	return checkfunc::checkPixelsRect(pixels_pos, pixels_size, pixels, _rectPos, _rectSize);
}

bool PixelsCollider::collides(const vec2f& _pixelPos, const vec2f& _pixelsSize, const uint8_t* _pixels) const
{
	return checkfunc::checkPixelsPixels(pixels_pos, pixels_size, pixels, _pixelPos, _pixelsSize, _pixels);
}
