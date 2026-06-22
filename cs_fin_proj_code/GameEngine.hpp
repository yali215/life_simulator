#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "Components.hpp"

class GameEngine {
private:
  Player pl; Character c; int birth_type;
public:
  void InitGame();
  bool CheckDeathRisk();
  void RunRein();
  void EndGame();
};

#endif