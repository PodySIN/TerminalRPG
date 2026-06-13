#include "battle_system.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "random.hpp"
#include "actor.hpp"
#include "enemy.hpp"
#include "skill_manager.hpp"
#include "effect_manager.hpp"

void rpg::BattleSystem::addHero(rpg::Actor* hero)
{
  heroes_.push_back(hero);
}

void rpg::BattleSystem::addEnemy(rpg::Enemy* enemy)
{
  enemies_.push_back(enemy);
}

void rpg::BattleSystem::startBattle()
{
  battle_over_ = false;
  current_turn_ = 0;
  regenMana();
  buildRound();
}

void rpg::BattleSystem::endBattle()
{
  for (auto* h : heroes_)
    h->getEffectManager().removeAllEffects();
  for (auto* e : enemies_)
    e->getEffectManager().removeAllEffects();
}

void rpg::BattleSystem::regenMana()
{
  for (auto* h : heroes_) {
    if (!h->getStats().getIsDead()) {
      float regen = h->getStats().getResourceRegen();
      float max_mp = h->getStats().getResource().getTotal();
      float cur = h->getStats().getCurrentResource();
      h->getStats().getCurrentResource() = std::min(max_mp, cur + regen);
    }
  }
  for (auto* e : enemies_) {
    if (!e->getStats().getIsDead()) {
      float max_mp = e->getStats().getResource().getTotal();
      if (max_mp > 0) {
        float regen = e->getStats().getResourceRegen();
        float cur = e->getStats().getCurrentResource();
        e->getStats().getCurrentResource() = std::min(max_mp, cur + regen);
      }
    }
  }
}

void rpg::BattleSystem::buildRound()
{
  round_queue_.clear();

  for (auto* h : heroes_)
    if (!h->getStats().getIsDead())
      round_queue_.push_back(h);
  for (auto* e : enemies_)
    if (!e->getStats().getIsDead())
      round_queue_.push_back(e);

  std::sort(round_queue_.begin(), round_queue_.end(), [](rpg::Actor* a, rpg::Actor* b) {
    return (10000.0f / a->getStats().getSpeed().getTotal()) < (10000.0f / b->getStats().getSpeed().getTotal());
  });
}

void rpg::BattleSystem::advanceTurn()
{
  if (round_queue_.empty()) {
    regenMana();
    buildRound();
    return;
  }

  current_turn_++;
  round_queue_.erase(round_queue_.begin());

  if (round_queue_.empty()) {
    regenMana();
    buildRound();
  } else {
    std::sort(round_queue_.begin(), round_queue_.end(), [](rpg::Actor* a, rpg::Actor* b) {
      return (10000.0f / a->getStats().getSpeed().getTotal()) < (10000.0f / b->getStats().getSpeed().getTotal());
    });
  }
}

rpg::Actor* rpg::BattleSystem::currentActor()
{
  return round_queue_.empty() ? nullptr : round_queue_[0];
}

size_t rpg::BattleSystem::currentTurn() const
{
  return current_turn_;
}

bool rpg::BattleSystem::isBattleOver() const
{
  return battle_over_;
}

bool rpg::BattleSystem::heroesWon() const
{
  for (auto* e : enemies_)
    if (!e->getStats().getIsDead())
      return false;
  return true;
}

bool rpg::BattleSystem::heroesLost() const
{
  for (auto* h : heroes_)
    if (!h->getStats().getIsDead())
      return false;
  return true;
}

std::string rpg::BattleSystem::showTurnQueue(size_t count) const
{
  std::stringstream ss;
  ss << "TURN QUEUE:\n" << std::string(40, '-') << "\n";

  size_t shown = 0;

  for (size_t i = 0; i < round_queue_.size() && shown < count; i++) {
    auto* actor = round_queue_[i];
    float av = 10000.0f / actor->getStats().getSpeed().getTotal();
    ss << "  " << (shown + 1) << ". " << actor->getName() << " (AV: " << std::fixed << std::setprecision(0) << av
       << ")";
    if (i == 0)
      ss << " <- CURRENT";
    ss << "\n";
    shown++;
  }

  if (shown >= count)
    return ss.str();

  ss << "  " << std::string(36, '-') << "\n";

  std::vector< rpg::Actor* > next_round;
  for (auto* h : heroes_)
    if (!h->getStats().getIsDead())
      next_round.push_back(h);
  for (auto* e : enemies_)
    if (!e->getStats().getIsDead())
      next_round.push_back(e);

  std::sort(next_round.begin(), next_round.end(), [](rpg::Actor* a, rpg::Actor* b) {
    return (10000.0f / a->getStats().getSpeed().getTotal()) < (10000.0f / b->getStats().getSpeed().getTotal());
  });

  for (size_t i = 0; i < next_round.size() && shown < count; i++) {
    auto* actor = next_round[i];
    float av = 10000.0f / actor->getStats().getSpeed().getTotal();
    ss << "  " << (shown + 1) << ". " << actor->getName() << " (AV: " << std::fixed << std::setprecision(0) << av
       << ")\n";
    shown++;
  }

  return ss.str();
}

