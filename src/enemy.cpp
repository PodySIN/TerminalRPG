#include "enemy.hpp"
#include <iostream>
#include "actor_config.hpp"
#include "skills.hpp"

rpg::Enemy::Enemy(const rpg::ActorConfig& config):
  rpg::Actor(config)
{}

void rpg::Enemy::die()
{
  std::cout << name_ << " has been defeated!\n";
}

rpg::Goblin::Goblin(size_t stage):
  rpg::Enemy(rpg::Presets::Goblin(stage))
{
  name_ = "Goblin";
  stage_ = stage;
  setupSkills();
}

void rpg::Goblin::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Wolf::Wolf(size_t stage):
  rpg::Enemy(rpg::Presets::Wolf(stage))
{
  name_ = "Wolf";
  stage_ = stage;
  setupSkills();
}

void rpg::Wolf::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Skeleton::Skeleton(size_t stage):
  rpg::Enemy(rpg::Presets::Skeleton(stage))
{
  name_ = "Skeleton";
  stage_ = stage;
  setupSkills();
}

void rpg::Skeleton::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Slime::Slime(size_t stage):
  rpg::Enemy(rpg::Presets::Slime(stage))
{
  name_ = "Slime";
  stage_ = stage;
  setupSkills();
}

void rpg::Slime::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Bat::Bat(size_t stage):
  rpg::Enemy(rpg::Presets::Bat(stage))
{
  name_ = "Bat";
  stage_ = stage;
  setupSkills();
}

void rpg::Bat::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Spider::Spider(size_t stage):
  rpg::Enemy(rpg::Presets::Spider(stage))
{
  name_ = "Spider";
  stage_ = stage;
  setupSkills();
}

void rpg::Spider::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Zombie::Zombie(size_t stage):
  rpg::Enemy(rpg::Presets::Zombie(stage))
{
  name_ = "Zombie";
  stage_ = stage;
  setupSkills();
}

void rpg::Zombie::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

rpg::Cultist::Cultist(size_t stage):
  rpg::Enemy(rpg::Presets::Cultist(stage))
{
  name_ = "Cultist";
  stage_ = stage;
  setupSkills();
}

void rpg::Cultist::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto curse = std::make_unique< rpg::Curse >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(curse));
}

rpg::Bandit::Bandit(size_t stage):
  rpg::Enemy(rpg::Presets::Bandit(stage))
{
  name_ = "Bandit";
  stage_ = stage;
  setupSkills();
}

void rpg::Bandit::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto backstab = std::make_unique< rpg::Backstab >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(backstab));
}

rpg::Imp::Imp(size_t stage):
  rpg::Enemy(rpg::Presets::Imp(stage))
{
  name_ = "Imp";
  stage_ = stage;
  setupSkills();
}

void rpg::Imp::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto fireball = std::make_unique< rpg::Fireball >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(fireball));
}

rpg::Harpy::Harpy(size_t stage):
  rpg::Enemy(rpg::Presets::Harpy(stage))
{
  name_ = "Harpy";
  stage_ = stage;
  setupSkills();
}

void rpg::Harpy::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto evasion = std::make_unique< rpg::SpeedBoost >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(evasion));
}

rpg::Mummy::Mummy(size_t stage):
  rpg::Enemy(rpg::Presets::Mummy(stage))
{
  name_ = "Mummy";
  stage_ = stage;
  setupSkills();
}

void rpg::Mummy::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto curse = std::make_unique< rpg::Curse >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(curse));
}

rpg::OrcWarrior::OrcWarrior(size_t stage):
  rpg::Enemy(rpg::Presets::OrcWarrior(stage))
{
  name_ = "Orc Warrior";
  stage_ = stage;
  setupSkills();
}

void rpg::OrcWarrior::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto heavy = std::make_unique< rpg::KnightSlash >();
  auto taunt = std::make_unique< rpg::Taunt >();
  auto shield = std::make_unique< rpg::ShieldBash >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(heavy));
  getSkillManager().addSkill(std::move(taunt));
  getSkillManager().addSkill(std::move(shield));
}

rpg::DarkElf::DarkElf(size_t stage):
  rpg::Enemy(rpg::Presets::DarkElf(stage))
{
  name_ = "Dark Elf";
  stage_ = stage;
  setupSkills();
}

void rpg::DarkElf::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto poison = std::make_unique< rpg::PoisonDagger >();
  auto shadow = std::make_unique< rpg::ShadowStep >();
  auto backstab = std::make_unique< rpg::Backstab >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(poison));
  getSkillManager().addSkill(std::move(shadow));
  getSkillManager().addSkill(std::move(backstab));
}

rpg::StoneGolem::StoneGolem(size_t stage):
  rpg::Enemy(rpg::Presets::StoneGolem(stage))
{
  name_ = "Stone Golem";
  stage_ = stage;
  setupSkills();
}

void rpg::StoneGolem::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto shield = std::make_unique< rpg::ShieldBash >();
  auto protect = std::make_unique< rpg::KnightProtection >();
  auto taunt = std::make_unique< rpg::Taunt >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(protect));
  getSkillManager().addSkill(std::move(taunt));
}

