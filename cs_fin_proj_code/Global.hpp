#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>

const int allend = 16;

enum TraitID {
  GOD_STRENGTH, LUCKY_SON, POOR_GUY, PEACH_BLOSSOM, HEAVY_PUPIL_RULER,
  SUPREME_BONE, FIVE_ELEMENT_WASTE, ASCENDED_IMMORTAL, SWORD_MASTER, ANCIENT_ANCESTOR,
  GINSENG_SPIRIT, LONG_AOTIAN, HERB_AFFINITY, WEAPON_FURNACE, GOLDEN_BODY,
  ASURA_KILL, RICH_MAN, ROYAL_BLOOD, THREE_LIVES_PEACH, WHITE_MOONLIGHT,
  LATE_BLOOMER, WINE_DISCIPLE, TOMB_RAIDER, SCHOLAR, PURE_HEART,
  ESCAPE_MASTER, BAD_LOOK, RIGHTEOUS, ANTI_POISON, PERFECT_MEMORY,
  BIG_MOUTH, CROW_MOUTH, IRON_COCK, WEAK_BODY, CLUMSY_HAND
};

enum Rank { COMMON, RARE, EPIC, LEGENDARY };

enum EndingID {
  END_LOCKJAW, END_CHOKED, END_ORDINARY_LIFE, END_DEPRESSED, END_RANSOM_FAIL,
  END_CRIPPLED, END_BODY_EXPLODE, END_MARTYR, END_ROMANCE_CROSS, END_HAREM_TRAGEDY,
  END_TRIBULATION_FAIL, END_AOTIAN_FALL, END_IMMORTAL_FALL, END_ANCIENT_REGRET,
  END_SWORD_GOD, END_CHAOS_MASTER
};

enum RealmID {
  REALM_MORTAL = 0, REALM_QI_REFINING = 1, REALM_FOUNDATION = 2, REALM_GOLDEN_CORE = 3,
  REALM_NASCENT_SOUL = 4, REALM_SOUL_FORMATION = 5, REALM_INTEGRATION = 6, REALM_TRIBULATION = 7,
  REALM_MAHAYANA = 8, REALM_EARTH_IMMORTAL = 9, REALM_HEAVEN_IMMORTAL = 10, REALM_GOLDEN_IMMORTAL = 11,
  REALM_CHAOS_IMMORTAL = 12
};

enum Camp { CAMP_NONE, CAMP_SECT, CAMP_JIANGHU, CAMP_DEMON };
enum RomanceState { ROMANCE_NONE, ROMANCE_ENGAGED, ROMANCE_REJECTED, ROMANCE_TAO_LU };

// ==================== 🔥 以下為本次擴充的全新機制定義 ====================

// 1. 性別定義（投胎隨機抽選）
enum Gender {
  GENDER_MALE,    // 男胎
  GENDER_FEMALE   // 女胎
};

// 2. 六大修煉專精方向（內門/一流高手選拔後開啟）
enum Specialization {
  SPEC_NONE,      // 尚未開悟專精 / 通識階段
  SPEC_SWORD,     // 劍道專精（劍修）
  SPEC_PILL,      // 丹道專精（丹修）
  SPEC_LAW,       // 術法專精（法修）
  SPEC_BODY,      // 煉體專精（體修）
  SPEC_TALISMAN,  // 符籙專精（符修） 🔥 新增
  SPEC_ARTIFACT   // 煉器專精（器修） 🔥 新增
};

// 3. 靈根系統（入門分流門檻）
enum SpiritualRoot {
  ROOT_HEAVEN,    // 天靈根 / 單靈根（直接進內門）
  ROOT_DOUBLE,    // 雙靈根（直接進內門）
  ROOT_TRIPLE,    // 三靈根（發配外門磨練）
  ROOT_WASTE      // 四五雜靈根（發配外門磨練）
};

// 4. 解耦後的各陣營職位階級（隨績效、年齡與特殊事件動態變更）
enum SectRank {
  RANK_NONE,              // 凡人 / 未具備派系身分
  
  // 正道宗門（青雲宗）晉升線
  RANK_SECT_OUTER,        // 外門弟子
  RANK_SECT_INNER,        // 內門弟子
  RANK_SECT_CORE,         // 親傳弟子 / 核心傳人
  RANK_SECT_ELDERS,       // 宗門長老
  RANK_SECT_MASTER,       // 宗門掌門

  // 江湖散修晉升線
  RANK_JH_NONE,           // 江湖散修 / 無名小卒
  RANK_JH_MASTER,         // 江湖一流高手
  RANK_JH_LEADER,         // 武林盟主 🔥 修正後名稱

  // 魔道派系晉升線
  RANK_DM_NONE,           // 魔道散修 / 未入流魔頭
  RANK_DM_SERVANT,        // 魔門雜役
  RANK_DM_FOLLOWER,       // 嗜血魔徒
  RANK_DM_RIGHTHAND,      // 魔主左右手
  RANK_DM_LORD            // 篡位魔主 🔥 修正後名稱
};

// ======================================================================

// 輔助函式
void PrintLine(std::string text, int delay_ms = 1100);
std::string GetRealmName(int r);
int GetMaxAge(int realm);

#endif