#include "game.hpp"
#include <iostream>
#include <limits>
#include <unordered_map>
#include "battle_system.hpp"
#include "commands.hpp"
#include "dungeon.hpp"
#include "hero_account.hpp"
#include "save_manager.hpp"
#include "skill_manager.hpp"

void rpg::Game::run()
{
  mainMenu();
}

void rpg::Game::mainMenu()
{
  std::cout << "Welcome to terminal rpg!\n";
  std::cout << "You can start new game or load previous\n";
  std::cout << "┌─────────────────────────────────────────────────────────────┐\n";
  std::cout << "│                   TERMINAL RPG                              │\n";
  std::cout << "├─────────────────────────────────────────────────────────────┤\n";
  std::cout << "│  new-account <name>       - Create new account              │\n";
  std::cout << "│  show-accounts            - List all accounts               │\n";
  std::cout << "│  show-account <name>      - Show account info               │\n";
  std::cout << "│  delete-account <name>    - Delete account                  │\n";
  std::cout << "│  login <name>             - Login to account                │\n";
  std::cout << "│  new-hero <class>         - Create new hero                 │\n";
  std::cout << "│  choose-dungeon <name>    - Select dungeon                  │\n";
  std::cout << "│  show-dungeons            - List available dungeons         │\n";
  std::cout << "│  abandon-dungeon          - Abandon current dungeon         │\n";
  std::cout << "│  dungeon-stats            - Show dungeon statistics         │\n";
  std::cout << "│  join-world               - Start/continue dungeon          │\n";
  std::cout << "│  admin-mode               - Enter admin mode                │\n";
  std::cout << "│  show-commands            - Show this help                  │\n";
  std::cout << "│  exit                     - Exit game                       │\n";
  std::cout << "└─────────────────────────────────────────────────────────────┘\n";
  std::cout << "> ";

  std::unordered_map< std::string, rpg::cmd_t > cmds;
  cmds["new-account"] = rpg::mmc::newAccount;
  cmds["show-accounts"] = rpg::mmc::showAccounts;
  cmds["show-account"] = rpg::mmc::showAccount;
  cmds["delete-account"] = rpg::mmc::deleteAccount;
  cmds["login"] = rpg::mmc::login;
  cmds["new-hero"] = rpg::mmc::newHero;
  cmds["choose-dungeon"] = rpg::mmc::chooseDungeon;
  cmds["show-dungeons"] = rpg::mmc::showDungeons;
  cmds["abandon-dungeon"] = rpg::mmc::abandonDungeon;
  cmds["dungeon-stats"] = rpg::mmc::showDungeonStats;
  cmds["join-world"] = rpg::mmc::joinWorld;
  cmds["exit"] = rpg::mmc::exit;
  cmds["show-commands"] = rpg::mmc::showCommands;

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout, account_);
      if (cmd == "join-world" && account_.account_name != "") {
        processGame();
      }
    } else if (cmd == "admin-mode") {
      rpg::adm::adminMode(std::cin, std::cout, account_);
      adminLoop();
    } else {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void rpg::Game::adminLoop()
{
  std::unordered_map< std::string, rpg::admin_cmd_t > cmds;
  cmds["exit-admin-mode"] = rpg::adm::exitAdminMode;
  cmds["create-dungeon"] = rpg::adm::dungeonCreate;
  cmds["generate-dungeon"] = rpg::adm::dungeonGenerate;
  cmds["delete-dungeon"] = rpg::adm::dungeonDelete;
  cmds["show-dungeon"] = rpg::adm::dungeonShow;
  cmds["exit"] = [](std::istream&, std::ostream&) { std::exit(0); };
  cmds["show-dungeons"] = [](std::istream& in, std::ostream& out) {
    rpg::HeroAccount dummy;
    rpg::mmc::showDungeons(in, out, dummy);
  };
  cmds["show-commands"] = rpg::adm::showAdminCommands;

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout);
      if (cmd == "exit-admin-mode")
        return;
    } else if (cmd == "modify-dungeon") {
      rpg::adm::modifyMode(std::cin, std::cout);
      if (rpg::adm::isModifyActive())
        modifyLoop();
    } else {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void rpg::Game::modifyLoop()
{
  std::unordered_map< std::string, rpg::modify_cmd_t > cmds;
  cmds["exit-modify-mode"] = rpg::adm::exitModifyMode;
  cmds["add-stage"] = rpg::adm::addStage;
  cmds["remove-stage"] = rpg::adm::removeStage;
  cmds["add-path"] = rpg::adm::addPath;
  cmds["remove-path"] = rpg::adm::removePath;
  cmds["show-stage"] = rpg::adm::showStage;
  cmds["show-path"] = rpg::adm::showPath;
  cmds["add-monster"] = rpg::adm::addMonster;
  cmds["remove-monster"] = rpg::adm::removeMonster;
  cmds["change-reward"] = rpg::adm::changeReward;
  cmds["show-monsters"] = rpg::adm::showMonsterTypes;
  cmds["show-dungeon"] = rpg::adm::dungeonShowActive;
  cmds["show-commands"] = rpg::adm::showModifyCommands;
  cmds["show-dungeons"] = [](std::istream& in, std::ostream& out) {
    rpg::HeroAccount dummy;
    rpg::mmc::showDungeons(in, out, dummy);
  };
  cmds["exit"] = [](std::istream&, std::ostream&) { std::exit(0); };

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout);
      if (cmd == "exit-modify-mode")
        return;
    } else {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void rpg::Game::processGame()
{
  if (!account_.hasDungeon()) {
    std::cout << "No dungeon selected\n";
    return;
  }

  account_.startDungeon();

  rpg::Dungeon dungeon;
  if (!dungeon.load(account_.active_dungeon)) {
    std::cout << "Failed to load dungeon\n";
    return;
  }

  dungeon.start();

  for (size_t i = 1; i < account_.current_dungeon_floor; i++) {
    dungeon.nextFloor();
  }

  while (!dungeon.isCompleted()) {
    auto* floor = dungeon.currentFloor();
    if (!floor)
      break;

    std::cout << dungeon.showProgress();
    std::cout << dungeon.showRoomChoice();

    std::cout << "Path> ";
    std::string input;
    std::cin >> input;

    if (input == "exit-to-menu") {
      account_.loadFromHeroes();
      rpg::SaveManager::save(account_);
      std::cout << "Returning to menu...\n";
      return;
    }
    if (input == "exit") {
      std::cout << "Exiting game...\n";
      std::exit(0);
    }
    if (input == "hero-stats") {
      rpg::pg::showHeroStats(std::cin, std::cout, account_);
      continue;
    }
    if (input == "show-commands") {
      std::cout << "  <path_number>  - Choose path\n";
      std::cout << "  hero-stats     - Show hero stats\n";
      std::cout << "  exit-to-menu   - Exit to menu\n";
      std::cout << "  exit           - Exit game\n";
      continue;
    }

    int path_choice = 0;
    try {
      path_choice = std::stoi(input);
    } catch (...) {
      std::cout << "<INVALID>\n";
      continue;
    }

    if (path_choice < 1 || path_choice > (int)floor->getRoomCount()) {
      std::cout << "<INVALID PATH>\n";
      continue;
    }

    dungeon.chooseRoom(path_choice - 1);
    auto* room = dungeon.currentRoom();

    if (room->hasMonsters()) {
      rpg::BattleSystem battle;

      for (auto& hero_ptr : account_.party_ptrs) {
        if (!hero_ptr->getStats().getIsDead())
          battle.addHero(hero_ptr.get());
      }
      for (auto& enemy_ptr : room->monsters) {
        battle.addEnemy(enemy_ptr.get());
      }

      battle.startBattle();
      std::cout << battle.showBattleStatus();

      while (!battle.isBattleOver()) {
        auto* current = battle.currentActor();
        if (!current)
          break;

        if (current->getActorType() == rpg::ActorType::Hero) {
          auto& sm = current->getSkillManager();

          for (size_t i = 0; i < sm.getSkillCount(); i++) {
            if (!sm.isSkillLocked(i)) {
              float cost = sm.getSkillCost(i);
              std::string target = "";
              switch (sm.getSkillTargetType(i)) {
                case rpg::SkillTarget::Enemy:
                  target = "[Enemy]";
                  break;
                case rpg::SkillTarget::Ally:
                  target = "[Ally]";
                  break;
                case rpg::SkillTarget::Self:
                  target = "[Self]";
                  break;
                case rpg::SkillTarget::AllEnemies:
                  target = "[All]";
                  break;
                case rpg::SkillTarget::AllAllies:
                  target = "[All]";
                  break;
              }
              std::cout << "  " << i << ". " << sm.getSkillName(i) << " " << target;
              if (cost > 0)
                std::cout << " [" << cost << " MP]";
              std::cout << " -- " << sm.getSkillDescription(i) << "\n";
            }
          }

          std::cout << "Skill> ";
          std::cin >> input;

          if (input == "exit-to-menu") {
            account_.loadFromHeroes();
            rpg::SaveManager::save(account_);
            std::cout << "Returning to menu...\n";
            return;
          }
          if (input == "exit") {
            std::cout << "Exiting game...\n";
            std::exit(0);
          }
          if (input == "hero-stats") {
            rpg::pg::showHeroStats(std::cin, std::cout, account_);
            continue;
          }
          if (input == "show-commands") {
            std::cout << "  <skill_id>     - Use skill\n";
            std::cout << "  hero-stats     - Show hero stats\n";
            std::cout << "  exit-to-menu   - Exit to menu\n";
            std::cout << "  exit           - Exit game\n";
            continue;
          }

          int skill_idx = 0;
          try {
            skill_idx = std::stoi(input);
          } catch (...) {
            std::cout << "<INVALID>\n";
            continue;
          }

          if (skill_idx < 0 || skill_idx >= (int)sm.getSkillCount() || sm.isSkillLocked(skill_idx)) {
            std::cout << "<INVALID SKILL>\n";
            continue;
          }

          auto target_type = sm.getSkillTargetType(skill_idx);
          size_t target_idx = 0;

          if (target_type == rpg::SkillTarget::Enemy || target_type == rpg::SkillTarget::AllEnemies) {
            auto targets = battle.getAliveEnemies();
            if (targets.size() > 1) {
              std::cout << "Target> ";
              std::cin >> input;

              if (input == "exit-to-menu") {
                account_.loadFromHeroes();
                rpg::SaveManager::save(account_);
                std::cout << "Returning to menu...\n";
                return;
              }
              if (input == "exit") {
                std::cout << "Exiting game...\n";
                std::exit(0);
              }
              if (input == "show-commands") {
                std::cout << "  <target_id>    - Choose target\n";
                std::cout << "  exit-to-menu   - Exit to menu\n";
                std::cout << "  exit           - Exit game\n";
                continue;
              }

              try {
                target_idx = std::stoi(input);
              } catch (...) {
                std::cout << "<INVALID>\n";
                continue;
              }
              if (target_idx >= targets.size()) {
                std::cout << "<INVALID TARGET>\n";
                continue;
              }
            }
          } else if (target_type == rpg::SkillTarget::Ally || target_type == rpg::SkillTarget::AllAllies) {
            auto targets = battle.getAliveHeroes();
            if (targets.size() > 1) {
              std::cout << "Target> ";
              std::cin >> input;

              if (input == "exit-to-menu") {
                account_.loadFromHeroes();
                rpg::SaveManager::save(account_);
                std::cout << "Returning to menu...\n";
                return;
              }
              if (input == "exit") {
                std::cout << "Exiting game...\n";
                std::exit(0);
              }
              if (input == "show-commands") {
                std::cout << "  <target_id>    - Choose target\n";
                std::cout << "  exit-to-menu   - Exit to menu\n";
                std::cout << "  exit           - Exit game\n";
                continue;
              }

              try {
                target_idx = std::stoi(input);
              } catch (...) {
                std::cout << "<INVALID>\n";
                continue;
              }
              if (target_idx >= targets.size()) {
                std::cout << "<INVALID TARGET>\n";
                continue;
              }
            }
          }

          battle.executeHeroAction(skill_idx, target_idx, target_type);
        } else {
          battle.executeEnemyAction();
        }

        std::cout << battle.showBattleStatus();
      }

      if (battle.heroesWon()) {
        std::cout << "VICTORY!\n";
        battle.endBattle();
        dungeon.clearRoom();

        account_.current_dungeon_floor = dungeon.currentFloorIndex() + 2;

        int floor_num = dungeon.currentFloorIndex() + 1;

        for (auto& hero_ptr : account_.party_ptrs) {
          if (hero_ptr->getStats().getIsDead()) {
            if (room->reward_quality == rpg::RewardQuality::Legendary) {
              std::cout << "\n" << hero_ptr->getName() << " is dead. Legendary reward may revive!\n";
            } else {
              std::cout << "\n" << hero_ptr->getName() << " is dead. No reward.\n";
              continue;
            }
          }

          std::cout << "\nREWARD FOR " << hero_ptr->getName() << ":\n";

          std::vector< std::unique_ptr< rpg::Reward > > rewards;
          switch (room->reward_quality) {
            case rpg::RewardQuality::Common:
              rewards = rpg::RewardFactory::generateCommonRewards(floor_num);
              break;
            case rpg::RewardQuality::Rare:
              rewards = rpg::RewardFactory::generateRareRewards(*hero_ptr, floor_num);
              break;
            case rpg::RewardQuality::Epic:
              rewards = rpg::RewardFactory::generateEpicRewards(*hero_ptr, floor_num);
              break;
            case rpg::RewardQuality::Legendary:
              rewards = rpg::RewardFactory::generateLegendaryRewards(*hero_ptr, floor_num);
              break;
          }

          if (!rewards.empty()) {
            while (true) {
              std::cout << "CHOOSE REWARD:\n";
              for (size_t i = 0; i < rewards.size(); i++)
                std::cout << "  " << (i + 1) << ". " << rewards[i]->getDescription() << "\n";
              std::cout << "Reward> ";
              std::cin >> input;

              if (input == "exit-to-menu") {
                account_.loadFromHeroes();
                rpg::SaveManager::save(account_);
                std::cout << "Returning to menu...\n";
                return;
              }
              if (input == "exit") {
                std::cout << "Exiting game...\n";
                std::exit(0);
              }
              if (input == "show-commands") {
                std::cout << "  <reward_number> - Choose reward\n";
                std::cout << "  exit-to-menu    - Exit to menu\n";
                std::cout << "  exit            - Exit game\n";
                continue;
              }

              int rc = 0;
              try {
                rc = std::stoi(input);
              } catch (...) {
                std::cout << "<INVALID>\n";
                continue;
              }
              if (rc >= 1 && rc <= (int)rewards.size()) {
                rewards[rc - 1]->apply(*hero_ptr);
                break;
              }
              std::cout << "<INVALID REWARD>\n";
            }
          }
        }

        account_.loadFromHeroes();
        rpg::SaveManager::save(account_);
      } else {
        std::cout << "DEFEAT!\n";
        battle.endBattle();
        account_.loadFromHeroes();
        account_.failDungeon();
        rpg::SaveManager::save(account_);
        return;
      }
    } else {
      std::cout << "Empty room, moving on...\n";
    }

    if (!dungeon.isLastFloor()) {
      dungeon.nextFloor();
    } else {
      std::cout << "DUNGEON COMPLETED!\n";
      account_.completeDungeon();
      account_.loadFromHeroes();
      rpg::SaveManager::save(account_);
      return;
    }
  }
}
