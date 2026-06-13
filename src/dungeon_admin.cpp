#include "dungeon_admin.hpp"
#include <iostream>
#include <cstdio>
#include "random.hpp"

rpg::Dungeon* rpg::DungeonAdmin::activeDungeon()
{
  return active_dungeon_.get();
}

const rpg::Dungeon* rpg::DungeonAdmin::activeDungeon() const
{
  return active_dungeon_.get();
}

bool rpg::DungeonAdmin::hasActiveDungeon() const
{
  return active_dungeon_ != nullptr;
}

std::string rpg::DungeonAdmin::activeDungeonName() const
{
  return active_dungeon_ ? active_dungeon_->name() : "";
}

bool rpg::DungeonAdmin::createDungeon(const std::string& name, size_t floors)
{
  (void)floors;
  auto dungeon = std::make_unique< rpg::Dungeon >(name);
  active_dungeon_ = std::move(dungeon);
  return true;
}

bool rpg::DungeonAdmin::generateDungeon(const std::string& name, size_t floors)
{
  auto dungeon = std::make_unique< rpg::Dungeon >(name);

  std::vector< std::string > common = {"Goblin", "Wolf", "Skeleton", "Slime", "Bat", "Spider", "Zombie"};
  std::vector< std::string > medium = {"Cultist", "Bandit", "Imp", "Harpy", "Mummy"};
  std::vector< std::string > elite = {"Orc Warrior",   "Dark Elf",    "Stone Golem",     "Fire Elemental",
                                      "Ice Elemental", "Thunderbird", "Shadow Assassin", "Ancient Treant"};
  std::vector< std::string > boss = {"Troll King", "Dragon", "Lich"};

  for (size_t i = 0; i < floors; i++) {
    dungeon->addFloor();
    int paths = rpg::Random::getInt(2, 3);
    for (int j = 0; j < paths; j++) {
      dungeon->addRoom(i, "Path " + std::to_string(j + 1));

      int monster_count = rpg::Random::getInt(1, 3);
      bool has_elite = false;

      for (int k = 0; k < monster_count; k++) {
        std::string type;
        size_t level = i + 1;

        if (i == floors - 1 && k == 0) {
          type = boss[rpg::Random::getInt(0, boss.size() - 1)];
          level = i + 2;
        } else if (i >= floors - 3) {
          if (rpg::Random::getFloat(0, 1) < 0.4f) {
            type = elite[rpg::Random::getInt(0, elite.size() - 1)];
            has_elite = true;
          } else {
            type = medium[rpg::Random::getInt(0, medium.size() - 1)];
          }
        } else if (i >= floors / 2) {
          if (rpg::Random::getFloat(0, 1) < 0.3f) {
            type = elite[rpg::Random::getInt(0, elite.size() - 1)];
            has_elite = true;
          } else if (rpg::Random::getFloat(0, 1) < 0.5f) {
            type = medium[rpg::Random::getInt(0, medium.size() - 1)];
          } else {
            type = common[rpg::Random::getInt(0, common.size() - 1)];
          }
        } else {
          if (rpg::Random::getFloat(0, 1) < 0.2f) {
            type = medium[rpg::Random::getInt(0, medium.size() - 1)];
          } else {
            type = common[rpg::Random::getInt(0, common.size() - 1)];
          }
        }

        dungeon->spawnMonster(i, j, type, level);
      }

      rpg::RewardQuality rq;
      if (i == floors - 1) {
        rq = rpg::RewardQuality::Legendary;
      } else if (i >= floors - 3) {
        rq = rpg::RewardQuality::Epic;
      } else if (has_elite) {
        rq = rpg::RewardQuality::Rare;
      } else if (monster_count >= 3) {
        rq = rpg::RewardQuality::Rare;
      } else if (monster_count >= 2) {
        rq = rpg::RewardQuality::Rare;
      } else {
        rq = rpg::RewardQuality::Common;
      }

      dungeon->setRewardQuality(i, j, rq);
    }
  }

  dungeon->save(name);
  active_dungeon_ = std::move(dungeon);
  return true;
}

bool rpg::DungeonAdmin::deleteDungeon(const std::string& name)
{
  if (active_dungeon_ && active_dungeon_->name() == name) {
    active_dungeon_.reset();
  }

  std::string filepath = "dungeons/" + name + ".dng";
  if (std::remove(filepath.c_str()) == 0) {
    return true;
  }

  std::cout << "<INVALID COMMAND> — file not found: " << filepath << "\n";
  return false;
}

bool rpg::DungeonAdmin::loadDungeon(const std::string& name)
{
  auto dungeon = std::make_unique< rpg::Dungeon >();
  if (!dungeon->load(name)) {
    return false;
  }
  active_dungeon_ = std::move(dungeon);
  return true;
}

bool rpg::DungeonAdmin::saveDungeon(const std::string& name)
{
  if (!active_dungeon_)
    return false;
  return active_dungeon_->save(name);
}
