#include "GameEngine.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

int main() {
#if defined(_WIN32) || defined(_WIN64)
  SetConsoleOutputCP(CP_UTF8); SetConsoleCP(CP_UTF8);
#endif
  std::srand(static_cast<unsigned int>(std::time(0)));
  GameEngine engine;
  engine.InitGame();
  while (true) {
    engine.RunRein();
    std::cout << "\n[7]查看石碑並退出 / [0]下一世: ";
    int op; std::cin >> op; if (op == 7) { engine.EndGame(); break; }
  }
  return 0;
}