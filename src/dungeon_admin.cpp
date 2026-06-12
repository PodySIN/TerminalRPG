#include "dungeon_admin.hpp"
#include <iostream>
#include <cstdio>
#include "enemy_factory.hpp"

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
  auto types = rpg::getAvailableEnemyTypes();

  for (size_t i = 0; i < floors; i++) {
    dungeon->addFloor();
    for (size_t j = 0; j < 3; j++) {
      dungeon->addRoom(i, "Path " + std::to_string(j + 1));
      size_t monsterCount = (i + j) % 3 + 1;
      for (size_t k = 0; k < monsterCount; k++) {
        size_t typeIdx = (i * 3 + j + k) % types.size();
        size_t level = i + 1;
        dungeon->spawnMonster(i, j, types[typeIdx], level);
      }
      dungeon->setRewardQuality(i, j,
                                ((i + 1) % 4 == 0)   ? rpg::RewardQuality::Legendary
                                : ((i + 1) % 3 == 0) ? rpg::RewardQuality::Epic
                                : ((i + 1) % 2 == 0) ? rpg::RewardQuality::Rare
                                                     : rpg::RewardQuality::Common);
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