std::string rpg::BattleSystem::showBattleStatus() const
{
  std::stringstream ss;

  auto padRight = [](std::stringstream& s, int w, const std::string& text) {
    s << text;
    int len = text.length();
    for (int i = 0; i < w - len; i++)
      s << ' ';
  };

  ss << "┌────────────────────────────────┬──────────────────────────────────────────┬─────────────────────────────────"
        "─┐\n";
  ss << "│ TURN QUEUE                     │ HEROES                                   │ ENEMIES                         "
        " │\n";
  ss << "├────────────────────────────────┼──────────────────────────────────────────┼─────────────────────────────────"
        "─┤\n";

  std::vector< rpg::Actor* > current_round = round_queue_;

  std::vector< rpg::Actor* > next_round;
  for (auto* h : heroes_)
    if (!h->getStats().getIsDead())
      next_round.push_back(h);
  for (auto* e : enemies_)
    if (!e->getStats().getIsDead())
      next_round.push_back(e);
  std::sort(next_round.begin(), next_round.end(), [](rpg::Actor* a, rpg::Actor* b) {
    return (10000.0f / a->getStats().getSpeed().getTotal()) < (10000.0f / b->getStats().getSpeed().getTotal());
  });

  const size_t MAX_ROWS = 6;
  size_t max_heroes = heroes_.size();
  size_t max_enemies = enemies_.size();
  size_t rows = std::max(MAX_ROWS, std::max(max_heroes, max_enemies));

  size_t row = 0;

  for (size_t i = 0; i < current_round.size() && row < MAX_ROWS; i++, row++) {
    ss << "│ ";
    auto* actor = current_round[i];
    float av = 10000.0f / actor->getStats().getSpeed().getTotal();
    std::string left = std::to_string(row + 1) + ". " + actor->getName() + " ";
    std::string right = "AV:" + std::to_string((int)av) + (i == 0 ? " <" : "  ");
    padRight(ss, 30 - (int)right.length(), left);
    ss << right;
    ss << " │ ";
    printHero(ss, row);
    ss << " │ ";
    printEnemy(ss, row);
    ss << " │\n";
  }

  if (row < MAX_ROWS && current_round.size() > 0) {
    ss << "│ " << std::string(30, '-') << " │ ";
    printHero(ss, row);
    ss << " │ ";
    printEnemy(ss, row);
    ss << " │\n";
    row++;
  }

  for (size_t i = 0; i < next_round.size() && row < MAX_ROWS; i++, row++) {
    ss << "│ ";
    auto* actor = next_round[i];
    float av = 10000.0f / actor->getStats().getSpeed().getTotal();
    std::string txt = std::to_string(row + 1) + ". " + actor->getName() + " AV:" + std::to_string((int)av) + "  ";
    padRight(ss, 30, txt);
    ss << " │ ";
    printHero(ss, row);
    ss << " │ ";
    printEnemy(ss, row);
    ss << " │\n";
  }

  while (row < rows) {
    ss << "│ " << std::string(30, ' ') << " │ ";
    printHero(ss, row);
    ss << " │ ";
    printEnemy(ss, row);
    ss << " │\n";
    row++;
  }

  ss << "└────────────────────────────────┴──────────────────────────────────────────┴─────────────────────────────────"
        "─┘\n";
  return ss.str();
}

std::vector< rpg::Actor* > rpg::BattleSystem::getAliveHeroes() const
{
  std::vector< rpg::Actor* > alive;
  for (auto* h : heroes_)
    if (!h->getStats().getIsDead())
      alive.push_back(h);
  return alive;
}

std::vector< rpg::Actor* > rpg::BattleSystem::getAliveEnemies() const
{
  std::vector< rpg::Actor* > alive;
  for (auto* e : enemies_)
    if (!e->getStats().getIsDead())
      alive.push_back(e);
  return alive;
}

rpg::Actor* rpg::BattleSystem::findBestTargetForEnemy(rpg::Actor* enemy) const
{
  auto alive = getAliveHeroes();
  if (alive.empty())
    return nullptr;

  auto* taunt = enemy->getEffectManager().getEffect(rpg::EffectType::Taunt);
  if (taunt) {
    auto* tauntEffect = dynamic_cast< rpg::TauntEffect* >(taunt);
    if (tauntEffect && tauntEffect->getTaunter()) {
      rpg::Actor* taunter = tauntEffect->getTaunter();
      if (!taunter->getStats().getIsDead())
        return taunter;
    }
  }

  rpg::Actor* highest_hp = alive[0];
  for (auto* h : alive)
    if (h->getStats().getCurrentHealth() > highest_hp->getStats().getCurrentHealth())
      highest_hp = h;

  if (rpg::Random::getFloat(0.0f, 1.0f) < 0.6f)
    return highest_hp;

  return alive[rpg::Random::getInt(0, alive.size() - 1)];
}

