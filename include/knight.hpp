#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "hero.hpp"

namespace rpg {
  class Knight : public Hero {
  public:
    Knight();
    void die() override;

  private:
  };
} // namespace rpg

#endif
