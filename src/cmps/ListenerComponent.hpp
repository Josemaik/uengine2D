#pragma once
#include <cstdint>
#include <unordered_set>

struct ListenerComponent
{
  // Funci�n para a�adir un c�digo de evento
  void addCode(uint16_t code) { codeSet.insert(code); }

  // Funci�n para quitar un c�digo de evento
  void removeCode(uint16_t code) { codeSet.erase(code); }

  // Funci�n para comprobar si un c�digo de evento est� registrado
  bool hasCode(uint16_t code) const { return codeSet.find(code) != codeSet.end(); }

private:
  // Set para a�adir c�digos
  std::unordered_set<uint16_t> codeSet;
};