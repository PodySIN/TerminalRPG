#ifndef HERO_HPP
#define HERO_HPP

#include "actor.hpp"
#include "types.hpp"

namespace rpg {

  class Hero : public Actor {
  public:
    Hero(const ActorConfig& config);
    void die() override;
    bool getIsDead() const;
    HeroClass getHeroClass() const;
    void setIsDead(bool is_dead);

  protected:
  private:
    HeroClass hero_class_;
    bool is_dead_;
  };

} // namespace rpg

#endif
