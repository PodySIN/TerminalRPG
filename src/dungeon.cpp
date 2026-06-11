#include "dungeon.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include "enemy.hpp"
#include "enemy_factory.hpp"
#include "hero.hpp"
#include "reward.hpp"

rpg::DungeonRoom::DungeonRoom():
  room_id(0),
  reward_quality(rpg::DungeonRoom::RewardQuality::Common)
{}

rpg::DungeonRoom::DungeonRoom(size_t id, const std::string& desc):
  room_id(id),
  description(desc),
  reward_quality(rpg::DungeonRoom::RewardQuality::Common)
{}

rpg::DungeonRoom::~DungeonRoom() = default;

bool rpg::DungeonRoom::hasMonsters() const
{
  return !monsters.empty();
}

size_t rpg::DungeonRoom::getMonsterCount() const
{
  return monsters.size();
}

rpg::DungeonFloor::DungeonFloor():
  floor_number(0)
{}

rpg::DungeonFloor::DungeonFloor(size_t num):
  floor_number(num)
{}

size_t rpg::DungeonFloor::getRoomCount() const
{
  return rooms.size();
}

rpg::DungeonRoom* rpg::DungeonFloor::room(size_t index)
{
  return (index < rooms.size()) ? &rooms[index] : nullptr;
}

const rpg::DungeonRoom* rpg::DungeonFloor::room(size_t index) const
{
  return (index < rooms.size()) ? &rooms[index] : nullptr;
}

size_t rpg::DungeonFloor::addRoom(const std::string& desc)
{
  size_t id = rooms.size();
  rooms.push_back(rpg::DungeonRoom(id, desc));
  return id;
}

bool rpg::DungeonFloor::removeRoom(size_t index)
{
  if (index >= rooms.size())
    return false;
  rooms.erase(rooms.begin() + index);
  for (size_t i = 0; i < rooms.size(); i++)
    rooms[i].room_id = i;
  return true;
}

rpg::Dungeon::Dungeon(const std::string& name):
  name_(name)
{}

size_t rpg::Dungeon::addFloor()
{
  size_t n = floors_.size() + 1;
  floors_.emplace_back(n);
  return floors_.size() - 1;
}

size_t rpg::Dungeon::insertFloor(size_t index)
{
  if (index >= floors_.size())
    return addFloor();
  rpg::DungeonFloor newFloor(0);
  floors_.insert(floors_.begin() + index, std::move(newFloor));
  for (size_t i = 0; i < floors_.size(); i++)
    floors_[i].floor_number = i + 1;
  return index;
}

bool rpg::Dungeon::removeFloor(size_t index)
{
  if (index >= floors_.size())
    return false;
  floors_.erase(floors_.begin() + index);
  for (size_t i = 0; i < floors_.size(); i++)
    floors_[i].floor_number = i + 1;
  return true;
}

rpg::DungeonFloor* rpg::Dungeon::floor(size_t index)
{
  return (index < floors_.size()) ? &floors_[index] : nullptr;
}

const rpg::DungeonFloor* rpg::Dungeon::floor(size_t index) const
{
  return (index < floors_.size()) ? &floors_[index] : nullptr;
}

size_t rpg::Dungeon::floorCount() const
{
  return floors_.size();
}

size_t rpg::Dungeon::addRoom(size_t floor_index, const std::string& desc)
{
  auto* f = floor(floor_index);
  return f ? f->addRoom(desc) : 0;
}

bool rpg::Dungeon::removeRoom(size_t floor_index, size_t room_index)
{
  auto* f = floor(floor_index);
  return f ? f->removeRoom(room_index) : false;
}

void rpg::Dungeon::spawnMonster(size_t floor_index, size_t room_index, const std::string& type, size_t level)
{
  auto* f = floor(floor_index);
  if (!f)
    return;
  auto* r = f->room(room_index);
  if (!r)
    return;
  auto enemy = rpg::createEnemyByType(type, level);
  if (enemy)
    r->monsters.push_back(std::move(enemy));
}

void rpg::Dungeon::removeMonster(size_t floor_index, size_t room_index, size_t monster_index)
{
  auto* f = floor(floor_index);
  if (!f)
    return;
  auto* r = f->room(room_index);
  if (r && monster_index < r->monsters.size())
    r->monsters.erase(r->monsters.begin() + monster_index);
}

void rpg::Dungeon::setRewardQuality(size_t floor_index, size_t room_index, rpg::DungeonRoom::RewardQuality q)
{
  auto* f = floor(floor_index);
  if (!f)
    return;
  auto* r = f->room(room_index);
  if (r)
    r->reward_quality = q;
}

rpg::DungeonRoom::RewardQuality rpg::Dungeon::rewardQuality(size_t floor_index, size_t room_index) const
{
  auto* f = floor(floor_index);
  if (!f)
    return rpg::DungeonRoom::RewardQuality::Common;
  auto* r = f->room(room_index);
  return r ? r->reward_quality : rpg::DungeonRoom::RewardQuality::Common;
}

