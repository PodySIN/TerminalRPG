#include "skill_manager.hpp"

void rpg::SkillManager::useSkill(size_t id) const
{
  skills_[id]->processSkill(owner_, owner_);
}

void rpg::SkillManager::useSkill(size_t id, Actor* target) const
{
  skills_[id]->processSkill(owner_, target);
}

void rpg::SkillManager::useSkill(size_t id, std::vector< Actor* > targets) const
{
  skills_[id]->processSkill(owner_, targets);
}

rpg::SkillManager::SkillManager(Actor* owner) : owner_(owner), skills_(5)
{
}

void rpg::SkillManager::addSkill(Skill* skill)
{
  skills_.push_back(skill);
}
