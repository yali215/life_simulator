#include "Components.hpp"
#include <iostream>
#include <cstdlib>

void Status::SetNewInitialRein(int s, int i, int p, int l, int f, int fa, int r) {
  str = s; wis = i; phy = p; luck = l; fortune = f; fame = fa; realm = r;
}
void Status::AddStatus(int s, int i, int p, int l, int f, int fa, int r) {
  str += s; wis += i; phy += p; luck += l; fortune += f; fame += fa; realm += r;
  if (str < 0) str = 0; if (wis < 0) wis = 0; if (phy < 0) phy = 0;
}

void Trait::SetTraitID(TraitID t_id) {
  id = t_id; s_b = 0; i_b = 0; p_b = 0; l_b = 0; f_b = 0; fa_b = 0;
  switch (id) {
    case GOD_STRENGTH: name = "天生神力"; rarity = RARE; s_b = 10; break;
    case PEACH_BLOSSOM: name = "桃花眷顧"; rarity = RARE; fa_b = 5; l_b = 5; break;
    case HERB_AFFINITY: name = "草木靈親"; rarity = RARE; i_b = 8; f_b = 3; break;
    case WEAPON_FURNACE: name = "神兵熔爐"; rarity = RARE; s_b = 6; p_b = 4; break;
    case GOLDEN_BODY: name = "不滅金身"; rarity = RARE; p_b = 12; break;
    case ASURA_KILL: name = "修羅殺道"; rarity = RARE; s_b = 15; fa_b = -5; break;
    case RICH_MAN: name = "富甲一方"; rarity = RARE; f_b = 15; fa_b = 2; p_b = -4; break;
    case ROYAL_BLOOD: name = "仙門帝胄"; rarity = RARE; fa_b = 8; f_b = 8; l_b = -2; break;
    case THREE_LIVES_PEACH: name = "三生三世桃花緣"; rarity = RARE; l_b = 4; fa_b = 8; break;
    case WHITE_MOONLIGHT: name = "白月光"; rarity = RARE; fa_b = 10; l_b = 2; break;
    case LATE_BLOOMER: name = "大器晚成"; rarity = RARE; s_b = -2; i_b = -2; p_b = -2; break;
    case POOR_GUY: name = "天選打工人"; rarity = COMMON; p_b = 3; s_b = 2; f_b = -4; break;
    case WINE_DISCIPLE: name = "酒仙弟子"; rarity = COMMON; p_b = 4; f_b = -3; break;
    case TOMB_RAIDER: name = "古墓傳人"; rarity = COMMON; f_b = 5; l_b = -2; break;
    case WEAK_BODY: name = "弱不禁風"; rarity = COMMON; p_b = -5; i_b = 3; break;
    case CLUMSY_HAND: name = "手腳笨拙"; rarity = COMMON; s_b = -3; l_b = -2; p_b = 5; break;
    case SCHOLAR: name = "白面書生"; rarity = COMMON; i_b = 4; s_b = -3; break;
    case PURE_HEART: name = "赤子之心"; rarity = COMMON; i_b = 3; l_b = 2; break;
    case ESCAPE_MASTER: name = "跑路大師"; rarity = COMMON; l_b = 3; s_b = -2; break;
    case BAD_LOOK: name = "面目可憎"; rarity = COMMON; fa_b = -5; s_b = 3; break;
    case RIGHTEOUS: name = "一身正氣"; rarity = COMMON; fa_b = 4; f_b = -2; break;
    case ANTI_POISON: name = "百毒不侵"; rarity = COMMON; p_b = 4; break;
    case PERFECT_MEMORY: name = "過目不忘"; rarity = COMMON; i_b = 5; break;
    case BIG_MOUTH: name = "大嘴巴"; rarity = COMMON; fa_b = -3; break;
    case CROW_MOUTH: name = "烏鴉嘴"; rarity = COMMON; l_b = -4; break;
    case IRON_COCK: name = "鐵公雞"; rarity = COMMON; f_b = 4; fa_b = -3; break;
    case LUCKY_SON: name = "氣運之子"; rarity = LEGENDARY; l_b = 8; f_b = 3; break;
    case HEAVY_PUPIL_RULER: name = "重瞳明君"; rarity = LEGENDARY; i_b = 8; l_b = 5; fa_b = 8; break;
    case SUPREME_BONE: name = "至尊骨生"; rarity = LEGENDARY; p_b = 8; s_b = 5; f_b = -3; break;
    case FIVE_ELEMENT_WASTE: name = "五行雜靈根"; rarity = EPIC; s_b = -2; i_b = -2; p_b = -2; l_b = -2; break;
    case ASCENDED_IMMORTAL: name = "仙界謫仙"; rarity = EPIC; i_b = 12; s_b = 3; l_b = -2; break;
    case SWORD_MASTER: name = "劍仙轉世"; rarity = EPIC; s_b = 8; i_b = 5; f_b = -2; break;
    case ANCIENT_ANCESTOR: name = "出山老祖"; rarity = EPIC; i_b = 15; fa_b = 15; p_b = -3; break;
    case GINSENG_SPIRIT: name = "萬年人參成精"; rarity = EPIC; p_b = 15; l_b = 3; break;
    case LONG_AOTIAN: name = "龍傲天"; rarity = EPIC; s_b = 12; l_b = 12; fa_b = 8; break;
    default: name = "凡夫俗子"; rarity = COMMON; break;
  }
}

