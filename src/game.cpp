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
  cmds["new-account"] = rpg::mmc::newAccount;
  cmds["show-accounts"] = rpg::mmc::showAccounts;
  cmds["show-account"] = rpg::mmc::showAccount;
  cmds["delete-account"] = rpg::mmc::deleteAccount;
  cmds["login"] = rpg::mmc::login;
  cmds["new-hero"] = rpg::mmc::newHero;
  cmds["join-world"] = rpg::mmc::joinWorld;
  cmds["exit"] = rpg::mmc::exit;
  std::string cmd;
  while (std::cin >> cmd) {
    if (cmds.find(cmd) != cmds.end()) {
      cmds.at(cmd)(std::cin, std::cout, account_);
      if (cmd == "join-world" && account_.account_name != "") {
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
  std::cout << "processGame\n";
}
