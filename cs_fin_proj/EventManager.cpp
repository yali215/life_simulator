#include "EventManager.hpp"
#include <iostream>
#include <cstdlib>

void EventManager::TriggerAgeEvent(Character& c, Player& pl, int birth_type) {
  int age = c.GetAge();

  // -------------------------------------------------------------------------
  // 0. 出生設定：高稀有度詞條強制置換初始靈根 (僅在0歲觸發一次)
  // -------------------------------------------------------------------------
  if (age == 0) {
    if (c.HasTrait(FIVE_ELEMENT_WASTE)) {
      c.SetSpiritualRoot(ROOT_WASTE);
    } else if (c.HasTrait(LUCKY_SON) || c.HasTrait(SUPREME_BONE) || c.HasTrait(ASCENDED_IMMORTAL)) {
      c.SetSpiritualRoot(ROOT_HEAVEN);
    }
    
    // 輸出出生儀式感提示
    std::string gender_str = (c.GetGender() == GENDER_MALE) ? "男嬰" : "女嬰";
    std::string root_str = "普通三靈根";
    if (c.GetSpiritualRoot() == ROOT_HEAVEN) root_str = "天靈根（單靈根）";
    else if (c.GetSpiritualRoot() == ROOT_DOUBLE) root_str = "地靈根（雙靈根）";
    else if (c.GetSpiritualRoot() == ROOT_WASTE) root_str = "五行雜靈根";
    
    PrintLine("[0歲]: 你降生在一個小村莊，天道流轉，你是一名【" + gender_str + "】，身懷【" + root_str + "】之資。");
    return;
  }

  // -------------------------------------------------------------------------
  // 高稀有度詞條最高優先權覆蓋判定（Override Priority）
  // -------------------------------------------------------------------------
  if (c.HasTrait(GINSENG_SPIRIT) && (std::rand() % 100 < 25) && age > 5) {
    PrintLine("[" + std::to_string(age) + "歲]: 【天賦專屬：草木遭劫】由於你是【萬年人參成精】，身上隱隱散發的藥香引來高階魔修覬覦！");
    if (c.GetStat().GetLuck() >= 15 || c.HasTrait(ESCAPE_MASTER)) {
      PrintLine(" -> 你施展本體遁地神通，狼狽逃過一劫！ 體魄+3，名望+2");
      c.AddStats(0, 0, 3, 0, 0, 2);
    } else {
      PrintLine(" -> 你被魔頭抓住強行割去三兩參肉煉丹！你痛苦萬分，元氣大傷！ 體魄-10");
      c.AddStats(0, 0, -10, 0, 0, 0);
      if (c.GetStat().GetPhy() <= 0) {
        c.SetDead("因為參體被魔頭啃食殆盡，不幸夭折，化作一縷怨魂。");
      }
    }
    return;
  }

  // -------------------------------------------------------------------------
  // 1. 婚約流事件鏈 (10~17歲)
  // -------------------------------------------------------------------------
  if (age >= 10 && age <= 17 && c.romance == ROMANCE_NONE) {
    if (c.GetStat().GetFortune() <= 12 && (std::rand() % 100 < 40)) {
      std::string ex_fiance = (c.GetGender() == GENDER_MALE) ? "傲慢的未婚妻" : "勢利的未婚夫";
      std::cout << "\n[" << age << "歲]: 【宿命風波：退婚流】落魄之際，" << ex_fiance << "前來強行撕毀婚約！\n1. 隱忍簽字（留得青山在）\n2. 憤怒撕書（莫欺少年窮！）" << std::endl;
      int act; std::cin >> act;
      if (act == 1) {
        PrintLine(" -> 你低頭簽字，緊握雙拳，心中種下忍辱道心。悟性+5，名望-3");
        c.AddStats(0, 5, 0, 0, 0, -3);
        c.romance = ROMANCE_REJECTED;
      } else {
        PrintLine(" -> 你當場大喝『莫欺少年窮』！將婚書扯得粉碎！打退前來叫囂的僕從！氣力+5，名望+5，但與權貴結怨。");
        c.AddStats(5, 0, 0, 0, -5, 5);
        c.romance = ROMANCE_ENGAGED; // 代表大鬧反殺狀態
      }
      return;
    }
  }

  // 退婚流後續：金丹期（或30歲以上）的高強度連鎖
  if (age >= 30 && (c.romance == ROMANCE_REJECTED || c.romance == ROMANCE_ENGAGED) && (std::rand() % 100 < 30)) {
    std::string target_name = (c.GetGender() == GENDER_MALE) ? "前未婚妻" : "前未婚夫";
    if (c.romance == ROMANCE_REJECTED) {
      PrintLine("\n[" + std::to_string(age) + "歲]: 【退婚連鎖：三十年河東】你在遠古秘境中重逢了當年退婚的" + target_name + "。此時你強大無比，而對方宗門落魄。");
      std::cout << "你選擇：\n1. 無情抹殺（斬草除根）\n2. 反向羞辱（道心大悅）" << std::endl;
      int choice; std::cin >> choice;
      if (choice == 1) {
        PrintLine(" -> 你冷酷出手，將其抹殺在秘境之中！斬斷因果，氣力+5，名望+5");
        c.AddStats(5, 0, 0, 0, 0, 5);
      } else {
        PrintLine(" -> 你當著眾人的面將其隨手震退，對方流下悔恨的淚水。你道心大悅，修為暴漲！ 悟性+10，氣運+3");
        c.AddStats(0, 10, 0, 3, 0, 0);
      }
      c.romance = ROMANCE_NONE; // 因果已了
      return;
    } else if (c.romance == ROMANCE_ENGAGED) {
      PrintLine("\n[" + std::to_string(age) + "歲]: 【退婚連鎖：宿命反殺】當年你大鬧婚宴，" + target_name + "背後的權貴勢力派強者前來截殺你！");
      if (c.GetStat().GetStr() >= 40) {
        PrintLine(" -> 你長劍橫掃，反殺強敵！名震天下，並在戰後巧遇前來暗中相助的命定之人。正式開啟桃花線！ 名望+10");
        c.AddStats(0, 0, 0, 0, 0, 10);
        c.romance = ROMANCE_TAO_LU; // 進入正經桃花線
      } else {
        PrintLine(" -> 仇家攻勢強橫，你重傷斷去一臂，道基受損！ 體魄-15，名望-5");
        c.AddStats(0, 0, -15, 0, 0, -5);
      }
      return;
    }
  }

  // -------------------------------------------------------------------------
  // 2. 陣營門戶分水嶺與靈根分流機制
  // -------------------------------------------------------------------------
  if (c.camp == CAMP_NONE) {
    bool can_choose = false;
    if (birth_type == 1 && age >= 15) can_choose = true;
    if (birth_type == 2 && age >= 13) can_choose = true;
    if (birth_type == 3 && age >= 9) can_choose = true;

    if (can_choose) {
      std::cout << "\n[" << age << "歲]: 【人生抉擇：仙途門戶】你已到了立身之年，決定未來的修仙方向：\n1. 拜入正道宗門（青雲宗）\n2. 隻身闖蕩江湖（生死散修）" << std::endl;
      int choice; std::cin >> choice;
      if (choice == 1) {
        c.camp = CAMP_SECT;
        c.SetRighteousCultivator(true); // 綁定正道修士 Tag
        
        // 依據靈根種類分發初始身分
        if (c.GetSpiritualRoot() == ROOT_HEAVEN || c.GetSpiritualRoot() == ROOT_DOUBLE) {
          PrintLine(" -> 宗門大長老驚嘆於你的優異靈根，免去考核，讓你免試直升【內門弟子】！ 悟性+5，名望+5");
          c.SetSectRank(RANK_SECT_INNER);
          c.AddStats(0, 5, 0, 0, 0, 5);
        } else {
          PrintLine(" -> 由於你天資平平，只能通過常規考核，成為青雲宗【外門弟子】。 體魄+3");
          c.SetSectRank(RANK_SECT_OUTER);
          c.AddStats(0, 0, 3, 0, 0, 0);
        }
      } else {
        PrintLine(" -> 你背上長劍，一頭扎進波譎雲詭的修仙江湖，成為散修【無名小卒】。");
        c.camp = CAMP_JIANGHU;
        c.SetSectRank(RANK_JH_NONE);
        c.AddStats(2, 0, 3, 1, -2, 0);
      }
      return;
    }
  }

  // -------------------------------------------------------------------------
  // 3. 內門大選、師尊擇徒與六大專精系統
  // -------------------------------------------------------------------------
  if (c.camp == CAMP_SECT && c.GetSectRank() == RANK_SECT_OUTER && age >= 18 && (std::rand() % 100 < 40)) {
    std::cout << "\n[" << age << "歲]: 【宗門大選】外門熬煮多年，三年一度的內門晉升大會開幕，你決定：\n1. 穩紮穩打，上台比武\n2. 消耗 5 點家產，私下打點執事" << std::endl;
    int choice; std::cin >> choice;
    bool pass = false;
    if (choice == 1 && c.GetStat().GetStr() >= 20) {
      PrintLine(" -> 你實力雄厚，強勢擊敗對手！順利晉升為【內門弟子】！");
      pass = true;
    } else if (choice == 2 && c.GetStat().GetFortune() >= 5) {
      PrintLine(" -> 靈石開路，執事暗中安排弱敵，你輕鬆晉升為【內門弟子】！");
      c.AddStats(0, 0, 0, 0, -5, 0); // 消耗家產
      pass = true;
    } else {
      PrintLine(" -> 你功敗垂成，只能繼續在外門當牛做馬。 體魄+2");
      c.AddStats(0, 0, 2, 0, 0, 0);
    }

    if (pass) {
      c.SetSectRank(RANK_SECT_INNER);
      std::cout << "請選擇你的修煉專精方向：\n1. 劍道（劍修）\n2. 丹道（丹修）\n3. 術法（法修）\n4. 體修\n5. 符修\n6. 器修" << std::endl;
      int spec_choice; std::cin >> spec_choice;
      if (spec_choice == 1) c.SetSpecialization(SPEC_SWORD);
      else if (spec_choice == 2) c.SetSpecialization(SPEC_PILL);
      else if (spec_choice == 3) c.SetSpecialization(SPEC_LAW);
      else if (spec_choice == 4) c.SetSpecialization(SPEC_BODY);
      else if (spec_choice == 5) c.SetSpecialization(SPEC_TALISMAN);
      else c.SetSpecialization(SPEC_ARTIFACT);
      PrintLine(" -> 專精確立，內門生活正式拉開序幕！");
    }
    return;
  }

  // 親傳弟子奇遇：已是內門弟子且符合專精屬性
  if (c.camp == CAMP_SECT && c.GetSectRank() == RANK_SECT_INNER && (std::rand() % 100 < 20)) {
    if ((c.GetSpecialization() == SPEC_SWORD && c.GetStat().GetStr() >= 30) || (c.GetSpecialization() == SPEC_PILL && c.GetStat().GetWis() >= 30)) {
      PrintLine("\n[" + std::to_string(age) + "歲]: 【師尊擇徒】太上長老出關，看中你的專精天資，破格收你為【親傳弟子】！ 悟性+10，名望+10");
      c.SetSectRank(RANK_SECT_CORE);
      c.AddStats(0, 10, 0, 0, 0, 10);
      return;
    }
  }

  // -------------------------------------------------------------------------
  // 4. 跨陣營戀情、動態性向(BL/GL/BG)與正魔大戰蝴蝶效應
  // -------------------------------------------------------------------------
  if (c.romance == ROMANCE_TAO_LU && (std::rand() % 100 < 25) && age >= 25) {
    // 隨機抽取道侶的性別與陣營（若主角是正道，道侶有高機率是魔道，引發衝突）
    c.SetLoverGender((std::rand() % 2 == 0) ? GENDER_MALE : GENDER_FEMALE);
    c.SetLoverCamp((c.camp == CAMP_SECT) ? CAMP_DEMON : CAMP_SECT);

    std::string lover_title = "";
    if (c.GetGender() == GENDER_MALE && c.GetLoverGender() == GENDER_FEMALE) lover_title = "魔門妖女（BG）";
    else if (c.GetGender() == GENDER_FEMALE && c.GetLoverGender() == GENDER_MALE) lover_title = "正道少俠（BG）";
    else if (c.GetGender() == GENDER_MALE && c.GetLoverGender() == GENDER_MALE) lover_title = "傲嬌魔兄（BL）";
    else lover_title = "正氣仙子（GL）";

    std::cout << "\n[" << age << "歲]: 【跨陣營戀情】你與宿命道侶【" << lover_title << "】暗生情愫，不料陣營對立之事敗露，天下震動！你決定：\n1. 棄絕天下，與其雙宿雙飛（私奔）\n2. 痛改前非，感化對方（改邪歸正）\n3. 憤而出走，雙雙墮魔（反出正道）\n4. 誓死不分，引發兩界正魔大戰！" << std::endl;
    int love_choice; std::cin >> love_choice;

    if (love_choice == 1) {
      PrintLine(" -> 【隱士結局】你們拋棄了宗門與江湖，退隱凡塵。數百年後，傳聞山中有仙侶白日飛升...");
      c.SetDead("與摯愛歸隱山林，不問世事，修得正果。");
      pl.SaveEnd(END_ROMANCE_CROSS);
    } else if (love_choice == 2) {
      if (c.camp == CAMP_DEMON) {
        PrintLine(" -> 在至愛的感化下，你散去全身魔功，拔除魔氣，【改邪歸正】加入正道！ 悟性+5，名望-10");
        c.camp = CAMP_SECT;
        c.SetSectRank(RANK_SECT_INNER);
        c.SetRighteousCultivator(true);
        c.AddStats(0, 5, 0, 0, 0, -10);
      } else {
        PrintLine(" -> 你實力強大，成功感化對方脫離魔道，結為正道神仙眷侶！ 名望+15");
        c.AddStats(0, 0, 0, 0, 0, 15);
      }
      c.romance = ROMANCE_NONE;
    } else if (love_choice == 3) {
      PrintLine(" -> 【雙雙墮魔】你痛恨正道偽善，索性與摯愛反出青雲宗，魔氣灌體，全面加入魔道！ 氣力+15，名望-20");
      c.camp = CAMP_DEMON;
      c.SetSectRank(RANK_DM_FOLLOWER);
      c.SetRighteousCultivator(false); // 拿掉正道 Tag
      c.AddStats(15, 0, 0, 0, 0, -20);
      c.romance = ROMANCE_NONE;
    } else {
      PrintLine(" -> 【千古浩劫：正魔大戰】兩界全面開戰，血流成河！戰場上狼煙四起...");
      int war_roll = std::rand() % 100;
      if (war_roll < 35) {
        PrintLine(" -> [正道慘勝]：正道聯盟攻破魔門，大戰中你的道侶為了替你擋下致命一擊不幸戰死！你萬念俱灰...");
        c.SetDead("摯愛戰死在大戰之中，心碎神傷，抑鬱而終。");
        pl.SaveEnd(END_HAREM_TRAGEDY);
      } else if (war_roll < 70) {
        PrintLine(" -> [魔道大勝]：魔道血洗正統！你與挚愛登臨魔界至高，稱霸寰宇！");
        c.SetDead("登臨魔道統治者，攜手摯愛主宰修仙界。");
        pl.SaveEnd(END_CHAOS_MASTER);
      } else {
        if (c.GetStat().GetStr() >= 50 && c.GetStat().GetWis() >= 50) {
          PrintLine(" -> [強行停戰]：你與道侶雙修大成，實力震古爍今，以無上修為強行威懾兩界停戰，開創正魔共存奇蹟！ 名望+50");
          c.AddStats(10, 10, 10, 5, 0, 50);
          c.romance = ROMANCE_NONE;
        } else {
          PrintLine(" -> [兩敗俱傷]：大戰引發天崩地裂，你與道侶雙雙隕落在金丹自爆的洪流之中。");
          c.SetDead("在正魔大戰的混亂中隕落。");
          pl.SaveEnd(END_MARTYR);
        }
      }
    }
    return;
  }

  // -------------------------------------------------------------------------
  // 5. 常規隨機事件池（包含家產消耗、身分日常通識與江湖高手、魔主晉升）
  // -------------------------------------------------------------------------
  int pool_selector = std::rand() % 100;

  // --- 全陣營通用日常池（替代原本的凡人日常，全陣營可觸發，比率30%） ---
  if (pool_selector < 30) {
    if (std::rand() % 2 == 0) {
      PrintLine("[" + std::to_string(age) + "歲]: (歲月靜好) 通讀各類雜學典籍，雖然日子平淡，但修為穩固。 -> 體魄+1，悟性+1");
      c.AddStats(0, 1, 1, 0, 0, 0);
    } else {
      std::cout << "\n[" << age << "歲]: 【修仙坊市：拍賣盲盒】拍賣行內正在拍賣一卷神秘上古殘頁，你選擇：\n1. 消耗 6 點家產搏一把（盲盒奇遇）\n2. 堅守鐵公雞本色，冷眼旁觀" << std::endl;
      int buy; std::cin >> buy;
      if (buy == 1) {
        if (c.HasTrait(LUCKY_SON) || std::rand() % 100 < 40) {
          PrintLine(" -> 盲盒大爆！居然是失傳的遠古秘術！ 悟性+6，氣運+2");
          c.AddStats(0, 6, 0, 2, -6, 0);
        } else {
          PrintLine(" -> 居然是一張廢紙，你被無良商販騙得傾家蕩產！ 家產-6");
          c.AddStats(0, 0, 0, 0, -6, 0);
        }
      } else {
        PrintLine(" -> 你捂緊錢包，淡定離去。 悟性+1");
        c.AddStats(0, 1, 0, 0, 0, 0);
      }
    }
    return;
  }

  // --- 魔道事件池 (CAMP_DEMON) ---
  if (c.camp == CAMP_DEMON) {
    // 江湖入魔者如果還沒有魔門身分，先賜予雜役
    if (c.GetSectRank() < RANK_DM_SERVANT) c.SetSectRank(RANK_DM_SERVANT);

    if (pool_selector < 60) {
      PrintLine("[" + std::to_string(age) + "歲]: (魔道修行) 你依靠【" + std::to_string(c.GetSpecialization()) + "】魔功吸取天地戾氣。 -> 氣力+3, 體魄+2");
      c.AddStats(3, 0, 2, 0, 0, -2);
    } else if (pool_selector < 85) {
      // 階級晉升事件
      if (c.GetSectRank() == RANK_DM_SERVANT) {
        PrintLine("[" + std::to_string(age) + "歲]: 【魔門大競】你殘忍斬殺同門對手，晉升為【嗜血魔徒】！ 名望+3");
        c.SetSectRank(RANK_DM_FOLLOWER);
      } else if (c.GetSectRank() == RANK_DM_FOLLOWER && c.GetStat().GetStr() >= 35) {
        PrintLine("[" + std::to_string(age) + "歲]: 【權力核心】你深得老魔主器重，被提拔為【魔主左右手】！");
        c.SetSectRank(RANK_DM_RIGHTHAND);
      } else if (c.GetSectRank() == RANK_DM_RIGHTHAND && c.GetStat().GetStr() >= 50) {
        PrintLine("[" + std::to_string(age) + "歲]: 【魔門弒師】你實力大成，深夜突襲反噬老魔主，大獲全勝，篡位成為【魔主】！");
        c.SetSectRank(RANK_DM_LORD);
      } else {
        PrintLine("[" + std::to_string(age) + "歲]: (正道圍剿) 正道高手前來除魔，你依仗身法邊打邊逃。 -> 體魄+3");
        c.AddStats(0, 0, 3, -1, 0, -3);
      }
    } else {
      std::cout << "\n[" << age << "歲]: 【魔道抉擇】一受傷的正道散修攜帶地圖倒在你眼前：\n1. 殘忍搜刮滅口\n2. 趁火打劫，強行簽署道侶契約" << std::endl;
      int act; std::cin >> act;
      if (act == 1) {
        PrintLine(" -> 滅口成功！ 家產+4，氣力+2");
        c.AddStats(2, 0, 0, -2, 4, 0);
      } else {
        c.romance = ROMANCE_TAO_LU; // 觸發連鎖桃花線起點
        PrintLine(" -> 對方屈辱含淚服從，宿命桃花線已埋下種子。");
      }
    }
    return;
  }

  // --- 正道宗門事件池 (CAMP_SECT) ---
  if (c.camp == CAMP_SECT) {
    if (pool_selector < 60) {
      // 依據專精給予特化日常通識回報
      if (c.GetSpecialization() == SPEC_SWORD) {
        PrintLine("[" + std::to_string(age) + "歲]: (內門劍塚) 你在劍塚感悟無上劍意，長劍錚錚作響。 -> 氣力+4");
        c.AddStats(4, 0, 0, 0, 0, 1);
      } else if (c.GetSpecialization() == SPEC_PILL) {
        PrintLine("[" + std::to_string(age) + "歲]: (丹堂煉丹) 消耗 2 點家產購買靈草，煉製出極品聚靈丹。 -> 悟性+5");
        c.AddStats(0, 5, 0, 0, -2, 0);
      } else if (c.GetSpecialization() == SPEC_TALISMAN) {
        PrintLine("[" + std::to_string(age) + "歲]: (符堂畫符) 提筆繪製天雷通神符，符成之時雷光閃爍。 -> 悟性+4");
        c.AddStats(0, 4, 0, 1, 0, 1);
      } else {
        PrintLine("[" + std::to_string(age) + "歲]: (宗門通識) 坐在演武場聽長老講授基礎吐納之法。 -> 體魄+2");
        c.AddStats(0, 0, 2, 0, 0, 1);
      }
    } else if (pool_selector < 85) {
      // 正道高階晉升線
      if (c.GetSectRank() == RANK_SECT_CORE && age >= 50 && c.GetStat().GetWis() >= 45) {
        PrintLine("[" + std::to_string(age) + "歲]: 【晉升長老】修為功德圓滿，你被宗門冊封為【宗門長老】！ 名望+20");
        c.SetSectRank(RANK_SECT_ELDERS);
      } else if (c.GetSectRank() == RANK_SECT_ELDERS && age >= 100 && c.GetStat().GetFame() >= 60) {
        PrintLine("[" + std::to_string(age) + "歲]: 【掌門大任】老掌門仙逝，將掌門大印交付予你，你正式成為【青雲宗掌門】！");
        c.SetSectRank(RANK_SECT_MASTER);
      } else {
        PrintLine("[" + std::to_string(age) + "歲]: (宗門大比) 參加切磋大會，穩紮穩打獲得小勝。 -> 氣力+2, 名望+3");
        c.AddStats(2, 0, 0, 0, 0, 3);
      }
    } else {
      std::cout << "\n[" << age << "歲]: 【宗門歷練】下山除魔，發現妖物作祟：\n1. 布下天羅地網（智取）\n2. 單槍匹馬殺入妖巢（強攻）" << std::endl;
      int act; std::cin >> act;
      if (act == 1) {
        PrintLine(" -> 智取成功！賜下聚靈丹。 悟性+2, 體魄+2");
        c.AddStats(0, 2, 2, 0, 0, 2);
      } else {
        if (c.GetStat().GetStr() >= 25) {
          PrintLine(" -> 橫掃妖巢！ 氣力+4，名望+2");
          c.AddStats(4, 0, 0, 0, 0, 2);
        } else {
          PrintLine(" -> 妖物橫練，你捏碎玉符被救回。 體魄-4, 名望-3");
          c.AddStats(0, 0, -4, 0, 0, -3);
        }
      }
    }
    return;
  }

  // --- 散修江湖事件池 (CAMP_JIANGHU) ---
  if (c.camp == CAMP_JIANGHU) {
    if (pool_selector < 60) {
      PrintLine("[" + std::to_string(age) + "歲]: (江湖浪跡) 作為一名自由散修，你在生死邊緣打磨武藝。 -> 體魄+2, 氣力+2");
      c.AddStats(2, 0, 2, 0, 0, 0);
    } else if (pool_selector < 85) {
      // 江湖散修晉升線
      if (c.GetSectRank() == RANK_JH_NONE && c.GetStat().GetStr() >= 30) {
        PrintLine("[" + std::to_string(age) + "歲]: 【名震一方】你挑翻數個黑道山寨，成為江湖赫赫有名的【一流高手】！");
        c.SetSectRank(RANK_JH_MASTER);
      } else if (c.GetSectRank() == RANK_JH_MASTER && c.GetStat().GetFame() >= 50) {
        PrintLine("[" + std::to_string(age) + "歲]: 【武林泰斗】天下群雄推舉，你一統武林盟，登基成為【武林盟主】！");
        c.SetSectRank(RANK_JH_LEADER);
      } else {
        std::cout << "\n[" << age << "歲]: 【江湖險惡】黑市散修向你兜售一本殘缺魔功：\n1. 鋌而走險修煉魔功（墮魔）\n2. 堅守本心拒絕" << std::endl;
        int act; std::cin >> act;
        if (act == 1) {
          PrintLine(" -> 【因果巨變】你反出正道，墮入魔道！全身魔氣翻湧！ 陣營轉變為【魔道】。");
          c.camp = CAMP_DEMON;
          c.SetSectRank(RANK_DM_FOLLOWER); // 直接轉為魔徒
          c.SetRighteousCultivator(false); // 拿掉正道修士身份
          c.AddStats(10, -5, 0, -2, 0, -10);
        } else {
          PrintLine(" -> 道心堅定，拒絕誘惑。 悟性+4");
          c.AddStats(0, 4, 0, 0, 0, 0);
        }
      }
    } else {
      PrintLine("[" + std::to_string(age) + "歲]: (荒野劫殺) 遭遇散修劫匪，一番血戰後將其驚退。 -> 體魄+2, 家產+2");
      c.AddStats(0, 0, 2, 0, 2, 0);
    }
    return;
  }
}