std::vector< std::unique_ptr< rpg::Reward > > rpg::Dungeon::generateRewards(rpg::Hero& hero) const
{
  auto* r = currentRoom();
  if (!r)
    return {};
  int fnum = static_cast< int >(current_floor_) + 1;
  if (r->reward_quality == rpg::DungeonRoom::RewardQuality::Rare)
    return rpg::RewardFactory::generateRareRewards(hero, fnum);
  return rpg::RewardFactory::generateCommonRewards(fnum);
}

void rpg::Dungeon::start()
{
  completed_ = false;
  in_progress_ = true;
  current_floor_ = 0;
  current_room_ = 0;
}

bool rpg::Dungeon::chooseRoom(size_t room_index)
{
  auto* f = floor(current_floor_);
  if (!f || room_index >= f->getRoomCount())
    return false;
  current_room_ = room_index;
  return true;
}

void rpg::Dungeon::clearRoom()
{
  auto* r = const_cast< rpg::DungeonRoom* >(currentRoom());
  if (r)
    r->monsters.clear();
}

bool rpg::Dungeon::nextFloor()
{
  if (isLastFloor()) {
    completed_ = true;
    in_progress_ = false;
    return false;
  }
  current_floor_++;
  current_room_ = 0;
  return true;
}

const std::string& rpg::Dungeon::name() const
{
  return name_;
}

void rpg::Dungeon::setName(const std::string& n)
{
  name_ = n;
}

size_t rpg::Dungeon::currentFloorIndex() const
{
  return current_floor_;
}

size_t rpg::Dungeon::currentRoomIndex() const
{
  return current_room_;
}

bool rpg::Dungeon::isCompleted() const
{
  return completed_;
}

bool rpg::Dungeon::isInProgress() const
{
  return in_progress_;
}

bool rpg::Dungeon::isLastFloor() const
{
  return current_floor_ >= floors_.size() - 1;
}

const rpg::DungeonRoom* rpg::Dungeon::currentRoom() const
{
  auto* f = floor(current_floor_);
  return f ? f->room(current_room_) : nullptr;
}

const rpg::DungeonFloor* rpg::Dungeon::currentFloor() const
{
  return floor(current_floor_);
}

std::string rpg::Dungeon::showRoomChoice() const
{
  auto* f = currentFloor();
  if (!f)
    return "No floor\n";
  std::stringstream ss;
  ss << "══════════════════════════════\n";
  ss << "📍 Floor " << f->floor_number << "\n";
  ss << "──────────────────────────────\n";
  ss << "Choose your path:\n\n";
  for (size_t i = 0; i < f->getRoomCount(); i++) {
    auto* r = f->room(i);
    ss << "  [" << (i + 1) << "] ";
    ss << (r->description.empty() ? "Room " + std::to_string(i + 1) : r->description) << "\n";
    ss << "      Monsters: ";
    if (r->monsters.empty()) {
      ss << "None\n";
    } else {
      for (size_t j = 0; j < r->monsters.size(); j++) {
        if (j > 0)
          ss << ", ";
        ss << r->monsters[j]->getType() << " (Lvl " << r->monsters[j]->getStage() << ")";
      }
      ss << "\n";
    }
    ss << "      Reward: " << (r->reward_quality == rpg::DungeonRoom::RewardQuality::Rare ? "⭐ Rare" : "Common")
       << "\n\n";
  }
  return ss.str();
}

std::string rpg::Dungeon::showRoom() const
{
  auto* r = currentRoom();
  auto* f = currentFloor();
  if (!r || !f)
    return "Invalid room\n";
  std::stringstream ss;
  ss << "══════════════════════════════\n";
  ss << "📍 Floor " << f->floor_number << " | Room " << (current_room_ + 1) << "/" << f->getRoomCount() << "\n";
  ss << "──────────────────────────────\n";
  if (!r->description.empty())
    ss << "📜 " << r->description << "\n";
  ss << "👾 Monsters: ";
  if (r->monsters.empty()) {
    ss << "None\n";
  } else {
    ss << "\n";
    for (size_t i = 0; i < r->monsters.size(); i++)
      ss << "  " << (i + 1) << ". " << r->monsters[i]->getType() << " (Lvl " << r->monsters[i]->getStage() << ")\n";
  }
  ss << "🎁 Reward: " << (r->reward_quality == rpg::DungeonRoom::RewardQuality::Rare ? "⭐ Rare" : "Common") << "\n";
  ss << "══════════════════════════════\n";
  return ss.str();
}

std::string rpg::Dungeon::showProgress() const
{
  std::stringstream ss;
  ss << "══════════════════════════════\n";
  ss << "🗺️  " << name_ << "\n──────────────────────────────\n";
  ss << "Floor " << (current_floor_ + 1) << " / " << floors_.size() << "\nStatus: ";
  if (completed_)
    ss << "✅ Completed\n";
  else if (in_progress_)
    ss << "⚔️ In Progress\n";
  else
    ss << "⏸️ Not Started\n";
  ss << "══════════════════════════════\n";
  return ss.str();
}

