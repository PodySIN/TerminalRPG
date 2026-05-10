#include "game.hpp"
#include "commands.hpp"
#include "hero_account.hpp"
#include <iostream>
#include <limits>
#include <unordered_map>

void rpg::Game::run()
{
  mainMenu();
}

void rpg::Game::mainMenu()
{
  std::cout << "Welcome to terminal rpg!\n";
  std::cout << "You can start new game or load previous\n";
  using cmd_t = void (*)(std::istream&, std::ostream&, HeroAccount&);
  std::unordered_map< std::string, cmd_t > cmds;
  HeroAccount account;
  cmds["new-hero"] = rpg::mmc::newHero;
  cmds["show-heroes"] = rpg::mmc::showHeroes;
  cmds["show-hero"] = rpg::mmc::showHero;
  cmds["delete-hero"] = rpg::mmc::deleteHero;
  cmds["play-as"] = rpg::mmc::playAs;
  cmds["new-teammate"] = rpg::mmc::newTeammate;
  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout, account_);
      if (cmd == "play-as" && account_.name != "") {
        processGame();
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
  std::cout << "Welcome, " << account_.name << "!\n";
}
