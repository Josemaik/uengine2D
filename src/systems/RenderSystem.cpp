#include "RenderSystem.hpp"
#include "../managers/MapManager.hpp"
#include <stb_image.h>
#include <litegfx.h>
#include <string>

#include <type_traits>
#include <sstream>

//Función que pasa de número a string
template <typename T>
std::string stringFromNumber(T val) {
	std::ostringstream stream;
	stream << std::fixed << val; // Asegúrate de que 'val' sea numérico
	return stream.str();
}

void RenderSystem::update(EntityManager& em, GameEngine& engine, MapManager& map)
{
	//Begin frame //clear background
	engine.beginFrame();
	//tileset.png
	/*ltex_drawrotsized(
		engine.GetTextureMap(),
		0.f,
		0.f,
		0.f,
		0.f,
		0.f,
		32.f * 25,
		32.f * 18,
		0.f, 0.f,
		1.f, 1.f
	);*/
	//Actualizamos entidades y dibujamos
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy, RenderComponent& rc)
	{
			//pasamos los datos de físicas a render
			rc.setPosition(phy.position);
			rc.setScale(phy.scale);
			rc.setOrientation(phy.getAngle());
			rc.setSize(phy.size);
			//auto angle = phy.angle;
			auto pos = rc.getPosition();
			//Set blend
			//lgfx_setblend(rc.getBlend());
			//Traslación cámara
			//lgfx_setorigin(engine.getCameraPosition().m_x, engine.getCameraPosition().m_y);

			if (e.hasTag<scrollTag>())  // Verificamos que sea un fondo
			{
				auto& gd = em.getSingleton<GameData>();
				if (gd.lose || gd.victory)
				{
					lgfx_setcolor(1.f, 1.f, 1.f, 0.5f);
					/*ge.drawFont("DERROTA", posSpawn, FontTypes::SLAPSTICK_DEFAULT);
					lgfx_setcolor(1.f, 1.f, 1.f, 1.f);*/
				}
				//Fondos sin transparencia
				lgfx_setblend(BLEND_ALPHA);

				//obtengo componente scroll
				auto& scrollcmp = em.getComponent<ScrollComponent>(e);
				float scrollRatio = scrollcmp.getScrollRatio();
				const vec2f& scrollOffset = scrollcmp.getscrollOffset();
				
				// Calcular desplazamiento basado en el scroll y el tamaño de la textura
				//Calculamos Coordenadas UV
				float u0 = fmod(engine.getCameraPosition().m_x * scrollRatio + scrollOffset.m_x, rc.getSize().m_x) / rc.getSize().m_x;
				float v0 = fmod(engine.getCameraPosition().m_y * scrollRatio + scrollOffset.m_y, rc.getSize().m_y) / rc.getSize().m_y;

				float u1 = u0 + (Screenwidth /  rc.getSize().m_x);
				float v1 = v0 + (Screenheight / rc.getSize().m_y);

				// Dibujar a pantalla completa con scroll aplicado
			/*	int width = engine.GetTexture(e.GetID())->width;
				for (int i = 0; i < map.getMapWIthonPixels() / width; i += width)
				{
					printf("x: %d", i);
				}*/
				/*int width = engine.GetTexture(e.GetID())->width;
				auto& gd = em.getSingleton<GameData>();*/
				/*if (gd.playerPos.m_x > pos.m_x + Screenwidth - 50.f)
				{
					pos.m_x += Screenwidth;
				}*/
				/*pos.m_x += gd.playerPos.m_x - Screenwidth * 0.5f;
				pos.m_x = engine.getCameraPosition().m_x;*/
				ltex_drawrotsized(
					engine.GetTexture(e.GetID()),
					0.f,   // Posición X
					0.f,  // Posición Y 
					0.f,                             // Sin rotación
					rc.getPivot().m_x,            // Pivot X 
					rc.getPivot().m_y,           // Pivot Y
					static_cast<float>(rc.getSize().m_x),          // Escalar al ancho de la pantalla
					static_cast<float>(rc.getSize().m_y),         // Escalar al alto de la pantalla
					u0, v0,                 // Coordenadas UV iniciales
					u1, v1    // Coordenadas UV finales
				);
				return;
			}
			//if collision, dran RED color
			lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
			if (rc.drawcollision)
			{
				lgfx_setcolor(1.f, 0.f, 0.f, 1.f);
			}
			//Los Sprites
			//Player		
			/*if (e.hasTag<playerTag>() || e.hasTag<ballTag>())
			{*/
			/*	auto& ac = em.getComponent<AnimationComponent>(e);
				rc.u0 = ac.UCoords.at(ac.getCurrentFrame()).m_x;
				rc.u1 = ac.UCoords.at(ac.getCurrentFrame()).m_x;*/
			// Aplicar la traslación de la cámara
			if(e.hasTag<playerTag>())
			{
				lgfx_setorigin(engine.getCameraPosition().m_x, engine.getCameraPosition().m_y);
				//printf("PosPlayer: (%f,%f)\n", pos.m_x, pos.m_y);
			}

			//Debug
			/*if (e.hasTag<shurikenTag>())
			{
				printf("Posshuriken: (%f,%f)\n", pos.m_x, pos.m_y);
			}*/
			//if (e.hasTag<foxTag>())
			//{
			//	//printf("PosFox: (%f,%f)\n", pos.m_x, pos.m_y);
			//}

				//Blend mode
				//lgfx_setblend(rc.getBlend());
				//Draw
				ltex_drawrotsized(
					engine.GetTexture(e.GetID()),
					pos.m_x,
					pos.m_y,
					rc.angle,
					rc.getPivot().m_x,
					rc.getPivot().m_y,
					rc.getScale().m_x * rc.getSize().m_x * phy.direction_scale,//rc.getScale().m_x * rc.getSize().m_x * phy.direction_scale,
					rc.getScale().m_y * rc.getSize().m_y,//rc.getScale().m_y * rc.getSize().m_y,
					rc.u0,	
					rc.v0,		
					rc.u1, 
					rc.v1		
				);

				/*if (e.hasTag<playerTag>())
				{
					lgfx_setorigin(0.0f, 0.0f);
				}*/
				
				//debug pos
				//lgfx_setcolor(0.f, 0.f, 1.f, 1.f);
				//lgfx_drawoval(pos.m_x, pos.m_y, 5.f, 5.f);
				//lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
				//////draw coll
				/*if (e.hasTag<playerTag>())
				{
					lgfx_setcolor(0.f, 0.f, 1.f, 1.f);
					lgfx_drawoval(pos.m_x, pos.m_y, 5.f, 5.f);
					lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
					vec2f size = rc.getSize();
					lgfx_setcolor(0.f, 1.f, 0.f, 0.5f);
					lgfx_drawrect(pos.m_x, pos.m_y, rc.getScale().m_x * rc.getSize().m_x * phy.direction_scale, rc.getSize().m_y);
					lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
				}*/
				//else {
				//	lgfx_setcolor(0.f, 1.f, 0.f, 0.5f);
				//	lgfx_drawrect(pos.m_x , pos.m_y, rc.getSize().m_x, rc.getSize().m_y);
				//	lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
				//}
				////lgfx_setorigin(0.0f, 0.0f);
			//}
	});
	//
	lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
	//Dibujamos mapa
	DrawMap(engine, map);
	//Fuentes
	DrawFonts(engine,em);
	//shuriken debug
	//auto e = em.getEntitywithTag<shurikenTag>();
	//if (e)
	//{
	//	auto& rc = em.getComponent<RenderComponent>(*e);
	//	auto& phy = em.getComponent<PhysicsComponent>(*e);
	//	rc.setPosition(phy.position);
	//	rc.setScale(phy.scale);
	//	rc.setOrientation(phy.getAngle());
	//	rc.setSize(phy.size);
	//	ltex_drawrotsized(
	//		engine.GetTexture(e->GetID()),
	//		rc.position.m_x,
	//		rc.position.m_y,
	//		rc.angle,
	//		rc.getPivot().m_x,
	//		rc.getPivot().m_y,
	//		rc.getScale().m_x* rc.getSize().m_x* phy.direction_scale,//rc.getScale().m_x * rc.getSize().m_x * phy.direction_scale,
	//		rc.getScale().m_y* rc.getSize().m_y,//rc.getScale().m_y * rc.getSize().m_y,
	//		rc.u0,
	//		rc.v0,
	//		rc.u1,
	//		rc.v1
	//	);
	//}

	//End frame
	engine.endFrame();
}

