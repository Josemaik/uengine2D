#include "AISystem.hpp"

void AISystem::update(EntityManager& em, GameEngine& ge, float dt)
{
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, AIComponent& aic, PhysicsComponent& phy,RenderComponent& rc)
	{
		/////////////////
		//FOX AI
		if (e.hasTag<foxTag>())
		{
			//Detectar player
			auto& gd = em.getSingleton<GameData>();
			if (!aic.playerdetected)
			{
				float distance = gd.playerPos.distance(phy.position);
				if (distance < (aic.detect_radius * aic.detect_radius))
				{
					//Player detectado
					aic.playerdetected = true;
					//Change animation
					ge.SetTexturetoEnt(e.GetID(), FOXSHORD);
				}
			}
			
			//Perseguir al player
			if (aic.playerdetected)
			{
				//Actualizamos velocidad según la posición del enemigo
				//seek
				phy.vel.m_x = aic.MaxSpeed * (gd.playerPos.m_x - phy.position.m_x);
				if (phy.vel.m_x < 0.f) //cambiar scale driection
				{
					//rc.setScale(vec2f{ -1.f, 0.f });
					phy.direction_scale = -1.f;
				}
				else {
					rc.setScale(vec2f{ 1.f, 0.f });
					phy.direction_scale = 1.f;
				}
			}

			//Volver a punto de patrulla
			if (aic.gotopatrolPoint)
			{
				phy.vel.m_x = aic.patrol_position.m_x - phy.position.m_x;
				float dist_to_patrol = aic.patrol_position.distance(phy.position);
				if (dist_to_patrol < 1.f)
				{
					//He llegado
					aic.gotopatrolPoint = false;
					aic.playerdetected = false;
					//Change animation
					ge.SetTexturetoEnt(e.GetID(), FOX);
				}
			}
		}
		/////////////////
		///BEE AI
		if (e.hasTag<beeTag>())
		{
			if (aic.decreaseTimer(dt, aic.elapsed_throw))
			{
				//Drop box
				auto& box{ em.createEntity() };
				em.addTag<boxTag>(box);
				auto& pc3 = em.addComponent<PhysicsComponent>(box, PhysicsComponent{ .position = vec2f(phy.position.m_x,phy.position.m_y + 50.f),.scale = vec2f(0.5f,0.5f) });
				auto& rc3 = em.addComponent<RenderComponent>(box, RenderComponent{ .position = pc3.position,.scale = pc3.scale,.blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.f,0.f) });
				auto& collc = em.addComponent<ColliderComponent>(box, ColliderComponent{ CollisionType::COLLISION_RECT });
				em.addComponent<LifeComponent>(box, LifeComponent{ .countdown_die = 3.f });
				auto size = ge.SetTexturetoEnt(box.GetID(), TexturesType::BOX);
				rc3.setSize(size);
				pc3.size = size;

				unsigned char* pixels{ nullptr };
				if (collc.getCollisionType() == CollisionType::COLLISION_PIXELS)
				{
					pixels = ge.getPixels(box.GetID());
				}

				//paso datos al collider
				collc.setCollisionType(collc.getCollisionType(), rc3.getPosition(), rc3.getSize(), rc3.getPivot(), pixels);
				collc.getCollider()->updatePosition(pc3.position, pc3.scale);
				//reiniciamos timer
				aic.elapsed_throw = 1.f;
			}
		}
	});
}