rpg::FireElemental::FireElemental(size_t stage):
  rpg::Enemy(rpg::Presets::FireElemental(stage))
{
  name_ = "Fire Elemental";
  stage_ = stage;
  setupSkills();
}

void rpg::FireElemental::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto fireball = std::make_unique< rpg::Fireball >();
  auto meteor = std::make_unique< rpg::MageUltimate1 >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(fireball));
  getSkillManager().addSkill(std::move(meteor));
}

rpg::IceElemental::IceElemental(size_t stage):
  rpg::Enemy(rpg::Presets::IceElemental(stage))
{
  name_ = "Ice Elemental";
  stage_ = stage;
  setupSkills();
}

void rpg::IceElemental::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto ice = std::make_unique< rpg::IceShard >();
  auto blizzard = std::make_unique< rpg::IceShard >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(ice));
  getSkillManager().addSkill(std::move(blizzard));
}

rpg::Thunderbird::Thunderbird(size_t stage):
  rpg::Enemy(rpg::Presets::Thunderbird(stage))
{
  name_ = "Thunderbird";
  stage_ = stage;
  setupSkills();
}

void rpg::Thunderbird::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto lightning = std::make_unique< rpg::LightningStrike >();
  auto speed = std::make_unique< rpg::SpeedBoost >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(lightning));
  getSkillManager().addSkill(std::move(speed));
}

rpg::ShadowAssassin::ShadowAssassin(size_t stage):
  rpg::Enemy(rpg::Presets::ShadowAssassin(stage))
{
  name_ = "Shadow Assassin";
  stage_ = stage;
  setupSkills();
}

void rpg::ShadowAssassin::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto backstab = std::make_unique< rpg::Backstab >();
  auto poison = std::make_unique< rpg::PoisonDagger >();
  auto assassinate = std::make_unique< rpg::AssassinUltimate2 >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(backstab));
  getSkillManager().addSkill(std::move(poison));
  getSkillManager().addSkill(std::move(assassinate));
}

rpg::AncientTreant::AncientTreant(size_t stage):
  rpg::Enemy(rpg::Presets::AncientTreant(stage))
{
  name_ = "Ancient Treant";
  stage_ = stage;
  setupSkills();
}

void rpg::AncientTreant::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto heal = std::make_unique< rpg::Heal >();
  auto protect = std::make_unique< rpg::KnightProtection >();
  auto ultimate = std::make_unique< rpg::PaladinUltimate1 >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(heal));
  getSkillManager().addSkill(std::move(protect));
  getSkillManager().addSkill(std::move(ultimate));
}

rpg::TrollKing::TrollKing(size_t stage):
  rpg::Enemy(rpg::Presets::TrollKing(stage))
{
  name_ = "Troll King";
  stage_ = stage;
  setupSkills();
}

void rpg::TrollKing::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto heavy = std::make_unique< rpg::KnightSlash >();
  auto taunt = std::make_unique< rpg::Taunt >();
  auto shield = std::make_unique< rpg::ShieldBash >();
  auto ult1 = std::make_unique< rpg::KnightUltimate1 >();
  auto ult2 = std::make_unique< rpg::KnightUltimate2 >();

  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(heavy));
  getSkillManager().addSkill(std::move(taunt));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(ult1));
  getSkillManager().addSkill(std::move(ult2));
}

rpg::Dragon::Dragon(size_t stage):
  rpg::Enemy(rpg::Presets::Dragon(stage))
{
  name_ = "Dragon";
  stage_ = stage;
  setupSkills();
}

void rpg::Dragon::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto fireball = std::make_unique< rpg::Fireball >();
  auto lightning = std::make_unique< rpg::LightningStrike >();
  auto shield = std::make_unique< rpg::ManaShield >();
  auto ult1 = std::make_unique< rpg::MageUltimate1 >();
  auto ult2 = std::make_unique< rpg::MageUltimate2 >();

  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(fireball));
  getSkillManager().addSkill(std::move(lightning));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(ult1));
  getSkillManager().addSkill(std::move(ult2));
}

rpg::Lich::Lich(size_t stage):
  rpg::Enemy(rpg::Presets::Lich(stage))
{
  name_ = "Lich";
  stage_ = stage;
  setupSkills();
}

void rpg::Lich::setupSkills()
{
  auto attack = std::make_unique< rpg::BaseAttack >();
  auto curse = std::make_unique< rpg::Curse >();
  auto heal = std::make_unique< rpg::Heal >();
  auto holy = std::make_unique< rpg::HolySmite >();
  auto ult1 = std::make_unique< rpg::SaintUltimate1 >();
  auto ult2 = std::make_unique< rpg::SaintUltimate2 >();

  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(curse));
  getSkillManager().addSkill(std::move(heal));
  getSkillManager().addSkill(std::move(holy));
  getSkillManager().addSkill(std::move(ult1));
  getSkillManager().addSkill(std::move(ult2));
}
