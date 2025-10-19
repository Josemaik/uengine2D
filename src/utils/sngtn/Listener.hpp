#pragma once
#include "../../../openal/AL/al.h"

struct Listener
{
	Listener()
	{
		printf("Listener ha sido construido\n");
		setListenerPosition(0.f, 0.f, 0.f);
		setListenerOrientation(0.f, 0.f, 0.f);
		setListenerVelocity(0.f, 0.f, 0.f);
	}
	void setListenerPosition(float x, float y, float z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}
	void setListenerVelocity(float x, float y, float z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
	}
	void setListenerOrientation(float x, float y, float z)
	{
		alListener3f(AL_ORIENTATION, x, y, z);
	}
};