//dibujar mapa
void RenderSystem::DrawMap(GameEngine& ge, MapManager& map)
{
	//Obtenemos datos
	const ltex_t* tileset = ge.GetTextureMap();
	const std::vector<int>& mapVec = map.getMap();
	int tiles_colums = map.getTileColums();
	int tilewidth = map.getTileWidth();
	int tileheight = map.getTileHeight();

	//Recorremos tiles
	for (int y = 0; y < map.getMapHeight(); y++)
	{
		for (int x = 0; x < map.getMapWidth(); x++)
		{
			int index = y * map.getMapWidth() + x;
			int gid = mapVec[index];

			//si es 0, next iteración
			if (gid == 0) continue;
			if (gid < map.getFirstgid()) continue;

			int tileindex = gid - map.getFirstgid();

			//UV coordenadas
			float u0 = static_cast<float>((tileindex % tiles_colums) * tilewidth) / tileset->width;
			float v0 = static_cast<float>((tileindex / tiles_colums) * tileheight) / tileset->height;
			float u1 = u0 + (static_cast<float>(tilewidth) / tileset->width);
			float v1 = v0 + (static_cast<float>(tileheight) / tileset->height);

			//posición
			float pos_x = static_cast<float>(x * tilewidth);
			float pos_y = static_cast<float>(y * tileheight);
			/*auto pos = vec2f{ pos_x,pos_y };
			ge.drawFont("Hola", pos, ORANGE_DEFAULT);*/
			//Dibujamos tile
			ltex_drawrotsized(
				tileset,
				pos_x,
				pos_y,
				0.f,
				0.f,
				0.f,
				static_cast<float>(tilewidth),
				static_cast<float>(tileheight),
				u0,v0,
				u1,v1
			);
			//debug pos
			//lgfx_setcolor(0.f, 0.f, 1.f, 1.f);
			//lgfx_drawoval(pos_x, pos_y,5.f,5.f);
			//lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
			////draw coll
			//lgfx_setcolor(0.f, 1.f, 0.f, 0.5f);
			//lgfx_drawrect(pos_x, pos_y, 32.f, 32.f);
			//lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
		}
	}
}

