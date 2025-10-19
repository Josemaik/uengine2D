#pragma once

#ifndef GAME_ENGINE
#define GAME_ENGINE

//#include "../utils/types.hpp"
#include <glfw3.h>
//#include <litegfx.h>
#include <cstdint>
//#include <stb_image.h>
#include <map>

//Managers
#include "InputManager.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "AudioManager.hpp"

//Macros liberías
#define STB_IMAGE_IMPLEMENTATION
#define LITE_GFX_IMPLEMENTATION


namespace ENGI
{
	struct GameEngine
	{
		//using u16 = std::uint16_t;

		GameEngine(int const width, int const height, float clearRed,float clearGreen,float clearBlue);
		//DRAW -> limpiar pantalla
		void beginFrame();
		//DRAW -> termina dibujado
		void endFrame();
		//FRAMERATE
		double GetDeltaTime();
		//MOUSE
		void SetCursorPos(double& mouse_x, double& mouse_y);
		//WINDOW
		void InitWindow(int const width, int const height);
		bool windowShouldClose();
		void closeWindow();
		GLFWwindow* getWindow() { return window; }
		//TEXTURES
		void LoadTexture(const char* filename, TexturesType type);
		void LoadTextureMap(const char* filename);
		vec2f SetTexturetoEnt(std::size_t id,TexturesType type);
		const ltex_t* GetTexture(size_t id);
		const ltex_t* GetTextureMap();
		TexturesType GetTextureType(size_t id);
		unsigned char* getPixels(size_t id);
		void unloadTextures();
		//FONTS
		void LoadFont(const char* filename, float height, std::uint8_t type);
		void drawFont(const char* text, vec2f& pos, std::uint8_t type);
		float getWidth(std::uint8_t type);
		void unloadFonts();
		//INPUTS
		bool IsKeyPressed(int key);
		bool IsKeyReleased(int key);
		bool IsKeyDown(int key);
		bool IsMouseButtomPressed(int buttom);
		bool IsMouseButtomDown(int buttom);
		//CAMERA
		const vec2f& getCameraPosition() const;
		void setCameraPosition(const vec2f& pos);
		void UpdateCamera(const vec2f& screenSize, const vec2f& levelSize, const vec2f& targetSprite);
		//AUDIO
		ALuint& getAudioBuffer(SoundID id) const;
		void loadAudio(const char* filename,SoundID id);
		/*ALint getAudioBuffer(int index) const
		{
			return m_AudioManager.getAudioBuffer(index);
		}*/
	private:
		//window
		GLFWwindow* window{ nullptr };
		//camera
		vec2f cameraPos{};
		//Clear Colors
		float clearRed, clearGreen, clearBlue;
		// Managers Instances
		InputManager& m_inputManager = InputManager::getInstance();
		TextureManager& m_textureManager = TextureManager::getInstance();
		FontManager& m_fontManager = FontManager::getInstance();
		AudioManager& m_AudioManager = AudioManager::getInstance();
	};
	#endif // !GAME_ENGINE
}