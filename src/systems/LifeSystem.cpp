#include "LifeSystem.hpp"

void LifeSystem::update(EntityManager& em, float dt)
{
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, LifeComponent& lc)
	{
			//Los muertos se destruye su entidad
			if (lc.decreaseTimer(dt, lc.elapsed_die))
			{
				lc.dead = true;
			}

			if (lc.dead)
			{
				//destruimos entidad
				em.destroyEntity(e.GetID());
			}
	});
}