#pragma once
#ifndef EVENT_SYSTEM
#define EVENT_SYSTEM
#include "../utils/types.hpp"
#include "../managers/EventManager.hpp"

struct EventSystem
{
  void update(EntityManager& em, EventManager& evm,AudioSource& as,ExerciseManager& exm,RenderSystem& rsys, AnimationSystem& asys, CollisionSystem& collsys,GameEngine& ge,MapManager& mapm) { evm.dispatchEvents(em,as,exm,rsys,asys,collsys,ge,mapm); };
};

#endif // !EVENT_SYSTEM