size_t rpg::BattleSystem::chooseBestSkillForEnemy(rpg::Actor* enemy) const
{
  auto& sm = enemy->getSkillManager();
  size_t count = sm.getSkillCount();
  float hp_percent = enemy->getStats().getCurrentHealth() / enemy->getStats().getHealth().getTotal();

  for (size_t i = 0; i < count; i++) {
    if (sm.isSkillLocked(i))
      continue;
    std::string name = sm.getSkillName(i);
    if (hp_percent < 0.3f) {
      if (name.find("Heal") != std::string::npos)
        return i;
      if (name.find("Shield") != std::string::npos)
        return i;
      if (name.find("Protection") != std::string::npos)
        return i;
    }
  }

  for (size_t i = 0; i < count; i++) {
    if (sm.isSkillLocked(i))
      continue;
    return i;
  }
  return 0;
}

void rpg::BattleSystem::executeHeroAction(size_t skill_index, size_t target_index, rpg::SkillTarget target_type)
{
  auto* hero = currentActor();
  if (!hero)
    return;

  if (hero->getStats().isStunned()) {
    std::cout << hero->getName() << " is stunned!\n";
    advanceTurn();
    return;
  }

  auto& sm = hero->getSkillManager();
  float cost = sm.getSkillCost(skill_index);
  float mp = hero->getStats().getCurrentResource();
  if (mp < cost) {
    std::cout << "Not enough MP! Need " << cost << ", have " << mp << "\n";
    return;
  }
  hero->getStats().getCurrentResource() -= cost;

  rpg::Actor* target = nullptr;

  if (target_type == rpg::SkillTarget::Enemy || target_type == rpg::SkillTarget::AllEnemies) {
    auto targets = getAliveEnemies();
    if (target_index < targets.size())
      target = targets[target_index];
  } else if (target_type == rpg::SkillTarget::Ally || target_type == rpg::SkillTarget::AllAllies) {
    auto targets = getAliveHeroes();
    if (target_index < targets.size())
      target = targets[target_index];
  } else if (target_type == rpg::SkillTarget::Self) {
    target = hero;
  }

  if (target) {
    sm.useSkill(skill_index, target);
  }

  if (heroesWon()) {
    battle_over_ = true;
    return;
  }
  advanceTurn();
}

void rpg::BattleSystem::executeEnemyAction()
{
  auto* enemy = currentActor();
  if (!enemy)
    return;

  if (enemy->getStats().isStunned()) {
    std::cout << enemy->getName() << " is stunned and cannot act!\n";
    advanceTurn();
    return;
  }

  size_t skill_idx = chooseBestSkillForEnemy(enemy);
  rpg::Actor* target = findBestTargetForEnemy(enemy);
  if (target)
    enemy->getSkillManager().useSkill(skill_idx, target);
  if (heroesLost()) {
    battle_over_ = true;
    return;
  }
  advanceTurn();
}

void rpg::BattleSystem::printHero(std::stringstream& ss, size_t row) const
{
  if (row < heroes_.size()) {
    auto* h = heroes_[row];
    auto& s = h->getStats();
    std::string txt = "[" + std::to_string(row) + "] " + h->getName() +
                      "  HP:" + std::to_string((int)s.getCurrentHealth()) + "/" +
                      std::to_string((int)s.getHealth().getTotal());
    if (s.getShield() > 0)
      txt += " S:" + std::to_string((int)s.getShield());
    txt +=
        "  MP:" + std::to_string((int)s.getCurrentResource()) + "/" + std::to_string((int)s.getResource().getTotal());

    int len = txt.length();
    ss << txt;
    for (int i = 0; i < 40 - len; i++)
      ss << ' ';
  } else {
    ss << std::string(40, ' ');
  }
}

void rpg::BattleSystem::printEnemy(std::stringstream& ss, size_t row) const
{
  if (row < enemies_.size()) {
    auto* e = enemies_[row];
    auto& s = e->getStats();
    std::string txt = "[" + std::to_string(row) + "] " + e->getName() +
                      "  HP:" + std::to_string((int)s.getCurrentHealth()) + "/" +
                      std::to_string((int)s.getHealth().getTotal());
    if (s.getShield() > 0)
      txt += " S:" + std::to_string((int)s.getShield());

    int len = txt.length();
    ss << txt;
    for (int i = 0; i < 32 - len; i++)
      ss << ' ';
  } else {
    ss << std::string(32, ' ');
  }
}
