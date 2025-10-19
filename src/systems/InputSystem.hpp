#pragma once
#include "../utils/types.hpp"
#include "../managers/EventManager.hpp"
struct InputSystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<PhysicsComponent,InputComponent>; //InputComponent
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em, GameEngine& ge,EventManager& evm);
};