//Dibujar fuentes
void RenderSystem::DrawFonts(GameEngine& ge, EntityManager& em)
{
	//Score
	auto& gd = em.getSingleton<GameData>();
	vec2f camera = ge.getCameraPosition();
	printf("PosFox: (%f,%f)\n", camera.m_x, camera.m_y);
	vec2f posSpawn = vec2f{ ge.getCameraPosition().m_x + 100.f,100.f };
	std::string score = "Score: " + std::to_string(gd.score);
	lgfx_setcolor(1.f,0.f,0.f,1.f);
	ge.drawFont(score.c_str(), posSpawn, FontTypes::SLAPSTICK_DEFAULT);
	lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
	//Derrota/Victoria
	if (gd.lose)
	{
		posSpawn = vec2f{ ge.getCameraPosition().m_x + 400.f,center_screen_y};
		lgfx_setcolor(1.f, 0.f, 0.f, 1.f);
		ge.drawFont("DERROTA", posSpawn, FontTypes::SLAPSTICK_DEFAULT);
		lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
	}
	if (gd.victory)
	{
		posSpawn = vec2f{ ge.getCameraPosition().m_x + 400.f,center_screen_y };
		vec2f posSpawn2 = vec2f{ ge.getCameraPosition().m_x + 250.f,center_screen_y + 100.f };
		lgfx_setcolor(0.f, 1.f, 0.f, 1.f);
		ge.drawFont("VI CTORI A", posSpawn, FontTypes::SLAPSTICK_DEFAULT);
		ge.drawFont("PRESS ENTER TO RESTART", posSpawn2, FontTypes::SLAPSTICK_DEFAULT);
		lgfx_setcolor(1.f, 1.f, 1.f, 1.f);
	}
}

void RenderSystem::DrawTiled(int numtiles, RenderComponent& rc,GameEngine& ge, size_t id_ent)
{
	for (int dx = 0; dx <= numtiles; ++dx) {
		for (int dy = 0; dy <= numtiles; ++dy) {
			ltex_draw(ge.GetTexture(id_ent),
				rc.position.m_x + dx * rc.getSize().m_x,
				rc.position.m_y + dy * rc.getSize().m_y);
		}
	}
}

//void RenderSystem::DrawTiledRot(int numtiles, RenderComponent& rc, GameEngine& ge, size_t id_ent)
//{
//	for (int dx = 0; dx <= numtiles; ++dx) {
//		for (int dy = 0; dy <= numtiles; ++dy) {
//			ltex_draw(ge.GetTexture(id_ent),
//				rc.position.m_x + dx * rc.getSize().m_x,
//				rc.position.m_y + dy * rc.getSize().m_y);
//		}
//	}
//}

void RenderSystem::LoadFonts(GameEngine& engine)
{
	//Cargamos Fuentes
	engine.LoadFont("../data/fuentes/Orange.ttf", 42.f, FontTypes::ORANGE_DEFAULT);
	engine.LoadFont("../data/fuentes/SFSlapstickComic.ttf", 42.f, FontTypes::SLAPSTICK_DEFAULT);
}

//void RenderSystem::SetupUVs(EntityManager& em)
//{
//	using SYSCMPs = MP::TypeList<RenderComponent,AnimationComponent>;
//	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& rc, AnimationComponent& ac)
//	{
//			// Calcular las coordenadas de textura
//			/*int currentFrameIndex = static_cast<int>(ac.getCurrentFrame());
//			int column = currentFrameIndex % ac.getHframes();*/
//			for (unsigned int i = 0; i < ac.getHframes(); i++)
//			{
//				int column = i % ac.getHframes();
//				float u0 = column / static_cast<float>(ac.getHframes());
//				float u1 = (column + 1) / static_cast<float>(ac.getHframes());
//				ac.UCoords.push_back(vec2f{ u0,u1 });
//			}
//	});
//}

