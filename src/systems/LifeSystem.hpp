#pragma once
#include "../utils/types.hpp"

struct LifeSystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<LifeComponent>;
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em, float dt);
};

