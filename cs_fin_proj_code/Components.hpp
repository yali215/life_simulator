#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Global.hpp"
#include <string>
#include <vector>
#include <set>

class Status {
private:
  int str, wis, phy, luck, fortune, fame, realm;
public:
  void SetNewInitialRein(int s, int i, int p, int l, int f, int fa, int r = 0);
  void AddStatus(int s, int i, int p, int l, int f, int fa, int r = 0);
  int GetStr() const { return str; }
  int GetWis() const { return wis; }
  int GetPhy() const { return phy; }
  int GetLuck() const { return luck; }
  int GetFortune() const { return fortune; }
  int GetFame() const { return fame; }
  int GetRealm() const { return realm; }
  void SetRealm(int r) { realm = r; }
};

class Trait {
private:
  TraitID id; Rank rarity; std::string name;
  int s_b, i_b, p_b, l_b, f_b, fa_b;
public:
  void SetTraitID(TraitID t_id);
  TraitID GetID() const { return id; }
  std::string GetName() const { return name; }
  int GetS() const { return s_b; }
  int GetI() const { return i_b; }
  int GetP() const { return p_b; }
  int GetL() const { return l_b; }
  int GetF() const { return f_b; }
  int GetFa() const { return fa_b; }
};

class Character {
private:
  std::string name; Status stat; std::vector<Trait> gifts;
  int age; bool is_alive; std::string ending_msg;

  // ==================== 🔥 本次擴充：核心宿命變數 ====================
  Gender gender;                        // 隨機抽取的主角性別
  SpiritualRoot spiritual_root;          // 靈根種類（受天賦詞條覆蓋）
  Specialization specialization;        // 六大修煉專精方向
  SectRank sect_rank;                   // 各陣營內部的動態階級職位
  bool is_righteous_cultivator;         // 正道修士 Tag（入魔或加入江湖後會動態轉變）

  // 桃花線蝴蝶效應與 BG/BL/GL 動態文本標記
  Gender lover_gender;                  // 道侶性別
  Camp lover_camp;                      // 道侶陣營
  // =================================================================

public:
  bool has_printed_mahayana;
  Camp camp;
  RomanceState romance;

  void Initial(); // 🔥 實作時，我們會在這個函式內加入「性別隨機抽取」與「靈根置換」邏輯
  void SetName(std::string n) { name = n; }
  std::string GetName() const { return name; }
  void AddStats(int s, int i, int p, int l, int f, int fa, int r = 0) { stat.AddStatus(s, i, p, l, f, fa, r); }
  void PushGift(Trait t) { if (gifts.size() < 6) gifts.push_back(t); }
  void SetDead(std::string msg) { is_alive = false; ending_msg = msg; }
  bool IsAlive() const { return is_alive; }
  int GetAge() const { return age; }
  void SetAge(int a) { age = a; }
  void GrowAge(int y) { age += y; }
  Status& GetStat() { return stat; }
  std::string GetEndingMsg() const { return ending_msg; }
  std::vector<Trait> GetGifts() const { return gifts; }
  bool HasTrait(TraitID tid) const;

  // ==================== 🔥 本次擴充：核心 Getters & Setters ====================
  Gender GetGender() const { return gender; }
  void SetGender(Gender g) { gender = g; }

  SpiritualRoot GetSpiritualRoot() const { return spiritual_root; }
  void SetSpiritualRoot(SpiritualRoot r) { spiritual_root = r; }

  Specialization GetSpecialization() const { return specialization; }
  void SetSpecialization(Specialization s) { specialization = s; }

  SectRank GetSectRank() const { return sect_rank; }
  void SetSectRank(SectRank r) { sect_rank = r; }

  bool IsRighteousCultivator() const { return is_righteous_cultivator; }
  void SetRighteousCultivator(bool b) { is_righteous_cultivator = b; }

  Gender GetLoverGender() const { return lover_gender; }
  void SetLoverGender(Gender g) { lover_gender = g; }

  Camp GetLoverCamp() const { return lover_camp; }
  void SetLoverCamp(Camp c) { lover_camp = c; }
  // =============================================================================
};

struct ReinRecord {
  int id; std::string ch_name; std::string final_realm; std::string ending;
  std::vector<std::string> trait_names; int total_stats; Status detailstat;
};

class Player {
private:
  std::string player_name; int reincnt;
  std::set<EndingID> unlocked_ends; std::set<TraitID> used_traits;
  std::vector<Trait> heritage_traits; std::vector<ReinRecord> history_records;
public:
  void Login(std::string name);
  void AddRein() { reincnt++; }
  int GetReinCount() const { return reincnt; }
  void SaveEnd(EndingID e) { unlocked_ends.insert(e); }
  void SaveTrait(TraitID t) { used_traits.insert(t); }
  std::vector<Trait> GetHeritage() const { return heritage_traits; }
  void SetHeritage(std::vector<Trait> t_list) { heritage_traits = t_list; }
  void ClearHeritage() { heritage_traits.clear(); }
  void AddHistoryRecord(const ReinRecord& rec) { history_records.push_back(rec); }
  void RollbackHistory(int allowed_count);
  void Sum();
};

#endif