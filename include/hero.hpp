#ifndef HERO_HPP
#define HERO_HPP

#include "actor.hpp"

namespace rpg {
  class Hero : public Actor {
  public:
    Hero(const ActorConfig& config);

  protected:
  private:
  };

} // namespace rpg

#endif
