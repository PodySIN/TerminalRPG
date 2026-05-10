#ifndef HERO_FACTORY_HPP
#define HERO_FACTORY_HPP

#include "hero.hpp"
#include "knight.hpp"
#include <memory>

namespace rpg {

  inline std::unique_ptr< Hero > createHeroByClass(HeroClass heroClass)
  {
    switch (heroClass) {
      case HeroClass::Knight:
        return std::make_unique< Knight >();
      default:
        return std::make_unique< Knight >();
    }
  }

  inline HeroClass getHeroClassByString(std::string hero_class)
  {
    for (size_t i = 0; i < hero_class.length(); i++) {
      hero_class[i] = std::tolower(hero_class[i]);
    }
    if (hero_class == "knight") {
      return HeroClass::Knight;
    } else if (hero_class == "archer") {
      return HeroClass::Archer;
    } else if (hero_class == "priest") {
      return HeroClass::Priest;
    } else if (hero_class == "palladin") {
      return HeroClass::Palladin;
    } else if (hero_class == "mage") {
      return HeroClass::Mage;
    } else if (hero_class == "assasin") {
      return HeroClass::Assasin;
    } else if (hero_class == "samurai") {
      return HeroClass::Samurai;
    } else if (hero_class == "saint") {
      return HeroClass::Saint;
    } else if (hero_class == "hunter") {
      return HeroClass::Hunter;
    } else if (hero_class == "engineer") {
      return HeroClass::Engineer;
    }
    return HeroClass::NoClass;
  }

} // namespace rpg

#endif