//Cargar texturas
void RenderSystem::LoadTextures(EntityManager& em,GameEngine& engine, MapManager& map)
{
	//Cargamos texturas
	//Mapa
	engine.LoadTextureMap(map.getTilesetPath().c_str());
	//Sprites
	//PLayer
	/*engine.LoadTexture("../data/idle.png", TexturesType::PLAYER_IDLE); //Personaje prac 7
	engine.LoadTexture("../data/run.png", TexturesType::PLAYER_RUN);*/
	engine.LoadTexture("../data/player/idle1.png", TexturesType::PLAYER_IDLE); //Personaje nuevo
	engine.LoadTexture("../data/player/run.png", TexturesType::PLAYER_RUN);

	engine.LoadTexture("../data/player/shuriken.png",TexturesType::SHURIKEN);

	engine.LoadTexture("../data/enemies/fox.png", TexturesType::FOX);
	engine.LoadTexture("../data/enemies/fox-sword.png", TexturesType::FOXSHORD);
	//engine.LoadTexture("../data/ball.png", TexturesType::BALL);->practica
	engine.LoadTexture("../data/bee_anim.png", TexturesType::BEE);
	engine.LoadTexture("../data/box.png", TexturesType::BOX);

	//engine.LoadTexture("../data/sky.png", TexturesType::SKY);
	//Nubes
	//engine.LoadTexture("../data/clouds.png", TexturesType::CLOUDS);
	engine.LoadTexture("../data/clouds_5.png", TexturesType::CLOUDS2);
	//flag
	engine.LoadTexture("../data/flag.png", TexturesType::FLAGFINAL);
	engine.LoadTexture("../data/coin.png", TexturesType::COIN);
	/*engine.LoadTexture("../data/level.png", TexturesType::LEVEL);
	engine.LoadTexture("../data/trees1.png", TexturesType::TREES1);
	engine.LoadTexture("../data/trees2.png", TexturesType::TREES2);*/

	//Aignamos texturas a entidades
	using SYSCMPsaux = MP::TypeList<RenderComponent>;
	em.forEach<SYSCMPsaux, SYSTAGs>([&](Entity& e, RenderComponent& rc)
	{
		TexturesType text_type { TexturesType::BALL };
		if (e.hasTag<beeTag>())
		{
			text_type = TexturesType::BEE;
		}
		if (e.hasTag<boxTag>())
		{
			text_type = TexturesType::BOX;
		}
		/*if (e.hasTag<levelTag>())
		{
			text_type = TexturesType::LEVEL;
		}
		if (e.hasTag<tree1Tag>())
		{
			text_type = TexturesType::TREES1;
		}
		if (e.hasTag<tree2Tag>())
		{
			text_type = TexturesType::TREES2;
		}*/
		if (e.hasTag<playerTag>())
		{
			text_type = TexturesType::PLAYER_IDLE;
		}
		if (e.hasTag<cloudsTag>())
		{
			text_type = TexturesType::CLOUDS;
		}
		if (e.hasTag<ballTag>())
		{
			text_type = TexturesType::BALL;
		}
		if (e.hasTag<skyTag>())
		{
			text_type = TexturesType::SKY;
		}
		if (e.hasTag<clouds2Tag>())
		{
			text_type = TexturesType::CLOUDS2;
		}
		if (e.hasTag<shurikenTag>())
		{
			text_type = TexturesType::SHURIKEN;
		}
		if (e.hasTag<foxTag>())
		{
			text_type = TexturesType::FOX;
		}
		if (e.hasTag<flagTag>())
		{
			text_type = TexturesType::FLAGFINAL;
		}
		if (e.hasTag<coinTag>())
		{
			text_type = TexturesType::COIN;
		}
		//guardo textura en pool y me devuelvo el size
		vec2f size = engine.SetTexturetoEnt(e.GetID(), text_type);
		//si tiene animacion, ajusto size
		if (e.hasTag<foxTag>() || e.hasTag<beeTag>() || e.hasTag<coinTag>())
		{
			auto& ac = em.getComponent<AnimationComponent>(e);
			size.m_x /= ac.getHframes();
		}
		rc.setSize(size);
		if (e.hasComponent<PhysicsComponent>())
		{
			auto& phy = em.getComponent<PhysicsComponent>(e);
			phy.size = size;
		}
	});
}

//Liberar memoria de las texturas
void RenderSystem::freeTextures(GameEngine& engine)
{
	engine.unloadTextures();
	engine.unloadFonts();
}