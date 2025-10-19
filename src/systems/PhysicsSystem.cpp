#include "PhysicsSystem.hpp"

//globales
static float dist_to_mouse = 60.f;


void PhysicsSystem::update(EntityManager& em,float& dt)
{
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& pc)
	{
    auto& vel = pc.vel;
    
    if (e.hasTag<boxTag>())
    {
      pc.vel.m_y += pc.gravity * dt;
    }

    auto& gd = em.getSingleton<GameData>();
    if (e.hasTag<playerTag>()) {
      //Si me salgo de los limites ,directionscale * -1, para que se mueva en direccion contraria
      /*if (positionX < 10 || positionX > 190)
        velocityX *= -1;*/
      printf("Hola\n");
      //Timer spawn
      if (gd.lose)
      {
        if (gd.decreaseTimer(dt, gd.elapsed_spawn))
        {
          printf("Hola");
          pc.position = gd.current_spawnpos; //Seteo al player
          gd.lose = false;
          gd.elapsed_spawn = 1.f;
        }
      }

      // Aplicar gravedad si está en el aire
      if (!pc.onground) {
        //vel.m_y += pc.gravity;  // Aceleración progresiva hacia abajo
        pc.vel.m_y += pc.gravity * dt;
      }
      //else {
      //  // Aterriza en el suelo
      //  pc.position.m_y = pc.startjump_y;
      //  pc.vel.m_y = 0;
      //  pc.jumpTimer = 0;
      //}

        //Lógica de salto con impulso gradual
      //if (pc.jumping && pc.position.m_y == pc.startjump_y) {
      //  pc.vel.m_y = pc.jumpImpulse;  // Impulso inicial solo una vez
      //  pc.jumping = false;       // Evitar salto continuo
      //}

      gd.playerPos = pc.position;
    }

    // Actualizar posición
    pc.prevPosition = pc.position;
    //pc.position += pc.vel;
    if (gd.lose == false && gd.victory == false)
    {
      float dirscale = pc.direction_scale;

      if (e.hasTag<foxTag>())
        dirscale = 1.f;

      pc.position.m_x += dirscale * vel.m_x * dt;
      pc.position.m_y += vel.m_y * dt;
    }
    

		if (e.hasTag<cloudsTag>() && e.hasComponent<ScrollComponent>())
		{
			auto& scrollcmp = em.getComponent<ScrollComponent>(e);
			/*pc.position.m_x += scrollcmp.getScrollSpeed().m_x * dt;
			pc.position.m_y += scrollcmp.getScrollSpeed().m_y * dt;*/
			scrollcmp.scrollOffset.m_x += scrollcmp.scrollSpeed.m_x * dt;
			scrollcmp.scrollOffset.m_y += scrollcmp.scrollSpeed.m_y * dt;
		}
	});
}
