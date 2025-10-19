#pragma once

#include "../utils/types.hpp"

//fordward declaration
struct MapManager;

struct RenderSystem
{
  // Se van a buscar las entidad que tengan estos componentes y tags
  using SYSCMPs = MP::TypeList<PhysicsComponent, RenderComponent>;
  using SYSTAGs = MP::TypeList<>;

  void update(EntityManager& em,GameEngine& engine,MapManager& map);
  //Draw
  void DrawMap(GameEngine& ge,MapManager& map);
  void DrawFonts(GameEngine& ge, EntityManager& em);
  void DrawTiled(int numtiles,RenderComponent& rc, GameEngine& ge,size_t id_ent);
  //void DrawTiledRot(int numtiles, RenderComponent& rc, GameEngine& ge, size_t id_ent);
  //Load
  void LoadTextures(EntityManager& em,GameEngine& ge, MapManager& map);
  void LoadFonts(GameEngine& ge);
  //Free
  void freeTextures(GameEngine& ge);
};