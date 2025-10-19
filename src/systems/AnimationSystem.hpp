#pragma once
#include "../utils/types.hpp"

struct AnimationSystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<AnimationComponent>;
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em, float dt);
  void SetupUVs(EntityManager& em);
};

