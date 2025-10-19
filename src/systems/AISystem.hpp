#pragma once
#include "../utils/types.hpp"

struct AISystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<AIComponent,PhysicsComponent,RenderComponent>;
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em,GameEngine& ge, float dt);
};


