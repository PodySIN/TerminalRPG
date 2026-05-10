#ifndef GAME_HPP
#define GAME_HPP

#include "actor.hpp"
#include "hero_account.hpp"
#include <memory>
#include <vector>

namespace rpg {

  class Game {
  public:
    void run();

  private:
    void mainMenu();
    void processGame();
    HeroAccount account_;
    std::vector< std::unique_ptr< Actor > > heroes_;
    std::vector< std::unique_ptr< Actor > > enemies_;
  };
} // namespace rpg

#endif
