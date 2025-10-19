#include "MapManager.hpp"
//cargar mapa
void MapManager::loadMap(const char* filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (result)
	{
		printf("Se ha cargado correctamente\n");// Cargado correctamente, podemos analizar su contenido
		//Nodo raíz -> obtenemos datos
		pugi::xml_node mapNode = doc.child("map");
		width = mapNode.attribute("width").as_int();
		printf("Ancho tile: %d\n", width);
		height = mapNode.attribute("height").as_int();
		printf("Alto tile: %d\n", height);
		/*tilewight = mapNode.attribute("tilewidth").as_int();
		tileheight = mapNode.attribute("tileheight").as_int();*/
		//Tileset
		pugi::xml_node tileset = mapNode.child("tileset");

		std::string ImageSource = tileset.child("image").attribute("source").as_string();
		tilesetPath = extractPath(filename) + ImageSource;

		firstgid = tileset.attribute("firstgid").as_int();
		tilewidth = tileset.attribute("tilewidth").as_int();
		tileheight = tileset.attribute("tileheight").as_int();
		printf("Alto y ancho pixeles tile: %d,%d\n", tilewidth, tileheight);
		tilecolums = tileset.attribute("columns").as_int();
		
		//Recorremos todos los tiles
		for (pugi::xml_node tileNode = mapNode.child("layer").child("data").child("tile");
			tileNode; // Condición correcta
			tileNode = tileNode.next_sibling("tile"))
		{
			int gid = tileNode.attribute("gid").as_int();
			map.push_back(gid);
		}

		printf("Tamano de map: %d\n", map.size());
	/*	pugi::xml_text text = mapNode.text();
		printf("Nombre nodomapa: %s",text.as_string());*/
	}
	else {
		// No se ha podido cargar
		std::cout << result.description() << std::endl;
	}
}

int MapManager::getMapWidth()
{
	return width;
}

int MapManager::getMapHeight()
{
	return height;
}

int MapManager::getTileWidth()
{
	return tilewidth;
}

int MapManager::getTileHeight()
{
	return tileheight;
}

int MapManager::getTileColums()
{
	return tilecolums;
}

int MapManager::getFirstgid()
{
	return firstgid;
}

std::string MapManager::getTilesetPath()
{
	return tilesetPath;
}

const std::vector<int>& MapManager::getMap() const
{
	return map;
}

std::vector<std::unique_ptr<ColliderComponent>>& MapManager::getMapColliders()
{
	return mapColliders;
}
