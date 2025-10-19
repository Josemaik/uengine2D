#pragma once

#include "../utils/types.hpp"

/////////////////////////////////////////////////
/// ESTA CLASE MANEJA LA CREACIÓN DE ENTIDADES

struct ExerciseManager
{
	////////////
	//PLAYER
	void CreatePlayer(EntityManager& em)
	{
		auto& player{ em.createEntity() };
		em.addTag<playerTag>(player);
		//vec2f(1300.f,241.f)
		//vec2f(20.f,center_screen_y - 150.f)
		//vec2f(3297.f,497.f)
		auto& pc2 = em.addComponent<PhysicsComponent>(player, PhysicsComponent{ .startjump_y = center_screen_y + 115,.position = vec2f(20.f,center_screen_y - 150.f),.scale = vec2f(1.f,1.f) });
		em.addComponent<RenderComponent>(player, RenderComponent{ .position = pc2.position,.scale = pc2.scale, .blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.5f,0.5f) });
		em.addComponent<ColliderComponent>(player, ColliderComponent{ CollisionType::COLLISION_PIXELS });
		em.addComponent<AnimationComponent>(player, AnimationComponent{ .hframes = 8,.vframes = 1,.framerate = 1.f / 8.0f, .currentFrame = 0 });
		em.addComponent<InputComponent>(player);

		//Añadimos los códigos de evento a nuestro componente
		auto& lc = em.addComponent<ListenerComponent>(player);
		for (auto i = 0; i < EventCodes::SpawnShuriken; i++)
			lc.addCode(static_cast<EventCodes>(i));
	}
	/////////////
	//MAP
	void CreateMap(EntityManager& em)
	{
		auto& clouds{ em.createEntity() };
		em.addTag<clouds2Tag>(clouds);
		em.addTag<scrollTag>(clouds);
		auto& pc3 = em.addComponent<PhysicsComponent>(clouds, PhysicsComponent{ .position = vec2f(center_screen_x,center_screen_y),.scale = vec2f(1.f,1.f) });
		em.addComponent<RenderComponent>(clouds, RenderComponent{ .position = pc3.position,.scale = pc3.scale, .blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.f,0.f) });
		em.addComponent<ScrollComponent>(clouds, ScrollComponent{ .scrollRatio = 0.1f});

	}
	//////////
	//OBJECTS
	void createObjects(EntityManager& em)
	{
		
		CreateCoin(em,490.f, 305.f);
		CreateCoin(em, 1045.f, 240.f);
		CreateCoin(em, 1885.f, 180.f);
		CreateCoin(em, 2400.f, 300.f);
		CreateCoin(em, 2850.f, 400.f);
		CreateCoin(em, 3270.f, 500.f);

		auto& flag{ em.createEntity() };
		em.addTag<flagTag>(flag);
		auto& pc5 = em.addComponent<PhysicsComponent>(flag, PhysicsComponent{ .position = vec2f(3419.f,450.f),.scale = vec2f(1.f,1.f) });
		em.addComponent<RenderComponent>(flag, RenderComponent{ .position = pc5.position,.scale = pc5.scale,.blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.f,0.f) });
		em.addComponent<ColliderComponent>(flag, ColliderComponent{ CollisionType::COLLISION_RECT });
	}
	void CreateCoin(EntityManager& em, float posX, float posY)
	{
		auto& coin{ em.createEntity() };
		em.addTag<coinTag>(coin);
		em.addTag<recolectableTag>(coin);
		auto& pc = em.addComponent<PhysicsComponent>(coin, PhysicsComponent{ .position = vec2f(posX,posY),.scale = vec2f(3.f,3.f) });
		em.addComponent<RenderComponent>(coin, RenderComponent{ .position = pc.position,.scale = pc.scale, .blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.f,0.f) });
		em.addComponent<ColliderComponent>(coin, ColliderComponent{ CollisionType::COLLISION_CIRCLE });
		em.addComponent<AnimationComponent>(coin, AnimationComponent{ .hframes = 7,.vframes = 1,.framerate = 1.f / 8.0f, .currentFrame = 0 });
	}
	//////////
	//ENEMIES
	void CreateEnemy(EntityManager& em)
	{
		CreateFox(em,766.f);
		CreateFox(em, 1542.f);
		CreateFox(em, 2720.f);
		CreateBee(em,1066.f,5.f);
		CreateBee(em,	1913.f,3.5f);
		CreateBee(em, 3076.f,3.f);
	}
	void CreateFox(EntityManager& em, float posSpawnX)
	{
		auto& fox{ em.createEntity() };
		em.addTag<foxTag>(fox);
		em.addTag<enemyTag>(fox);
		auto& pc2 = em.addComponent<PhysicsComponent>(fox, PhysicsComponent{ .startjump_y = center_screen_y + 115,.position = vec2f(posSpawnX,center_screen_y + 105.f),.scale = vec2f(1.f,1.f),.direction_scale = -1.f });
		//auto& pc2 = em.addComponent<PhysicsComponent>(fox, PhysicsComponent{ .startjump_y = center_screen_y + 115,.position = vec2f(1400.f,center_screen_y + 80.f),.scale = vec2f(1.f,1.f) });
		em.addComponent<RenderComponent>(fox, RenderComponent{ .position = pc2.position,.scale = pc2.scale, .blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.5f,0.5f) });
		em.addComponent<ColliderComponent>(fox, ColliderComponent{ CollisionType::COLLISION_RECT });
		em.addComponent<AnimationComponent>(fox, AnimationComponent{ .hframes = 8,.vframes = 1,.framerate = 1.f / 6.0f, .currentFrame = 0 });
		em.addComponent<AIComponent>(fox, AIComponent{ .detect_radius = 15.f,.patrol_position = pc2.position });
	}
	void CreateBee(EntityManager& em, float posSpawnX,float countdown_thwow)
	{
		auto& bee{ em.createEntity() };
		em.addTag<beeTag>(bee);
		em.addTag<enemyTag>(bee);
		auto& pc = em.addComponent<PhysicsComponent>(bee, PhysicsComponent{ .position = vec2f(posSpawnX,center_screen_y - 300.f),.scale = vec2f(1.f,1.f) });
		em.addComponent<RenderComponent>(bee, RenderComponent{ .position = pc.position,.scale = pc.scale,.blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f, .pivot = vec2f(0.f,0.f) });
		em.addComponent<AnimationComponent>(bee, AnimationComponent{ .hframes = 8,.vframes = 1,.framerate = 1.f / 8.0f, .currentFrame = 0 });
		em.addComponent<AIComponent>(bee, AIComponent{.countdown_throw = countdown_thwow });
	}
};