#include "dungeon_admin.hpp"
#include <iostream>
#include <cstdio>
#include "enemy_factory.hpp"

namespace rpg {

  Dungeon* DungeonAdmin::activeDungeon()
  {
    return active_dungeon_.get();
  }

  const Dungeon* DungeonAdmin::activeDungeon() const
  {
    return active_dungeon_.get();
  }

  bool DungeonAdmin::hasActiveDungeon() const
  {
    return active_dungeon_ != nullptr;
  }

  std::string DungeonAdmin::activeDungeonName() const
  {
    return active_dungeon_ ? active_dungeon_->name() : "";
  }

  bool DungeonAdmin::createDungeon(const std::string& name, size_t floors)
  {
    (void)floors;
    auto dungeon = std::make_unique< Dungeon >(name);
    active_dungeon_ = std::move(dungeon);
    return true;
  }

  bool DungeonAdmin::generateDungeon(const std::string& name, size_t floors)
  {
    auto dungeon = std::make_unique< Dungeon >(name);
    auto types = getAvailableEnemyTypes();

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
        dungeon->setRewardQuality(
            i, j, ((i + 1) % 3 == 0) ? DungeonRoom::RewardQuality::Rare : DungeonRoom::RewardQuality::Common);
      }
    }

    dungeon->save(name);
    active_dungeon_ = std::move(dungeon);
    return true;
  }

  bool DungeonAdmin::deleteDungeon(const std::string& name)
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

  bool DungeonAdmin::loadDungeon(const std::string& name)
  {
    auto dungeon = std::make_unique< Dungeon >();
    if (!dungeon->load(name)) {
      return false;
    }
    active_dungeon_ = std::move(dungeon);
    return true;
  }

  bool DungeonAdmin::saveDungeon(const std::string& name)
  {
    if (!active_dungeon_)
      return false;
    return active_dungeon_->save(name);
  }

}
