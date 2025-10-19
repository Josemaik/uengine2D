#pragma once

//Componentes
#include "../cmps/PhysicsComponent.hpp"
#include "../cmps/RenderComponent.hpp"
#include "../cmps/AnimationComponent.hpp"
#include "../cmps/InputComponent.hpp"
#include "../cmps/ColliderComponent.hpp"
#include "../cmps/ScrollComponent.hpp"
#include "../cmps/ListenerComponent.hpp"
#include "../cmps/LifeComponent.hpp"
#include "../cmps/AIComponent.hpp"
//Singletons
#include "sngtn/GameData.hpp"
#include "sngtn/PlayerInfo.hpp"
#include "sngtn/Listener.hpp"
//#include "../systems/AudioSystem.hpp"
//Entity Manager
#include "../managers/Entitymanager.hpp"
#include "../managers/GameEngine.hpp"
//Metaprogramación
#include "meta_program.hpp"

//Fordward declaration
namespace ENGI { struct GameEngine; }

//Eventos
enum EventCodes : uint16_t
{
	RestartGame,
	SpawnShuriken
};

//declare TAGS
//draw
struct beeTag{};
struct levelTag{};
struct tree1Tag{};
struct tree2Tag {};
struct cloudsTag {};
struct scrollTag{};
struct playerTag{};
struct ballTag{};
struct boxTag{};
struct shurikenTag{};
struct foxTag{};
struct flagTag{};
struct coinTag{};
struct recolectableTag{};
struct enemyTag{};

struct skyTag{};
struct clouds2Tag{};
//COMPONENTS
using CL = MP::TypeList <
	PhysicsComponent,
	RenderComponent,
	AnimationComponent,
	ColliderComponent,
	InputComponent,
	ScrollComponent,
	ListenerComponent,
	LifeComponent,
	AIComponent
>;

//TAGS
using TL = MP::TypeList <
	beeTag,
	levelTag,
	tree1Tag,
	tree2Tag,
	cloudsTag,
	scrollTag,
	playerTag,
	ballTag,
	boxTag,
	skyTag,
	clouds2Tag,
	shurikenTag,
	foxTag,
	flagTag,
	coinTag,
	recolectableTag,
	enemyTag
>;
//SINGLETONS
using SCL = MP::TypeList <
	GameData,
	PlayerInfo,
	Listener
	//AudioSystem
>;

using EntityManager = ETMG::EntityManager<CL,TL,SCL>;
using Entity = EntityManager::Entity;
using GameEngine = ENGI::GameEngine;

//Definimos variables globales 
static int Screenwidth = 1024; //el alto y ancho de la ventana
static int Screenheight = 640;
static float center_screen_x = static_cast<float>(Screenwidth / 2);
static float center_screen_y = static_cast<float>(Screenheight / 2);

