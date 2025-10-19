#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include "../utils/types.hpp"
#include "../lib/pugixml/pugixml.hpp"

struct MapManager
{
public:
	void loadMap(const char* filename);
	int getMapWidth();
	int getMapHeight();

	int getTileWidth();
	int getTileHeight();
	int getTileColums();
	float getMapWIthonPixels() { return static_cast<float>(width * tilewidth); }

	int getFirstgid();
	std::string getTilesetPath();

	const std::vector<int>& getMap() const;
	std::vector<std::unique_ptr<ColliderComponent>>& getMapColliders();
private:

	inline std::string extractPath(const std::string& filename) {
		std::string filenameCopy = filename;
		while (filenameCopy.find("\\") != std::string::npos) {
			filenameCopy.replace(filenameCopy.find("\\"), 1, "/");
		}
		filenameCopy = filenameCopy.substr(0, filenameCopy.rfind('/'));
		if (filenameCopy.size() > 0) filenameCopy += "/";
		return filenameCopy;
	}


	//Dimensiones del mapa
	int width = 0, height = 0;

	//índice del primer tile
	int firstgid{0};
	//Columnas del tile
	int tilecolums{ 0 };
	//Ancho y Alto de cada frame
	int tilewidth{0}, tileheight{0};

	//tileset image path
	std::string tilesetPath{};
	//vector de tiles id
	std::vector<int> map;
	//tiles colisionables
	std::vector<std::unique_ptr<ColliderComponent>> mapColliders;
};

