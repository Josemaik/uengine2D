#include "InputSystem.hpp"

void InputSystem::update(EntityManager& em, GameEngine& ge, EventManager& evm)
{
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& pc,InputComponent& ic)//InputComponent& ic
	{
			////////////////
			//MOVEMENT
			auto& playerInfo = em.getSingleton<PlayerInfo>();
			bool isMoving = false;
			auto& vel = pc.vel;
			//Reset velocity
			/*pc.vel = {};*/
			//auto& speed = pc.speed;
			if (ge.IsKeyDown(ic.move_left))
			{
				pc.direction_scale = -1.f;
				//pc.position.m_x += 128.f;
				//vel.setX(vel.m_x - speed);
				isMoving = true;
			}
			if (ge.IsKeyDown(ic.move_right))
			{
				pc.direction_scale = 1.f;
				//pc.position.m_x -= 128.f;
				//vel.setX(vel.m_x + speed);
				isMoving = true;
			}
			if (isMoving)
			{
				auto& gd = em.getSingleton<GameData>();
				gd.playerDirection = pc.direction_scale;
			}
			//Chequeamos si el player se ha movido o no
			if (isMoving && playerInfo.currentState != PlayerState::RUNNING) {
				pc.vel = {100.f,0.f};
				auto& rc = em.getComponent<RenderComponent>(e);
				playerInfo.currentState = PlayerState::RUNNING;
				vec2f size = ge.SetTexturetoEnt(e.GetID(), PLAYER_RUN);
				auto& ac = em.getComponent<AnimationComponent>(e);
				////////////
				//ac.setHframes(8);
				size.m_x /= ac.getHframes();//->personaje nuevo
				///////////
				
				//size.m_x = 32.f;-> personaje antiguo
				pc.size = size;
				rc.setSize(size);
			}

			if (!isMoving && playerInfo.currentState != PlayerState::IDLE) {
				pc.vel = {};
				auto& rc = em.getComponent<RenderComponent>(e);
				playerInfo.currentState = PlayerState::IDLE;
				vec2f size = ge.SetTexturetoEnt(e.GetID(), PLAYER_IDLE);
				//size.m_x = 32.f;/*
				//size.m_y = 45.f;
				//size.m_x = 128.f; //32 en todo
				//size.m_y = 96.f;
				//pc.size.m_x = 128.f;
				//pc.size.m_y = 96.f;
				/*auto& ac = em.getComponent<AnimationComponent>(e);
				ac.hframes = 3;
				ac.currentFrame = 0;
				size.m_x /= ac.getHframes() + 1;*/
				pc.size = size;
				rc.setSize(size);
			}

			////////////////
			//JUMP
			if (ge.IsKeyPressed(ic.jump))
			{
				if (pc.onground)
				{
					//pc.jumping = true;
					vel.m_y = -250.f;
					pc.onground = false;
				}
			}
			if (ge.IsKeyReleased(ic.jump))
			{
				vel.m_y = 0.f;
				//pc.jumping = false;
			}

			////////////////
			///TeSt shuriken

			if (ge.IsMouseButtomPressed(ic.attack))
			{
				//get gamedata and sound system
				//auto& ss = em.getSingleton<AudioSystem>();
				auto& gd = em.getSingleton<GameData>();
				//play sound
				evm.scheduleEvent(Event{ EventCodes::SpawnShuriken });
				//ss.Play(SoundID::SHURIKEN_THROW);
				//Create shuriken entity
				auto& shuriken{ em.createEntity() };
				em.addTag<shurikenTag>(shuriken);
				//Calcular dirección
				vec2f mouse = vec2f{ static_cast<float>(gd.mouse_x),static_cast<float>(gd.mouse_y) }; //mouse
				//printf("mouse: %d, %d", mouse.m_x, mouse.m_y);
				vec2f shurikenvel{};
				/*if (mouse.m_y < gd.playerPos.m_y)
				{*/
					shurikenvel = vec2f{ 100.f * gd.playerDirection, mouse.m_y - gd.playerPos.m_y };
				/*}
				else {
					shurikenvel = vec2f{ 100.f * gd.playerDirection, 50.f };
				}*/
				//vec2f player = vec2f{ gd.playerPos.m_x,gd.playerPos.m_y };
				//vec2f dir = player - mouse;
				//r.normalize();
				auto& pc2 = em.addComponent<PhysicsComponent>(shuriken, PhysicsComponent{ .position = vec2f(gd.playerPos.m_x + 20.f,gd.playerPos.m_y + 20.f),.scale = vec2f(1.f,1.5f),.vel = shurikenvel });//100.f * gd.playerDirection,0.f
				auto& rc2 = em.addComponent<RenderComponent>(shuriken, RenderComponent{ .position = pc2.position,.scale = pc2.scale, .blendmode = BLEND_ALPHA,.u0 = 0.f,.v0 = 0.f,.u1 = 1.f,.v1 = 1.f,.pivot = vec2f(0.5f,0.5f) });
				em.addComponent<LifeComponent>(shuriken, LifeComponent{ .countdown_die = 4.f });
				auto& collc = em.addComponent<ColliderComponent>(shuriken, ColliderComponent{ CollisionType::COLLISION_CIRCLE });			
				auto& ac = em.addComponent<AnimationComponent>(shuriken, AnimationComponent{ .hframes = 2,.vframes = 1,.framerate = 1.f / 8.0f, .currentFrame = 0 });
				//auto& ac = em.addComponent<AnimationComponent>(shuriken, AnimationComponent{ .hframes = 2,.vframes = 1,.framerate = 1.f / 8.0f, .currentFrame = 0 });
				ac.CalculateUVs();
				auto size = ge.SetTexturetoEnt(shuriken.GetID(), TexturesType::SHURIKEN);
				rc2.setSize(size);
				pc2.size = size;

				unsigned char* pixels{ nullptr };
				if (collc.getCollisionType() == CollisionType::COLLISION_PIXELS)
				{
					pixels = ge.getPixels(shuriken.GetID());
				}

				//paso datos al collider
				collc.setCollisionType(collc.getCollisionType(), rc2.getPosition(), rc2.getSize(), rc2.getPivot(), pixels);
				collc.getCollider()->updatePosition(pc2.position, pc2.scale);
				//collc.setCollisionType(CollisionType::COLLISION_CIRCLE, pc2.position, pc2.size, pc2.pivot, pc2.scale);
			}
			
			//Si ganamos, comprobar ENTER para reiniar juego
			auto& gd = em.getSingleton<GameData>();
			if (gd.victory)
			{
				if (ge.IsKeyPressed(ic.restart))
				{
					//Respawn enemies and objects
					evm.scheduleEvent(Event{ EventCodes::RestartGame });
					gd.victory = false;
					//spawn pos inicial
					pc.position = vec2f{ 20.f,center_screen_y - 150.f };
					//score to 0
					gd.score = 0;
				}
			}

			if (ge.IsKeyPressed(GLFW_KEY_R))
			{
				pc.position = vec2f{ 3046.f,401.f };
			}
			
	});
}