std::string rpg::Dungeon::showFull() const
{
  std::stringstream ss;
  ss << "══════════════════════════════\n";
  ss << "🗺️  " << name_ << "\nFloors: " << floors_.size() << "\n";
  ss << "══════════════════════════════\n\n";
  for (const auto& fl : floors_) {
    ss << "📍 Floor " << fl.floor_number << " (" << fl.getRoomCount() << " rooms)\n";
    for (size_t j = 0; j < fl.getRoomCount(); j++) {
      auto* r = fl.room(j);
      ss << "   [" << j << "] " << (r->description.empty() ? "—" : r->description) << "\n";
      ss << "       Monsters: ";
      if (r->monsters.empty()) {
        ss << "none\n";
      } else {
        ss << "\n";
        for (const auto& m : r->monsters)
          ss << "         • " << m->getType() << " (Lvl " << m->getStage() << ")\n";
      }
      ss << "       Reward: " << (r->reward_quality == rpg::DungeonRoom::RewardQuality::Rare ? "Rare" : "Common")
         << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}

bool rpg::Dungeon::save(const std::string& filename) const
{
  mkdir("dungeons", 0777);
  std::ofstream file("dungeons/" + filename + ".dng", std::ios::binary);
  if (!file)
    return false;
  size_t len = name_.size();
  file.write(reinterpret_cast< const char* >(&len), sizeof(len));
  file.write(name_.c_str(), len);
  len = floors_.size();
  file.write(reinterpret_cast< const char* >(&len), sizeof(len));
  for (const auto& fl : floors_) {
    file.write(reinterpret_cast< const char* >(&fl.floor_number), sizeof(fl.floor_number));
    len = fl.rooms.size();
    file.write(reinterpret_cast< const char* >(&len), sizeof(len));
    for (const auto& room : fl.rooms) {
      file.write(reinterpret_cast< const char* >(&room.room_id), sizeof(room.room_id));
      len = room.description.size();
      file.write(reinterpret_cast< const char* >(&len), sizeof(len));
      file.write(room.description.c_str(), len);
      len = room.monsters.size();
      file.write(reinterpret_cast< const char* >(&len), sizeof(len));
      for (const auto& m : room.monsters) {
        std::string type = m->getType();
        size_t tlen = type.size();
        file.write(reinterpret_cast< const char* >(&tlen), sizeof(tlen));
        file.write(type.c_str(), tlen);
        size_t lvl = m->getStage();
        file.write(reinterpret_cast< const char* >(&lvl), sizeof(lvl));
      }
      int rq = static_cast< int >(room.reward_quality);
      file.write(reinterpret_cast< const char* >(&rq), sizeof(rq));
    }
  }
  file.close();
  return true;
}

bool rpg::Dungeon::load(const std::string& filename)
{
  std::ifstream file("dungeons/" + filename + ".dng", std::ios::binary);
  if (!file)
    return false;
  floors_.clear();
  size_t len;
  file.read(reinterpret_cast< char* >(&len), sizeof(len));
  name_.resize(len);
  file.read(&name_[0], len);
  file.read(reinterpret_cast< char* >(&len), sizeof(len));
  for (size_t i = 0; i < len; i++) {
    size_t fn;
    file.read(reinterpret_cast< char* >(&fn), sizeof(fn));
    rpg::DungeonFloor fl(fn);
    size_t rc;
    file.read(reinterpret_cast< char* >(&rc), sizeof(rc));
    for (size_t j = 0; j < rc; j++) {
      size_t rid;
      file.read(reinterpret_cast< char* >(&rid), sizeof(rid));
      size_t dlen;
      file.read(reinterpret_cast< char* >(&dlen), sizeof(dlen));
      std::string desc(dlen, '\0');
      file.read(&desc[0], dlen);
      rpg::DungeonRoom room(rid, desc);
      size_t mc;
      file.read(reinterpret_cast< char* >(&mc), sizeof(mc));
      for (size_t k = 0; k < mc; k++) {
        size_t tlen;
        file.read(reinterpret_cast< char* >(&tlen), sizeof(tlen));
        std::string type(tlen, '\0');
        file.read(&type[0], tlen);
        size_t lvl;
        file.read(reinterpret_cast< char* >(&lvl), sizeof(lvl));
        auto enemy = rpg::createEnemyByType(type, lvl);
        if (enemy)
          room.monsters.push_back(std::move(enemy));
      }
      int rq;
      file.read(reinterpret_cast< char* >(&rq), sizeof(rq));
      room.reward_quality = static_cast< rpg::DungeonRoom::RewardQuality >(rq);
      fl.rooms.push_back(std::move(room));
    }
    floors_.push_back(std::move(fl));
  }
  file.close();
  return true;
}
