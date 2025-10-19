#include <iostream>
//Sistemas
#include "systems/RenderSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/AnimationSystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/EventSystem.hpp"
#include "systems/AudioSystem.hpp"
#include "systems/LifeSystem.hpp"
#include "systems/AISystem.hpp"
//Managers
#include "managers/ExerciseManager.hpp"
#include "managers/EventManager.hpp"
#include "managers/MapManager.hpp"

//#define _CRT_SECURE_NO_DEPRECATE
//Randoms
#include <cstdlib>
#include <ctime>

#if _DEBUG
#include <crtdbg.h>
#endif

using namespace std;


int main() {

	// Inicializar la semilla con el tiempo actual
	srand(static_cast<unsigned>(time(0)));

#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(83);
#endif
	//Declaramos managers y sistemas
	RenderSystem rsys{};
	PhysicsSystem physys{};
	AnimationSystem amsys{};
	InputSystem insys{};
	CollisionSystem collsys{};
	EventSystem evmsys{};
	AudioSource ss{};
	EntityManager em{};
	LifeSystem lsys{};
	AISystem aisys{};
	//manager que maneja la creación de entidades
	ExerciseManager exm{};
	MapManager map{};
	EventManager evm{};
	GameEngine engine{Screenwidth,Screenheight,0.15f,0.15f,0.15f};
	//Cargamos Audio
	//auto& ss = em.getSingleton<AudioSystem>();
	ss.CreateSounds(engine);
	printf(" buenos dias \n");
	//Creamos las entidades
	//exm.CreateFontEntity(em);
	exm.CreateMap(em);
	exm.CreatePlayer(em);
	exm.createObjects(em);
	exm.CreateEnemy(em);
	// loadMap
	//map.loadMap("../data/map.tmx",0);
	map.loadMap("../data/mapa_prueba_1.tmx");
	//Cargamos Texturas
	rsys.LoadTextures(em,engine,map);
	rsys.LoadFonts(engine);
	//Play audio
	ss.Play(SoundID::MUSIC);
	//Guardo dimensiones del Nivel
	auto& gd = em.getSingleton<GameData>();
	//const ltex_t* level = engine.GetTexture(em.getEntitywithTag<levelTag>()->GetID());
	gd.level_Size = vec2f{ static_cast<float>(map.getMapWidth() * map.getTileWidth()) ,static_cast<float>(Screenheight)};
	//Seteamos Colliders
	collsys.setColliders(em, engine, map);
	//engine.setClearColor();
	//Calcular UVs de la animación
	amsys.SetupUVs(em);
	//time
	float last_frame{ static_cast<float>(engine.GetDeltaTime()) };
	//Loop
	do {
		//Get deltatime
		float now = static_cast<float>(engine.GetDeltaTime());
		float dt = now - last_frame;
		//Guardo las coordenadas del ratón en mi singleton GameData
		//auto &gd = em.getSingleton<GameData>();
		engine.SetCursorPos(gd.mouse_x, gd.mouse_y);
		//Actuaizamos Sistemas
		lsys.update(em, dt);
		if (!gd.lose)
		{
			insys.update(em, engine, evm);
		}
		physys.update(em, dt);
		collsys.update(em,map);
		amsys.update(em, dt);
		aisys.update(em, engine,dt);
		evmsys.update(em,evm,ss,exm,rsys,amsys,collsys,engine,map);
		engine.UpdateCamera(vec2f(static_cast<float>(Screenwidth), static_cast<float>(Screenheight)),gd.level_Size,gd.playerPos);
		//update listener
		auto& lis = em.getSingleton<Listener>();
		lis.setListenerPosition(engine.getCameraPosition().m_x, engine.getCameraPosition().m_y, 0.f);
		//ss.setPosition(engine.getCameraPosition().m_x, engine.getCameraPosition().m_y, 0.f,SoundID::MUSIC);
		rsys.update(em,engine,map);

		//set last frame to now
		last_frame = now;
	} while (!engine.windowShouldClose()); //Mientras no cerremos la ventana, sigue el loop

	//Liberamos memoria
	rsys.freeTextures(engine);
	engine.closeWindow();
}
