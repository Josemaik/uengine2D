#include "CollisionSystem.hpp"
#include "../managers/EventManager.hpp"
#include "../managers/MapManager.hpp"

void CollisionSystem::update(EntityManager& em,MapManager& map)
{
	bool collisionDetected = false;
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy,ColliderComponent& collc)
	{
		//Axtualizar posiciones de los colliders
		collc.getCollider()->updatePosition(phy.position, phy.scale);

		//////////////////////////
		//Check Fox Collisions
		if (e.hasTag<foxTag>())
		{
			auto& gd = em.getSingleton<GameData>();
			//shuriken
			bool shuriken_kill_fox{ false };
			auto shuriken = em.getEntitywithTag<shurikenTag>();
			if (shuriken)
			{
				auto& collsh = em.getComponent<ColliderComponent>(*shuriken);
				if (collsh.getCollider()->collides(*collc.getCollider()))
				{
					em.destroyEntity(e.GetID());
					//rc.drawcollision = collisionDetected = true;
					gd.score += 150;

					shuriken_kill_fox = true;
				}
			}
			//player
			if (!shuriken_kill_fox)
			{
				auto& collp = em.getComponent<ColliderComponent>(*em.getEntitywithTag<playerTag>());
				if (collp.getCollider()->collides(*collc.getCollider()))
				{
					printf("derrota\n");
					gd.lose = true;
					gd.SetSpawnPos(phy.position); //Calculo último spawn

					auto& aic = em.getComponent<AIComponent>(e);
					aic.gotopatrolPoint = true;
				}
			}
		}
		//////////////////////////
		//Check box Collisions
		if (e.hasTag<boxTag>())
		{
			auto& gd = em.getSingleton<GameData>();
			auto& rc = em.getComponent<RenderComponent>(e); 
			//player
			auto& collp = em.getComponent<ColliderComponent>(*em.getEntitywithTag<playerTag>());
			if (collp.getCollider()->collides(*collc.getCollider())) {//|| collsh.getCollider()->collides(*collc.getCollider())
				rc.drawcollision = collisionDetected = true;
				gd.lose = true; //muero
				gd.SetSpawnPos(phy.position); //Calculo último spawn
			}
			else {
				rc.drawcollision = false;
			}

			//SHURIKEN
			if (e.hasTag<boxTag>())
			{
				auto shuriken = em.getEntitywithTag<shurikenTag>();
				if (shuriken)
				{
					auto& collsh = em.getComponent<ColliderComponent>(*shuriken);
					if (collsh.getCollider()->collides(*collc.getCollider()))
					{
						em.destroyEntity(e.GetID()); //destruye
						rc.drawcollision = collisionDetected = true;
						gd.score += 50; //da puntos
					}
				}
			}
		}
		//////////////////////////
		//Check Player Collisions
		if (e.hasTag<playerTag>())
		{
			//TILES
			phy.onground = false;
			for (auto& tile : map.getMapColliders())
			{
				auto& colltile = *tile->getCollider();
				if (collc.getCollider()->collides(*tile->getCollider())) {//collc.getCollider()->collides(*tile->getCollider())//tile->getCollider()->collides(*collc.getCollider())
					//comprobar checkpoint
					//Check collision Down
					if (colltile.getPosition().m_y >= phy.position.m_y) {
						// Colisión por abajo
						phy.position.m_y = phy.prevPosition.m_y;
						phy.onground = true;
						break;
					}
					//Check collision UP
					if (phy.position.m_y < colltile.getPosition().m_y + 32.f)
					{
						phy.position.m_y = phy.prevPosition.m_y;
						break;
					}
					//Check Collision Left
					if (phy.position.m_x - 32.f < colltile.getPosition().m_x + 32.f
						&& colltile.getPosition().m_y <= phy.position.m_y)
					{
						phy.position.m_x = phy.prevPosition.m_x;
						phy.onground = true;
						break;
					}
					//Check collision Right
					if (phy.position.m_x + 32.f > colltile.getPosition().m_x
						&& colltile.getPosition().m_y <= phy.position.m_y)//static_cast<float>(map.getTileWidth()))
					{
						phy.position.m_x = phy.prevPosition.m_x;
						break;
					}
				}
			}
		}
		//////////////////////////
		//Check Flag Collision
		if (e.hasTag<flagTag>())
		{
			auto& collp = em.getComponent<ColliderComponent>(*em.getEntitywithTag<playerTag>());
			if (collp.getCollider()->collides(*collc.getCollider())) {
				auto& gd = em.getSingleton<GameData>();
				gd.victory = true;
			}
		}
		//////////////////////////
		//Check Coin Collision
		if (e.hasTag<coinTag>())
		{
			auto& collp = em.getComponent<ColliderComponent>(*em.getEntitywithTag<playerTag>());
			if (collp.getCollider()->collides(*collc.getCollider())) {
				auto& gd = em.getSingleton<GameData>();
				gd.score += 25;
				em.destroyEntity(e.GetID());
			}
		}
	});
}

void CollisionSystem::setColliders(EntityManager& em, GameEngine& ge,MapManager& map)
{
	//Seteo colliders a entidades
	using SYSCMPsaux = MP::TypeList<ColliderComponent, RenderComponent>;
	em.forEach<SYSCMPsaux, SYSTAGs>([&](Entity& e,ColliderComponent& collc,RenderComponent& rc)
	{
			//Obtengo los píxeles de la textura si es de tipo pixels
			unsigned char* pixels{ nullptr };
			if (collc.getCollisionType() == CollisionType::COLLISION_PIXELS)
			{
				 pixels = ge.getPixels(e.GetID());
			}

			//paso datos al collider
			collc.setCollisionType(collc.getCollisionType(), rc.getPosition(), rc.getSize(), rc.getPivot(), pixels);
	});
	//Seteo colliders al mapa
	int tilewidth = map.getTileWidth();
	int tileheight = map.getTileHeight();
	auto& mapVec = map.getMap();
	for (int y = 0; y < map.getMapHeight(); y++)
	{
		for (int x = 0; x < map.getMapWidth(); x++)
		{
			int index = y * map.getMapWidth() + x;
			int gid = mapVec[index];
			//Si no son suelos seguimos
			//Si no son plataformas estáticas seguimos
			if (gid != 171 && gid != 185 
			&& gid != 30 && gid != 31 && gid != 32
			&& gid != 113 && gid != 114 && gid != 115) continue;
			//Calculo posición
			float pos_x = static_cast<float>(x * tilewidth);
			float pos_y = static_cast<float>(y * tileheight);
			auto pos = vec2f{ pos_x, pos_y };
			//Obtengo collidercomponent
			auto collc = std::make_unique<ColliderComponent>();
			collc->setCollisionType(
				COLLISION_RECT,
				vec2f{ pos_x, pos_y },
				vec2f{ static_cast<float>(tilewidth), static_cast<float>(tileheight) },
				vec2f{ 0.f, 0.f },
				nullptr
			);
			map.getMapColliders().emplace_back(std::move(collc));
		}
	}
}