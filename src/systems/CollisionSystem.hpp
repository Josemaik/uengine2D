#pragma once
#include "../utils/types.hpp"

struct EventManager;
struct MapManager;

struct CollisionSystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<PhysicsComponent,ColliderComponent>; //InputComponent
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em, MapManager& map);
  void setColliders(EntityManager& em, GameEngine& ge, MapManager& map);
};

