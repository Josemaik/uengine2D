#pragma once

#include<map>
#include <litegfx.h>
#include <stb_image.h>
#include "../utils/Vec2.hpp"

//enum texturas-> entidades
enum TexturesType : uint8_t
{
	PLAYER_IDLE,
	PLAYER_RUN,
	BEE,
	BOX,
	LEVEL,
	TREES1,
	TREES2,
	CLOUDS,
	BALL,
	CLOUDS2,
	SKY,
	SHURIKEN,
	FOX,
	FOXSHORD,
	FLAGFINAL,
	COIN
};

struct TextureManager
{
public:
	static TextureManager& getInstance() {
		static TextureManager instance;
		return instance;
	}
	void SaveTexture(const char* filename, TexturesType type)
	{
		//save texture in pool
		texturesPool.insert(std::make_pair(type, GenerateTexture(filename)));
	}
	//Seteamos textura a la entidad
	vec2f SetTextureEnt(std::size_t id_ent, TexturesType type)
	{
		const ltex_t* textfinded = texturesPool.at(type);
		if (textfinded)
		{
			if (texturesEnt.contains(id_ent)) //si existe una textura valida para esa entidad
			{
				texturesEnt.at(id_ent).first = textfinded; //sustituimos por la nueva textura
				texturesEnt.at(id_ent).second = type;
			}
			else { //si no hay textura para esa entidad
				texturesEnt.insert(std::make_pair(id_ent, std::make_pair(textfinded,type))); //Insertamos una nueva entidad asociada a la textura	
			}
			return vec2f{ static_cast<float>(textfinded->width),static_cast<float>(textfinded->height) }; //devolvemos size
		}
		//si no existe, devolvemos vector vacio
		return vec2f{ 0.f,0.f };
	}

	//Devolver la textura dad un id de entidad
	const ltex_t* GetTexture(size_t id)
	{
		return texturesEnt.at(id).first;
	}
	//Devolver los píxeles dad una textura
	unsigned char* Getpixels(size_t id)
	{
		//obetemos la textura
		const ltex_t* _texture = texturesEnt.at(id).first;
		// obtener las dimensiones de la textura
		int width =  _texture->width;
		int height = _texture->height;

		// calcular el tamaño del buffer (asumiendo 4 bytes por píxel: rgba)
		size_t buffersize = width * height * 4;

		// asignar memoria para los píxeles
		unsigned char* outpixels = new unsigned char[buffersize];

		// obtener los píxeles
		ltex_getpixels(_texture, outpixels);

		// retornar el puntero
		return outpixels;
	}

	TexturesType GetTextureType(size_t id)
	{
		return texturesEnt.at(id).second;
	}

	//Liberamos memoria
	void UnloadTextures()
	{
		for (auto& tex : texturesPool)
		{
			ltex_free(const_cast<ltex_t*>(tex.second));
		}
	}

	///MAP
	void SaveMapTexture(const char* filename)
	{
		textureMap = GenerateTexture(filename);
	}

	const ltex_t* GetTextureMap()
	{
		return textureMap;
	}

private:
	ltex_t* GenerateTexture(const char* filename)
	{
		GLFWimage images[1];
		//int* image_comp;
		//buffer que almacena los datos de la imágen
		images[0].pixels = stbi_load(filename, &images[0].width, &images[0].height, 0, 4);
		if (images[0].pixels && images[0].width && images[0].height) //verificamos que se haya cargado bien
		{
			//Generamos la textura
			ltex_t* data = ltex_alloc(images[0].width, images[0].height, 1);
			//Volcar píxeles
			ltex_setpixels(data, images[0].pixels);
			//Liberamos el buffer generado
			stbi_image_free(images[0].pixels);
			//Devolvemos la textura generada
			return data;
		}
		printf("Error al cargar la textura: %s", filename);
		return nullptr;
	}
	//Textures storage
	std::map<TexturesType, const ltex_t*> texturesPool;
	//Textures ascoiated with entities by ID
	std::map<std::size_t, std::pair<const ltex_t*, TexturesType>> texturesEnt;
	//Texture map
	const ltex_t* textureMap{nullptr};
};