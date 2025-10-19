#pragma once

#include "../utils/Vec2.hpp"
#include <fstream>
#include <litegfx.h>
#include <unordered_map>
#include <stb_truetype.h>

enum FontTypes : uint8_t
{
	ORANGE_DEFAULT,
	SLAPSTICK_DEFAULT
};

struct FontManager
{
	FontManager() = default;

	//void LoadFont(const char* filename, float height,std::size_t id)
	//{
	//	if (!fontsPool.contains(id)) //Insertamos fuente asociada a una entidad
	//		fontsPool.insert(std::make_pair(id, std::make_pair(load(filename, height)));
	//}
	static FontManager& getInstance() {
		static FontManager instance;
		return instance;
	}

	void load(const char* filename, float height, uint8_t id);
	
	void draw(const char* text, vec2f& pos, uint8_t type) const;

	float getWidth(uint8_t type);

	void unloadFonts();
private:
	int calculateTextureSize(float pixel_height, int num_chars);
	//Pool de fuentes
	std::unordered_map<std::uint8_t, std::pair<const ltex_t*,const stbtt_bakedchar*>> fontsPool;
	//Id de la fuente
	//std::size_t m_id{ 0 };
};