#include "knight.hpp"
#include "actor_config.hpp"
#include "skills.hpp"
#include "types.hpp"

rpg::Knight::Knight() : Hero(Presets::Knight())
{
  auto parry_attack = std::make_unique< ParryAttack >(1.25f, ScaleType::Defense,
                                                      DamageType::Physical);
  auto knight_slash = std::make_unique< BaseAttack >();
  auto savage_slash = std::make_unique< SavageSlash >();
  getSkillManager().addSkill(std::move(parry_attack));
  getSkillManager().addSkill(std::move(knight_slash));
  getSkillManager().addSkill(std::move(savage_slash));
}
