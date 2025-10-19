#pragma once

#include "../utils/types.hpp"

struct PhysicsSystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<PhysicsComponent>;
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em,float& dt);
};