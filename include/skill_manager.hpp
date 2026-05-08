#ifndef SKILL_MANAGER_HPP
#define SKILL_MANAGER_HPP

#include "skills.hpp"
#include <cstddef>
#include <memory>

namespace rpg {
  class Actor;
  class SkillManager {
  public:
    SkillManager(Actor* owner);
    void useSkill(size_t id) const;
    void useSkill(size_t id, Actor* target) const;
    void useSkill(size_t id, std::vector< Actor* > targets) const;
    void addSkill(std::unique_ptr< Skill > skill);

  private:
    Actor* owner_;
    std::vector< std::unique_ptr< Skill > > skills_;
  };

} // namespace rpg

#endif
