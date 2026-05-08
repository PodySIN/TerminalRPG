#ifndef HERO_HPP
#define HERO_HPP

#include "actor.hpp"

namespace rpg {
  class Hero : public Actor {
  public:
    Hero(const ActorConfig& config);
    void die() override;
    bool getIsDead() const;
    void setIsDead(bool is_dead);

  protected:
  private:
    bool is_dead_;
  };

} // namespace rpg

#endif
