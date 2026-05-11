#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "hero_account.hpp"
#include <istream>
namespace rpg {
  namespace mmc {
    void newAccount(std::istream& in, std::ostream& out, HeroAccount& account);
    void showAccount(std::istream& in, std::ostream& out, HeroAccount& account);
    void showAccounts(std::istream& in, std::ostream& out,
                      HeroAccount& account);
    void login(std::istream& in, std::ostream& out, HeroAccount& account);
    void deleteAccount(std::istream& in, std::ostream& out,
                       HeroAccount& account);
    void newHero(std::istream& in, std::ostream& out, HeroAccount& account);
    void joinWorld(std::istream& in, std::ostream& out, HeroAccount& account);
    void exit(std::istream& in, std::ostream& out, HeroAccount& account);
  } // namespace mmc

  namespace pg {
    void save();
    void exit();
    void exitToMenu();
    void fight();
    void choosePath();
    void useSkill();
    void showBonuses();
    void chooseBonus();
  } // namespace pg
} // namespace rpg

#endif
