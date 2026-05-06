#ifndef SKILL_MANAGER_HPP
#define SKILL_MANAGER_HPP

#include "skills.hpp"
#include <cstddef>

namespace rpg {
  class Actor;
  class SkillManager {
  public:
    SkillManager(Actor* owner);
    void useSkill(size_t id) const;
    void useSkill(size_t id, Actor* target) const;
    void useSkill(size_t id, std::vector< Actor* > targets) const;
    void addSkill(Skill* skill);

  private:
    Actor* owner_;
    std::vector< Skill* > skills_;
  };

} // namespace rpg

#endif
