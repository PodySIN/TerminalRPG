#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "hero_account.hpp"
#include <istream>
namespace rpg {
  namespace mmc {
    void newHero(std::istream& in, std::ostream& out, HeroAccount& account);
    void showHero(std::istream& in, std::ostream& out, HeroAccount& account);
    void showHeroes(std::istream& in, std::ostream& out, HeroAccount& account);
    void playAs(std::istream& in, std::ostream& out, HeroAccount& account);
    void deleteHero(std::istream& in, std::ostream& out, HeroAccount& account);
    void newTeammate(std::istream& in, std::ostream& out, HeroAccount& account);
  } // namespace mmc
} // namespace rpg

#endif
