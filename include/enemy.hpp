#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "actor.hpp"

namespace rpg {
  class Enemy : public Actor {
  public:
    Enemy();
    ~Enemy();
    Enemy(const Enemy& enemy);
    Enemy(Enemy&& enemy) noexcept;
    Enemy& operator=(const Enemy& enemy);
    Enemy& operator=(Enemy&& enemy) noexcept;
  };
} // namespace rpg

#endif
