#include "game.hpp"
#include <iostream>
#include <limits>
#include <unordered_map>
#include "commands.hpp"
#include "hero_account.hpp"
#include "save_manager.hpp"

void rpg::Game::run()
{
  mainMenu();
}

void rpg::Game::mainMenu()
{
  std::cout << "Welcome to terminal rpg!\n";
  std::cout << "You can start new game or load previous\n";

  std::unordered_map< std::string, cmd_t > cmds;
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

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout, account_);
      if (cmd == "join-world" && account_.account_name != "") {
        processGame();
      }
    } else if (cmd == "admin-mode") {
      adm::adminMode(std::cin, std::cout, account_);
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
  std::unordered_map< std::string, admin_cmd_t > cmds;
  cmds["exit-admin-mode"] = adm::exitAdminMode;
  cmds["create-dungeon"] = adm::dungeonCreate;
  cmds["generate-dungeon"] = adm::dungeonGenerate;
  cmds["delete-dungeon"] = adm::dungeonDelete;
  cmds["show-dungeon"] = adm::dungeonShow;
  cmds["exit"] = [](std::istream&, std::ostream&) { std::exit(0); };
  cmds["show-dungeons"] = [](std::istream& in, std::ostream& out) {
    HeroAccount dummy;
    mmc::showDungeons(in, out, dummy);
  };
  std::string cmd;
  while (std::cin >> cmd) {
    std::cout << cmd << "\n";
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout);
      if (cmd == "exit-admin-mode") {
        return;
      }
    } else if (cmd == "modify-dungeon") {
      adm::modifyMode(std::cin, std::cout);
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
  std::unordered_map< std::string, modify_cmd_t > cmds;
  cmds["exit-modify-mode"] = adm::exitModifyMode;
  cmds["add-stage"] = adm::addStage;
  cmds["remove-stage"] = adm::removeStage;
  cmds["add-path"] = adm::addPath;
  cmds["remove-path"] = adm::removePath;
  cmds["show-stage"] = adm::showStage;
  cmds["show-path"] = adm::showPath;
  cmds["add-monster"] = adm::addMonster;
  cmds["remove-monster"] = adm::removeMonster;
  cmds["change-reward"] = adm::changeReward;
  cmds["show-monsters"] = adm::showMonsterTypes;
  cmds["show-dungeon"] = adm::dungeonShowActive;
  cmds["exit"] = [](std::istream&, std::ostream&) { std::exit(0); };
  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout);
      if (cmd == "exit-modify-mode") {
        return; // Возврат в adminLoop
      }
    } else {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void rpg::Game::processGame()
{
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["save"] = [](std::istream&, std::ostream& out, HeroAccount& acc) {
    SaveManager::save(acc);
    out << "GAME SAVED\n";
  };
  cmds["exit-to-menu"] = [](std::istream&, std::ostream& out, HeroAccount& acc) {
    SaveManager::save(acc);
    out << "Returning to main menu...\n";
  };
  // Будущие команды:
  // cmds["fight"] = pg::fight;
  // cmds["use-skill"] = pg::useSkill;
  // cmds["choose-path"] = pg::choosePath;
  // cmds["show-bonuses"] = pg::showBonuses;
  // cmds["choose-bonus"] = pg::chooseBonus;

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout, account_);
      if (cmd == "exit-to-menu") {
        return; // Возврат в mainMenu
      }
    } else {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}
