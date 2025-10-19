#pragma once

#include<cstdint>
//Colliders
#include "../utils/colliders/CircleCollider.hpp"
#include "../utils/colliders/RectCollider.hpp"
#include "../utils/colliders/PixelsCollider.hpp"

enum CollisionType : uint8_t
{
	COLLISION_NONE,
	COLLISION_CIRCLE,
	COLLISION_RECT,
	COLLISION_PIXELS
};

struct ColliderComponent
{
  // Constructor por defecto
  ColliderComponent() = default;

  // Constructor parametrizado
  ColliderComponent(CollisionType _type) 
    : collisiontype(_type)
  {
   //Asignamos el nuevo collider
  // setCollider(spritePos,spriteSize,pivot,scale,pixelData);
  }

  // Destructor
  ~ColliderComponent()
  {
    printf("HE liberado Collider\n");
    delete collider;
    collider = nullptr;
  }

  // Constructor de copia
  /*ColliderComponent(const ColliderComponent& other) : collisiontype(other.collisiontype)
  {
    copyCollider(other.collider);
  }*/

  // Operador de asignación
  ColliderComponent& operator=(const ColliderComponent& other)
  {
    if (this != &other) // Evitar autoasignación
    {
      delete collider; // Liberar la memoria actual
      collider = nullptr;
      collisiontype = other.collisiontype;
      //copyCollider(other.collider);
    }
    return *this;
  }

  // Método para configurar el collider
  void setCollider(const vec2f spritePos, const vec2f spriteSize, const vec2f pivot, const uint8_t* pixelData = nullptr)
  {
    if (collider)
    {
      delete collider; // Limpiar memoria anterior
      collider = nullptr;
    }
   
    float radius{ 0.f };
    switch (collisiontype)
    {
    case COLLISION_CIRCLE:
      radius = std::fmaxf(spriteSize.m_x, spriteSize.m_y) * 0.5f;
      collider = new CircleCollider(radius);
      printf("CircleCollider creado\n");
      break;
    case COLLISION_RECT:
      collider = new RectCollider(pivot, spriteSize, spritePos);
      printf("RectCollider creado\n");
      break;
    case COLLISION_PIXELS:
      collider = new PixelsCollider(spriteSize, pivot,pixelData);
      printf("PixelsCollider creado\n");
      break;
    default:
      collider = nullptr;
      break;
    }
  }

  void setCollisionType(CollisionType type, const vec2f& spritePos, const vec2f& spriteSize, const vec2f& pivot, const uint8_t* pixelData = nullptr)
  {
    if(type != COLLISION_NONE)
      collisiontype = type;

    setCollider(spritePos,spriteSize,pivot,pixelData);
  }

  CollisionType getCollisionType() const { return collisiontype; }
  Collider* getCollider() const { return collider; }

private:
  //void copyCollider(const Collider* otherCollider)
  //{
  //  if (otherCollider)
  //  {
  //    //Rellenar
  //  }
  //}

  CollisionType collisiontype{ COLLISION_NONE };
  Collider* collider{ nullptr };
};