#include "skill_manager.hpp"
#include <iostream>

void rpg::SkillManager::useSkill(size_t id) const
{
  std::cout << id << "<-id\n";
  skills_[id]->processSkill(owner_, owner_);
}

void rpg::SkillManager::useSkill(size_t id, Actor* target) const
{
  std::cout << id << "<-\n";
  skills_[id]->processSkill(owner_, target);
}

void rpg::SkillManager::useSkill(size_t id, std::vector< Actor* > targets) const
{
  skills_[id]->processSkill(owner_, targets);
}

rpg::SkillManager::SkillManager(Actor* owner) : owner_(owner)
{
}

void rpg::SkillManager::addSkill(std::unique_ptr< Skill > skill)
{
  skills_.push_back(std::move(skill));
}
