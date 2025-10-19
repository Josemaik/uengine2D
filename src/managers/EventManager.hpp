#pragma once

#include <vector>
#include <cstdint>
#include "../utils/types.hpp"
#include "../systems/AudioSystem.hpp"
#include "../managers/ExerciseManager.hpp"
#include "../systems/AnimationSystem.hpp"
#include "../systems/RenderSystem.hpp"
#include "../systems/CollisionSystem.hpp"

struct Event {
  EventCodes code; // Código identificador del evento
};

struct EventManager
{
  public:
    // Programa un evento para ser despachado tan pronto como sea posible
    void scheduleEvent(const Event& event) {
      events.push_back(event);
    }

    // Dispara todos los eventos pendientes
    void dispatchEvents(EntityManager& em,AudioSource& as,ExerciseManager& exm,RenderSystem& rsys,AnimationSystem& asys,CollisionSystem& collsys,GameEngine& ge,MapManager& map) {
      // Recorre todos los eventos pendientes
      bool out = false;
      while (!events.empty() && !out) {
        // Obtiene el siguiente evento y lo elimina de la cola
        Event& event = events.back();
        events.pop_back();

        using CMPs = MP::TypeList<ListenerComponent>;
        using noTag = MP::TypeList<>;

        // Notifica a todos los listeners que estén interesados en el evento
        em.forEach<CMPs, noTag>([&](Entity& e, ListenerComponent& lc)
        {
          if (lc.hasCode(event.code))
          {
            switch (event.code)
            {
            case EventCodes::RestartGame: {   
              //Eliminamos entidades recolectables y enemigos
              for (auto& ent : em.getEntities())
              {
                if (ent.hasTag<recolectableTag>() || ent.hasTag<enemyTag>())
                {
                  em.destroyEntity(ent.GetID());
                }
              }
              //Los creamos de nuevo
              exm.createObjects(em);
              exm.CreateEnemy(em);
              //set up texturas,colliders y animaciones
              rsys.LoadTextures(em, ge, map);
              collsys.setColliders(em, ge, map);
              asys.SetupUVs(em);
            }
              break;
            case EventCodes::SpawnShuriken: as.Play(SoundID::SHURIKEN_THROW);
              break;
            default: (void)e;
              break;
            }
          }
        });
      }
    }

    void reset() { events.clear(); }

  private:
    // Cola de eventos pendientes
    std::vector<Event> events;
};