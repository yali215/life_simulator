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