void Character::Initial() {
  is_alive = true; age = 0; gifts.clear();
  stat.SetNewInitialRein(0, 0, 0, 0, 0, 0, 0);
  has_printed_mahayana = false; 
  camp = CAMP_NONE; 
  romance = ROMANCE_NONE;

  // ==================== 🔥 本次升級：投胎與靈根初始化 ====================
  // 1. 隨機抽選投胎性別（50% 男，50% 女）
  gender = (std::rand() % 2 == 0) ? GENDER_MALE : GENDER_FEMALE;

  // 2. 隨機抽取初始靈根（權重隨機：10%天靈根、25%雙靈根、40%三靈根、25%雜靈根）
  int root_roll = std::rand() % 100;
  if (root_roll < 10)       spiritual_root = ROOT_HEAVEN;
  else if (root_roll < 35)  spiritual_root = ROOT_DOUBLE;
  else if (root_roll < 75)  spiritual_root = ROOT_TRIPLE;
  else                      spiritual_root = ROOT_WASTE;

  // 3. 初始身分、專精與 Tag 清空與重置
  specialization = SPEC_NONE;
  sect_rank = RANK_NONE;
  is_righteous_cultivator = false; // 凡人尚未具有正道修士 Tag，進宗門才綁定

  // 4. 清空道侶動態標記
  lover_gender = GENDER_MALE;
  lover_camp = CAMP_NONE;
  // =====================================================================
}

// 為了在選完詞條後、正式投胎前能夠依據選定天賦置換靈根，我們需要一個觸發函式
// 這裡可以直接在 RunRein 選完词條後執行檢查，為了方便後面判斷，HasTrait 依然保持原樣：
bool Character::HasTrait(TraitID tid) const {
  for (auto &g : gifts) { if (g.GetID() == tid) return true; }
  return false;
}

void Player::Login(std::string name) {
  player_name = name; reincnt = 0; unlocked_ends.clear(); used_traits.clear(); heritage_traits.clear(); history_records.clear();
}
void Player::RollbackHistory(int allowed_count) {
  if (history_records.empty()) return;
  std::cout << "\n🔮=======【天道輪迴鏡：前世回溯】=======🔮" << std::endl;
  int displayed = 0;
  for (auto ri = history_records.rbegin(); ri != history_records.rend(); ++ri) {
    if (displayed >= allowed_count) break;
    std::cout << "【前 " << displayed + 1 << " 世 / 第 " << ri->id << " 世】 " << ri->ch_name 
              << " | 境界: " << ri->final_realm << " | 結局: " << ri->ending << std::endl;
    displayed++;
  }
}
void Player::Sum() {
  std::cout << "\n=== 玩家天道石碑 ===" << std::endl;
  std::cout << "玩家: " << player_name << " | 總輪迴: " << reincnt << " 次" << std::endl;
  std::cout << "解鎖結局率: " << ((double)unlocked_ends.size() / allend) * 100 << "%" << std::endl;
}