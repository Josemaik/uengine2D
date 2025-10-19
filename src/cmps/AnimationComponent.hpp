#pragma once
#include <vector>

struct AnimationComponent
{
	//getters
	int getHframes() const { return hframes; }
	int getVframes() const { return vframes; }
	int getFps() const { return static_cast<int>(framerate); }
	float getCurrentFrame() const { return currentFrame; }
	//setters
	void setFps(int fps) { framerate = static_cast<float>(fps); }
	void setCurrentFrame(float frame) { currentFrame = frame; }
	void setHframes(int _hframes) { hframes = _hframes; }
	void setVframes(int _vframes) { vframes = _vframes; }

	//Set UVsCoord
	void CalculateUVs()
	{
		for (int i = 0; i < hframes; i++)
		{
			int column = i % hframes;
			float u0 = column / static_cast<float>(hframes);
			float u1 = (column + 1) / static_cast<float>(hframes);
			UCoords.push_back(vec2f{ u0,u1 });
		}
	}

	//Número de frames en horizontal y vertical
	int hframes{}, vframes{};
	//Veces por segundo que cambia el frame de animación
	float framerate{};
	float currentFrame{};
	//coordenadas de textura(u0,u1)	- horizontal
	std::vector<vec2f> UCoords;
};