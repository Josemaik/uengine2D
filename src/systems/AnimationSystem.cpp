#include "AnimationSystem.hpp"

void AnimationSystem::update(EntityManager& em, float dt)
{
	auto& pi = em.getSingleton<PlayerInfo>();
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, AnimationComponent& ac)
	{
			if (pi.currentState == PlayerState::RUNNING || e.hasTag<shurikenTag>() || e.hasTag<foxTag>() || e.hasTag<beeTag>() || e.hasTag<coinTag>()) //actualizar animación si se mueve el player y entidades animables
			{
				// Actualizar el currentFrame
				float newframe = ac.getCurrentFrame() + (dt / ac.framerate);
				ac.setCurrentFrame(newframe);

				// Si el frame actual excede el número total de frames, reiniciar
				int totalFrames = ac.getHframes() * ac.getVframes();
				if (static_cast<int>(ac.getCurrentFrame()) >= totalFrames)
				{
					ac.setCurrentFrame(0);
				}

				int currentFrameIndex = static_cast<int>(ac.getCurrentFrame());
				//Las almacenamos en el RenderComponent
				auto& rc = em.getComponent<RenderComponent>(e);
				rc.u0 = ac.UCoords.at(currentFrameIndex).m_x;
				rc.u1 = ac.UCoords.at(currentFrameIndex).m_y;

			}
			else {
				auto& rc = em.getComponent<RenderComponent>(e);
				rc.u0 = 0.f; rc.v0 = 0.f;
				rc.u1 = 1.f; rc.v1 = 1.f;
			}

			//Si hay mas de una fila
			/*if (ac.getVframes() > 1)
			{
				int row = currentFrameIndex / ac.getHframes();
				rc.v0 = row / static_cast<float>(ac.getVframes());
				rc.v1 = (row + 1) / static_cast<float>(ac.getVframes());
			}*/
			
		});
}

void AnimationSystem::SetupUVs(EntityManager& em)
{
	using SYSCMPs = MP::TypeList<AnimationComponent>;
	em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e,AnimationComponent& ac)
	{
			if (e.hasTag<playerTag>() || e.hasTag<shurikenTag>() || e.hasTag<foxTag>() || e.hasTag<beeTag>() || e.hasTag<coinTag>())
			{
				// Calcular las coordenadas de textura
				for (int i = 0; i < ac.getHframes(); i++)
				{
					int column = i % ac.getHframes();
					float u0 = column / static_cast<float>(ac.getHframes());
					float u1 = (column + 1) / static_cast<float>(ac.getHframes());
					ac.UCoords.push_back(vec2f{ u0,u1 });
				}
			}
		
	});
}