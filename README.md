# 🌌 文字版修仙生死輪迴模擬器 (Immortal Reincarnation Simulator)

[![Language](https://img.shields.io/badge/Language-C%2B%2B11%20or%20above-blue.svg)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Architecture](https://img.shields.io/badge/Architecture-OOD%20%7C%20Decoupled-orange.svg)]()

本專案是一款基於 C++ 控制台（Console）開發的高隨機性、輕量化文字互動養成遊戲。玩家將扮演一名在天道撥弄下不斷轉世的修仙者，利用初始的「詞條抉擇」與過程中的「事件分支」來影響角色資質，在極簡的介面中體驗數百年的修仙興衰與命運流轉。

---

## 一、 遊戲使用說明

### 1. 環境配置與編譯
*   **開發語言**：C++11 或以上標準。
*   **跨平台支援**：程式碼內建 Windows 平台的 UTF-8 編碼轉換機制（`SetConsoleOutputCP(CP_UTF8)`），可於 Windows CMD、Linux 或 macOS 終端機直接編譯執行，絕不亂碼。
*   **編譯指令範例** (以 `g++` 為例)：
```bash
    g++ -std=c++11 main.cpp -o ImmortalSimulator
    ```

### 2. 核心遊玩流程
1.  **天道留名**：首次啟動遊戲時，須輸入玩家的真實姓名，這將永久記錄在「天道石碑」中（具備空字串防呆）。
2.  **轉世命名**：每一世輪迴開始前，玩家可為該世肉身輸入一個專屬的「轉世道號」。
3.  **因果繼承**：若上一世結束時有留存的天賦，系統會列出並允許玩家輸入對應編號選擇繼承（輸入 `0` 結束）。
4.  **詞條抉擇**：系統每輪隨機顯化 15 個天地詞條，玩家可輸入 `1~15` 的編號加入專屬格位（上限 6 個）。格子未滿時可輸入 `s` 或 `S` 提早開啟本世。
5.  **背景選擇**：自選出生背景（平凡人家、富甲商賈、修仙門閥），藉此獲得不同的初始屬性加成與起跑境界。
6.  **動態推進與互動抉擇**：遊戲文本以流暢的速度自動跑字（每行延遲 500ms）。當觸發重大事件（如：退婚流風波、黑市古鼎）時，系統會暫停並要求玩家輸入數字進行抉擇。
7.  **結算與退出**：角色身殞或登臨至高後，可在輪迴間隙輸入 `0` 進入下一世，或輸入 `7` 統計當前總數據（解鎖率）並優雅退出遊戲。

---

## 二、 模組設計文件 (Architecture & Module Design)

本遊戲採用**物件導向設計 (Object-Oriented Design, OOD)**，將「遊戲資料狀態」、「角色實體」與「核心控制邏輯」進行高度解耦。邏輯底層不與終端機輸出高度綁定，以便未來能無縫編譯為 WebAssembly (Wasm) 並移植至 Web 網頁平台。

### 1. 類別架構關係圖

[ GameEngine ] (天道核心引擎)
      │
      ├─► [ Player ] ──────► 管理永久跨世數據 (天道石碑、已解鎖結局 set)
      │
      └─► [ Character ] ───► 管理當世個體狀態 (年齡、生死、當前劇情線)
               │
               ├─► [ Status ] (核心六大資質與境界封裝)
               │
               └─► std::vector<Trait> (當世已裝備的天賦詞條動態陣列)

---

### 2. 核心類別詳細規格

#### ① `Class: Status` (資質狀態類別)
*   **職責**：高度封裝角色的核心修仙資質與境界資料，維護屬性邊界安全。
*   **私有屬性 (Private)**：
    *   `int str` (氣力)、`int wis` (悟性)、`int phy` (體魄)
    *   `int luck` (氣運)、`int fortune` (家產)、`int fame` (名望)
    *   `int realm` (當前境界編號，對應 `RealmID`)
*   **核心方法 (Public)**：
    *   `SetNewInitialRein()`：初始化或重設本世基本數值。
    *   `AddStatus(int s, int w, int p, int l, int f, int fa)`：傳入屬性變更值，內含防呆機制（數值最低截斷為 0，避免數值溢位或出現負數）。
    *   提供完整的滿足物件封裝之 Getter / Setter 方法。

#### ② `Class: Trait` (天賦詞條類別)
*   **職責**：定義單一詞條的稀有度、名稱與其對核心資質產生的即時加減成（靜態資料驅動）。
*   **私有屬性 (Private)**：
    *   `TraitID id` (詞條唯一識別碼)
    *   `Rank rarity` (稀有度：普通/罕見/史詩/傳說)
    *   `std::string name` (詞條文字名稱)
    *   `int s_b, i_b, p_b, l_b, f_b, fa_b` (對應六大屬性的屬性增益 Buffer 值)
*   **核心方法 (Public)**：
    *   `SetTraitID(TraitID t_id)`：利用內部 `switch-case` 核心對應表，根據傳入的 ID 自動初始化該詞條的所有文本與加成數值（例如 `LONG_AOTIAN` 給予氣力+20, 氣運+20, 名望+15）。

#### ③ `Class: Character` (轉世角色實體類別)
*   **職責**：代表當前輪迴中活生生的修仙個體，維護生命週期、年齡步長與當前觸發的劇情分歧。
*   **私有屬性 (Private)**：
    *   `std::string name` (修仙道號)
    *   `Status stat` (資質狀態實體物件)
    *   `std::vector<Trait> gifts` (當世已裝備天賦詞條容器，上限 6 個)
    *   `int age` (當前年齡)
    *   `bool is_alive` (生死狀態旗標)
    *   `std::string ending_msg` (死因或結局最終文本)
*   **核心方法 (Public)**：
    *   `Initial()`：每世開局重置個體狀態、清空上一世詞條陣列、重置劇情分支旗標。
    *   `HasTrait(TraitID tid)`：快速檢索當前角色是否擁有某個特定天賦（例如是否擁有龍傲天光環以觸發特殊對話）。
    *   `GrowAge(int y)`、`SetDead(std::string msg)`：個體狀態變更方法。
    *   `RouteBranch current_route`：動態記錄當前處於哪一條特定的重大劇情線（如 `ROUTE_YIN_REN` 隱忍線、`ROUTE_DA_NAO` 大鬧線），用以控制跨歲數的連鎖事件。

#### ④ `Class: Player` (天道玩家永久類別)
*   **職責**：記錄跨越無數次生死輪迴、屬於「玩家本體」的永久存檔數據、天道石碑成就與因果傳遞。
*   **私有屬性 (Private)**：
    *   `std::string player_name` (玩家真實姓名)
    *   `int reincnt` (總輪迴次數計數器)
    *   `std::set<EndingID> unlocked_ends` (使用 `std::set` 自動去重，記錄已解鎖的結局集合)
    *   `std::set<TraitID> used_traits` (記錄生涯體驗過的詞條集合)
    *   `std::vector<Trait> heritage_traits` (宿命留存陣列：儲存上一世結束時的詞條，供下一世繼承)
*   **核心方法 (Public)**：
    *   `Login()`：永久玩家登入初始化與防呆攔截。
    *   `SaveEnd(EndingID e)`：解鎖並儲存新結局至 `set` 中。
    *   `SetHeritage()` / `ClearHeritage()`：控制前世因果詞條的傳遞與安全清空。
    *   `Sum()`：天道石碑結算，動態計算與輸出結局總解鎖率百分比。

#### ⑤ `Class: GameEngine` (天道核心遊戲引擎類別)
*   **職責**：整個遊戲生命週期的總控制中心，負責隨機事件分發、動態時間軸前進、境界晉升與天劫檢定、以及輸入緩衝區的防呆攔截。
*   **核心方法 (Public)**：
    *   `InitGame()`：引導玩家進行天道留名登入。
    *   `RunRein()`：核心主循環。內部包含：
        *   前世因果繼承選擇邏輯（含重複性檢查防呆）。
        *   15 選 6 天地詞條顯化與隨機洗牌演算法（`std::random_shuffle`）。
        *   出生背景選擇與特殊詞條（如出山老祖）的覆蓋邏輯。
        *   `while(c.IsAlive())` 歲月流逝循環：動態計算年齡步長、分發隨機事件、進行境界自動晉升（大限限制檢查）與高境界雷劫生死檢定。
        *   角色身殞後，將當世詞條寫回 `Player` 的 Heritage 槽實現繼承。
    *   `CheckDeathRisk()`：特殊命運（如「剛過易折」危害詞條）的死劫機率觸發、反殺與判定邏輯。

---

### 3. 關鍵枚舉 (Enum) 與資料結構設計

全案採用**強型別枚舉 (Strongly-typed Enums)** 以確保類型安全 (Type Safety)，嚴防數值混淆：

*   **`TraitID`**：定義 35 種以上天賦詞條的唯一代號（如 `LONG_AOTIAN`, `RE_TONG`, `TIAN_SHENG_SHEN_LI`），便於未來無限擴充天賦庫。
*   **`Rank`**：定義詞條稀有度權重：`COMMON` (普通), `RARE` (罕見), `EPIC` (史詩), `LEGENDARY` (傳說)。
*   **`EndingID`**：定義 16 種最終結局代碼（如 `END_ORDINARY_LIFE` 平凡一生、`END_SWORD_GOD` 一代劍聖、`END_ASHES` 灰飛煙滅）。
*   **`RealmID`**：定義修仙十三大境界（從 `REALM_MORTAL = 0` 凡人，一路至 `REALM_CHAOS_IMMORTAL = 12` 混元無極仙）。各境界綁定不同的壽元大限（壽長）與事件觸發權重。
*   **`RouteBranch`**：劇情路由狀態（`ROUTE_NONE`, `ROUTE_YIN_REN`, `ROUTE_DA_NAO`），配合引擎進行跨年齡、跨歲月的「連鎖事件連環觸發」判定。

---

## 三、 健全性與健壯性設計 (Robustness)

1.  **記憶體安全 (No Memory Leak)**：所有動態資料（如歷史紀錄、因果暫存槽）皆採用標準 STL 容器管理，在玩家進行「無限次重開輪迴」的極端測試下，過期物件皆會被正確析構，記憶體佔用穩定控制在 10MB 以內。
2.  **控制台輸入防呆 (I/O Sieve)**：當面臨劇情選項、詞條編號輸入時，若使用者惡意或誤輸入英文字母、特殊符號或超出範圍的數值，系統會調用 `std::cin.clear()` 與 `std::cin.ignore()` 徹底清除輸入緩衝區狀態，優雅攔截異常並重置輸入點，絕不陷入死迴圈或導致程式崩潰。
```</Trait>
