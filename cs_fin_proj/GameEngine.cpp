#include "GameEngine.hpp"
#include "EventManager.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

bool IsAllWhitespace(const std::string& str) {
    if (str.empty()) return true;
    for (char c : str) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return false; // 只要有一個不是空白的字元，就是合法的
        }
    }
    return true;
}

void GameEngine::InitGame() {
  std::cout << "【天道系統】請輸入執掌天命的玩家名稱: ";
  std::string p_name;
  std::getline(std::cin, p_name);
  while (IsAllWhitespace(p_name)) {
    std::cout << "輸入無效，名稱不可為空或純空白！\n";
    std::cout << "【天道系統】請重新輸入玩家名稱: ";
    std::getline(std::cin, p_name);
  }
  pl.Login(p_name);
}

bool GameEngine::CheckDeathRisk() {
  if (c.HasTrait(LONG_AOTIAN) && (std::rand() % 100 < 20)) {
    c.SetDead("因太過囂張引來渡劫大能一掌拍死。【剛過易折暴斃】");
    pl.SaveEnd(END_AOTIAN_FALL);
    return true;
  }
  return false;
}

void GameEngine::RunRein() {
  c.Initial(); pl.AddRein();
  std::cout << "\n========================================" << std::endl;
  std::cout << "請輸入本次輪迴的轉世道號: ";
  std::string ch_name;
  std::getline(std::cin, ch_name);
  while (IsAllWhitespace(ch_name)) {
    std::cout << "輸入無效，名稱不可為空或純空白！\n";
    std::cout << "【天道系統】請重新輸入玩家名稱: ";
    std::getline(std::cin, ch_name);
  }
  c.SetName(ch_name);

  // 1. 繼承詞條選擇
  std::vector<Trait> past_traits = pl.GetHeritage();
  if (!past_traits.empty()) {
    std::cout << "\n【因果宿命】：檢測到上一世留存的所有詞條因果：" << std::endl;
    for (size_t i = 0; i < past_traits.size(); i++) {
      std::cout << "[" << i + 1 << "]: " << past_traits[i].GetName() << std::endl;
    }
    std::cout << "請輸入你想繼承的詞條編號（一行為一個，輸入 0 結束繼承選擇）：" << std::endl;
    while (c.GetGifts().size() < 6) {
      int choice;
      std::cin >> choice;
      if (choice == 0) {
        break;
      }
      if (choice >= 1 && choice <= static_cast<int>(past_traits.size())) {
        Trait t = past_traits[choice - 1];
        if (c.HasTrait(t.GetID())) {
          std::cout << "不能重複選擇相同的繼承詞條！" << std::endl;
          continue;
        }
        c.PushGift(t);
        std::cout << "已繼承：[" << t.GetName() << "] (目前配置 " << c.GetGifts().size() << "/6)" << std::endl;
      } else {
        std::cout << "無效編號，請重新輸入。" << std::endl;
      }
    }
  }
  pl.ClearHeritage();

  // 2. 隨機15抽天賦
  std::vector<int> available_pool;
  for (int i = 0; i < 35; i++) { if (!c.HasTrait(static_cast<TraitID>(i))) available_pool.push_back(i); }
  std::random_shuffle(available_pool.begin(), available_pool.end());
  std::cout << "\n【天道】：為你顯化 15 種天地詞條..." << std::endl;
  for (int i = 0; i < 15; i++) {
    Trait t; t.SetTraitID(static_cast<TraitID>(available_pool[i]));
    std::cout << "[" << i + 1 << "]: " << t.GetName() << " ";
  }
  std::cout << "\n輸入編號配置天賦，輸入 's' 開始本世：" << std::endl;
  while (c.GetGifts().size() < 6) {
    std::cout << "請輸入詞條編號 (目前格位 " << c.GetGifts().size() << "/6) 或輸入 's' 開始遊戲：";
    std::string input;
    std::cin >> input; 
    if (input == "s" || input == "S") break;
    int idx = std::atoi(input.c_str());
    if (idx >= 1 && idx <= 15) {
      TraitID sel_id = static_cast<TraitID>(available_pool[idx - 1]);
      if (c.HasTrait(sel_id)) {
        std::cout << "不能重複選擇相同的隨機詞條！" << std::endl;
        continue;
      }

      Trait t; 
      t.SetTraitID(sel_id); 
      c.PushGift(t); 
      pl.SaveTrait(sel_id);
      c.AddStats(t.GetS(), t.GetI(), t.GetP(), t.GetL(), t.GetF(), t.GetFa());
      std::cout << "已選定：[" << t.GetName() << "] (目前配置 " << c.GetGifts().size() << "/6)" << std::endl;
    } else if (idx == 0) {
      break;
    } else {
      std::cout << "無效編號，請重新輸入 1~15 或者是 's'。" << std::endl;
    }
  }

  // 3. 出生背景
  birth_type = 1;
  if (c.HasTrait(ANCIENT_ANCESTOR)) {
    PrintLine("\n【特殊宿命】：你覺醒了[出山老祖]記憶，直接登臨元嬰期！");
    c.SetAge(200); c.GetStat().SetRealm(REALM_NASCENT_SOUL); c.camp = CAMP_SECT;
  } else {
    std::cout << "\n請選擇出生背景：\n1. 平凡人家 (15歲覺醒門戶) \n2. 富甲商賈 (13歲覺醒門戶) \n3. 修仙世家 (9歲覺醒門戶)" << std::endl;
    std::cin >> birth_type;
    if (birth_type == 2) c.AddStats(2, 2, 2, 4, 10, 3);
    else if (birth_type == 3) { c.AddStats(2, 8, 2, 4, 3, 5); c.GetStat().SetRealm(REALM_QI_REFINING); }
    else c.AddStats(4, 4, 4, 4, 4, 4);
  }

  // -------------------------------------------------------------------------
  // 🔥 【投胎儀式感：強制置換靈根與出生報告】
  // 在 0 歲生命週期正式開始前，呼叫事件管理器以觸發詞條覆蓋與性別靈根宣告。
  // -------------------------------------------------------------------------
  if (!c.HasTrait(ANCIENT_ANCESTOR)) {
    EventManager::TriggerAgeEvent(c, pl, birth_type);
  }

  // 4. 生命大循環
  while (c.IsAlive()) {
    int current_age = c.GetAge();
    int current_realm = c.GetStat().GetRealm();

    if (current_realm == REALM_MAHAYANA) {
      int total_stat = c.GetStat().GetStr() + c.GetStat().GetWis() + c.GetStat().GetPhy() + c.GetStat().GetLuck();
      if (total_stat >= 300) {
        if (c.HasTrait(SWORD_MASTER)) { c.SetDead("一劍破萬法，【登臨至高：一代劍聖】！"); pl.SaveEnd(END_SWORD_GOD); }
        else { c.SetDead("天劫自散，踏碎虛空，【混元無極大圓滿】！"); pl.SaveEnd(END_CHAOS_MASTER); }
        break;
      }
    }

    int max_age_allowed = GetMaxAge(current_realm);
    if (c.HasTrait(GINSENG_SPIRIT)) max_age_allowed += 200;
    if (current_age >= max_age_allowed) {
      c.SetDead(current_realm < REALM_EARTH_IMMORTAL ? "大限已至，不甘化為一抔黃土。【平凡一生】" : "在仙界浩劫中與仙域一同歸於寂滅。【萬古孤寂】");
      pl.SaveEnd(current_realm < REALM_EARTH_IMMORTAL ? END_ORDINARY_LIFE : END_ANCIENT_REGRET);
      break;
    }

    // 只有非出山老祖在 0 歲之後、或是老祖本人，才進行正常歲數增長與事件觸發
    if (current_age > 0 || c.HasTrait(ANCIENT_ANCESTOR)) {
      int time_step = (current_age < 30) ? (1 + std::rand() % 2) : (3 + std::rand() % 8);
      c.GrowAge(time_step);

      // 呼叫外部解耦的事件管理器
      EventManager::TriggerAgeEvent(c, pl, birth_type);
    } else {
      // 確保 0 歲投胎報告印完後，下一次迴圈能順利進入 1~2 歲
      c.GrowAge(1 + std::rand() % 2);
    }

    // 突破邏輯判定
    current_realm = c.GetStat().GetRealm();
    if (current_realm == REALM_MORTAL && c.GetStat().GetWis() >= 20) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [引氣入體] 成功破境步入【練氣期】！"); c.GetStat().SetRealm(REALM_QI_REFINING);
    } else if (current_realm == REALM_QI_REFINING && c.GetStat().GetWis() >= 45) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [破境築基] 晉升【築基期】！"); c.GetStat().SetRealm(REALM_FOUNDATION);
    } else if (current_realm == REALM_FOUNDATION && c.GetStat().GetWis() >= 80) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [凝聚金丹] 一粒金丹吞入腹！晉升【金丹期】！"); c.GetStat().SetRealm(REALM_GOLDEN_CORE);
    } else if (current_realm == REALM_GOLDEN_CORE && c.GetStat().GetWis() >= 130) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [破丹成嬰] 晉升【元嬰期】！"); c.GetStat().SetRealm(REALM_NASCENT_SOUL);
    } else if (current_realm == REALM_NASCENT_SOUL && c.GetStat().GetWis() >= 190 && c.GetStat().GetPhy() >= 140) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [出神化神] 晉升【化神期】！"); c.GetStat().SetRealm(REALM_SOUL_FORMATION);
    } else if (current_realm == REALM_SOUL_FORMATION && c.GetStat().GetWis() >= 250 && c.GetStat().GetStr() >= 200) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [身合天地] 晉升【合體期】！"); c.GetStat().SetRealm(REALM_INTEGRATION);
    } else if (current_realm == REALM_INTEGRATION && c.GetStat().GetWis() >= 320 && c.GetStat().GetPhy() >= 280) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [逆天渡劫] 晉升【渡劫期】！"); c.GetStat().SetRealm(REALM_TRIBULATION);
    } else if (current_realm == REALM_TRIBULATION) {
      if (c.GetStat().GetPhy() < 340 || c.GetStat().GetStr() < 340) {
        c.SetDead("肉身無法承受紫霄神雷，灰飛煙滅。【灰飛煙滅】"); pl.SaveEnd(END_TRIBULATION_FAIL);
      } else {
        PrintLine(" -> 成功扛下雷劫，仙體圓滿，破境步入【大乘期】！"); c.GetStat().SetRealm(REALM_MAHAYANA);
      }
    } else if (current_realm == REALM_MAHAYANA) {
      if (std::rand() % 100 < 10) {
        std::cout << "\n[" << c.GetAge() << "歲]: 凡界虛空大開，是否白日飛升步入仙界？\n1.飛升 \n2.留在人界" << std::endl;
        int up; std::cin >> up; if (up == 1) c.GetStat().SetRealm(REALM_EARTH_IMMORTAL);
      }
    } else if (current_realm >= REALM_EARTH_IMMORTAL) {
      PrintLine("[" + std::to_string(c.GetAge()) + "歲]: [仙界爭霸] 參與天帝道果大戰。");
      // 🔥 Bug 修正：如果 CheckDeathRisk() 回傳 true（代表龍傲天暴斃），應直接 break 跳出循環而非 continue。
      if (CheckDeathRisk()) break; 
      c.SetDead("仙王大戰餘波震碎了你的仙格，隕落於仙界。【仙人隕落】"); pl.SaveEnd(END_IMMORTAL_FALL);
    }
  }

  std::cout << "\n【輪迴結束】最終境界: " << GetRealmName(c.GetStat().GetRealm()) << " | 結局: " << c.GetEndingMsg() << std::endl;

  // 寫入紀錄
  ReinRecord record;
  record.id = pl.GetReinCount(); record.ch_name = c.GetName();
  record.final_realm = GetRealmName(c.GetStat().GetRealm()); record.ending = c.GetEndingMsg();
  record.total_stats = c.GetStat().GetStr() + c.GetStat().GetWis() + c.GetStat().GetPhy() + c.GetStat().GetLuck();
  record.detailstat = c.GetStat();
  for (auto &t : c.GetGifts()) record.trait_names.push_back(t.GetName());
  pl.AddHistoryRecord(record);

  std::cout << "\n[天道因果動盪] 是否回溯前世紀錄？(1:確認 / 0:跳過): ";
  int check; std::cin >> check; if (check == 1) pl.RollbackHistory(3);
  pl.SetHeritage(c.GetGifts());
}

void GameEngine::EndGame() { pl.Sum(); }