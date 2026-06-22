#include "Global.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void PrintLine(std::string text, int delay_ms) {
  std::cout << text << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

std::string GetRealmName(int r) {
  switch (r) {
    case REALM_MORTAL: return "凡人";
    case REALM_QI_REFINING: return "練氣期";
    case REALM_FOUNDATION: return "築基期";
    case REALM_GOLDEN_CORE: return "金丹期";
    case REALM_NASCENT_SOUL: return "元嬰期";
    case REALM_SOUL_FORMATION: return "化神期";
    case REALM_INTEGRATION: return "合體期";
    case REALM_TRIBULATION: return "渡劫期";
    case REALM_MAHAYANA: return "大乘期";
    case REALM_EARTH_IMMORTAL: return "地仙";
    case REALM_HEAVEN_IMMORTAL: return "天仙";
    case REALM_GOLDEN_IMMORTAL: return "金仙";
    case REALM_CHAOS_IMMORTAL: return "混元無極仙";
    default: return "未知";
  }
}

int GetMaxAge(int realm) {
  switch (realm) {
    case REALM_MORTAL: return 85;
    case REALM_QI_REFINING: return 150;
    case REALM_FOUNDATION: return 250;
    case REALM_GOLDEN_CORE: return 500;
    case REALM_NASCENT_SOUL: return 1000;
    case REALM_SOUL_FORMATION: return 2000;
    case REALM_INTEGRATION: return 5000;
    case REALM_TRIBULATION: return 10000;
    default: return 999999;
  }
}