#include "knight.hpp"
#include "actor_config.hpp"
#include "skills.hpp"

rpg::Knight::Knight() : Hero(Presets::Knight())
{
  auto knight_slash = std::make_unique< KnightSlash >();
  auto savage_slash = std::make_unique< SavageSlash >();
  getSkillManager().addSkill(std::move(knight_slash));
  getSkillManager().addSkill(std::move(savage_slash));
}
