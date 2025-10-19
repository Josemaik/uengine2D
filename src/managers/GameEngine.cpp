#include "GameEngine.hpp" 
#include <algorithm>

ENGI::GameEngine::GameEngine(int const width, int const height, float _clearRed, float _clearGreen, float _clearBlue)
{
	//iniciamos la ventana
	ENGI::GameEngine::InitWindow(width, height);
	//seteamos el modo de input
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//set clear color
	clearRed = _clearRed;
	clearGreen = _clearGreen;
	clearBlue = _clearBlue;
}
void ENGI::GameEngine::InitWindow(int const width, int const height)
{
	glfwInit();

	window = glfwCreateWindow(width, height, "Mi juegaso", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	lgfx_setup2d(width, height);

	//----- Configure callback -----//
	glfwSetWindowUserPointer(getWindow(), &m_inputManager);
	glfwSetKeyCallback(getWindow(), InputManager::keyCallback);
	glfwSetMouseButtonCallback(getWindow(), InputManager::mouseButtonCallback);
}
bool ENGI::GameEngine::windowShouldClose()
{
	return glfwWindowShouldClose(glfwGetCurrentContext());
}

void ENGI::GameEngine::closeWindow()
{
	if (window) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}
double ENGI::GameEngine::GetDeltaTime()
{
	//deltatime
	return glfwGetTime();
}

void ENGI::GameEngine::beginFrame()
{
	//Color de fondo
	lgfx_clearcolorbuffer(clearRed, clearGreen, clearBlue);
}

void ENGI::GameEngine::endFrame()
{
	m_inputManager.updateEndFrame();
	//Intercambiamos backbuffer por frontbuffer
	glfwSwapBuffers(glfwGetCurrentContext());
	//Procesar Eventos
	glfwPollEvents();
}

void ENGI::GameEngine::SetCursorPos(double& mouse_x,double& mouse_y)
{
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
}
/////////////////////////////////
/////TEXTURES
void ENGI::GameEngine::LoadTexture(const char* filename, TexturesType type)
{
	m_textureManager.SaveTexture(filename, type);
}
void ENGI::GameEngine::LoadTextureMap(const char* filename)
{
	m_textureManager.SaveMapTexture(filename);
}

vec2f ENGI::GameEngine::SetTexturetoEnt(size_t id,TexturesType type)
{
	 return m_textureManager.SetTextureEnt(id, type);
}
const ltex_t* ENGI::GameEngine::GetTexture(size_t id)
{
	return m_textureManager.GetTexture(id);
}
const ltex_t* ENGI::GameEngine::GetTextureMap()
{
	return m_textureManager.GetTextureMap();
}
unsigned char* ENGI::GameEngine::getPixels(size_t id)
{
	return m_textureManager.Getpixels(id);
}
TexturesType ENGI::GameEngine::GetTextureType(size_t id)
{
	return m_textureManager.GetTextureType(id);
}
void ENGI::GameEngine::unloadTextures()
{
	m_textureManager.UnloadTextures();
}
/////////////////////////////////
/////FONTS
void ENGI::GameEngine::LoadFont(const char* filename, float height,std::uint8_t type)
{
	m_fontManager.load(filename, height,type);
}
void ENGI::GameEngine::drawFont(const char* text, vec2f& pos, std::uint8_t type)
{
	m_fontManager.draw(text, pos,type);
}
float ENGI::GameEngine::getWidth(std::uint8_t type)
{
	return m_fontManager.getWidth(type);
}
void ENGI::GameEngine::unloadFonts()
{
	m_fontManager.unloadFonts();
}
//////////////////////////////////////////
// INPUTS
// Check if a key has been pressed once
bool ENGI::GameEngine::IsKeyPressed(int key) {
	return m_inputManager.isKeyPressed(key);
}
// Check if a key has been pressed once
bool ENGI::GameEngine::IsKeyReleased(int key) {
	return m_inputManager.isKeyReleased(key);
}

bool ENGI::GameEngine::IsKeyDown(int key)
{
	return m_inputManager.isKeyDown(key);
}

bool ENGI::GameEngine::IsMouseButtomPressed(int buttom)
{
	return m_inputManager.isMouseButtonPressed(buttom);
}
bool ENGI::GameEngine::IsMouseButtomDown(int buttom)
{
	return m_inputManager.isMouseButtonDown(buttom);
}

//////////////////////////////////
//CAMERA
const vec2f& ENGI::GameEngine::getCameraPosition() const
{
	return cameraPos;
}

void ENGI::GameEngine::setCameraPosition(const vec2f& pos)
{
	cameraPos = pos;
}

void ENGI::GameEngine::UpdateCamera(const vec2f& screenSize, const vec2f& levelSize, const vec2f& targetSpritePosition)
{
	//Calcular la posición deseada de la cámara para centrar el sprite
	vec2f desiredCameraPos;
	desiredCameraPos.m_x = targetSpritePosition.m_x - screenSize.m_x * 0.5f;
	desiredCameraPos.m_y = targetSpritePosition.m_y - screenSize.m_y * 0.5f;
	//Clampeamos
	desiredCameraPos.m_x = std::clamp(desiredCameraPos.m_x, 0.0f, levelSize.m_x - screenSize.m_x);
	desiredCameraPos.m_y = std::clamp(desiredCameraPos.m_y, 0.0f, levelSize.m_y - screenSize.m_y);

	//Actualizar la posición de la cámara
	setCameraPosition(desiredCameraPos);
}
//////////////////////////////////
//AUDIO
ALuint& ENGI::GameEngine::getAudioBuffer(SoundID id) const
{
	return m_AudioManager.getAudioBuffer(id);
}

void ENGI::GameEngine::loadAudio(const char* filename, SoundID id)
{
	m_AudioManager.loadsound(filename,id);
}

//ALint ENGI::GameEngine::getAudioBuffer(int index) const
//{
//	return m_AudioManager.getAudioBuffer(index);
//}
