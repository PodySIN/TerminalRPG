#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

#include "effects.hpp"
#include <memory>
#include <vector>

namespace rpg {
  class Actor;

  class EffectManager {
  public:
    EffectManager(Actor* owner);
    void addEffect(std::unique_ptr< Effect > effect);
    void update();
    std::unique_ptr< Effect > isActorHasEffect(EffectType type) const;

  private:
    Actor* owner_;
    std::vector< std::unique_ptr< Effect > > effects_;
  };

} // namespace rpg
#endif
