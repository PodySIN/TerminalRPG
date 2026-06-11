#include "commands.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <dirent.h>
#include "actor.hpp"
#include "dungeon.hpp"
#include "dungeon_admin.hpp"
#include "enemy_factory.hpp"
#include "hero_account.hpp"
#include "hero_factory.hpp"
#include "save_manager.hpp"
#include "types.hpp"

void rpg::mmc::newHero(std::istream& in, std::ostream& out, rpg::HeroAccount& account)
{
  std::string name, class_name;
  in >> std::quoted(name) >> class_name;
  if (account.party.size() >= 4) {
    out << "You have 4 heroes, this is maximum party size\n";
    return;
  }
  rpg::HeroClass teammate_class = rpg::getHeroClassByString(class_name);
  if (teammate_class == rpg::HeroClass::NoClass) {
    out << "Class: " << class_name << " doesnt exist\n";
    return;
  }
  std::unique_ptr< rpg::Hero > new_teammate_ptr = rpg::createHeroByClass(teammate_class);
  rpg::UnitSaveData new_teammate_data;
  new_teammate_data.loadFrom(*new_teammate_ptr);
  new_teammate_data.name = name;
  account.party.push_back(new_teammate_data);
  account.party_ptrs.push_back(std::move(new_teammate_ptr));
  out << "Now in your party: " << account.party.size() << " heroes\n";
  rpg::SaveManager::save(account);
}

void rpg::mmc::chooseDungeon(std::istream& in, std::ostream& out, rpg::HeroAccount& account)
{
  if (account.account_name.empty()) {
    out << "Please login first\n";
    return;
  }
  std::string name;
  in >> std::quoted(name);
  account.chooseDungeon(name);
  rpg::SaveManager::save(account);
}

void rpg::mmc::abandonDungeon(std::istream&, std::ostream& out, rpg::HeroAccount& account)
{
  if (account.account_name.empty()) {
    out << "Please login first\n";
    return;
  }
  account.abandonDungeon();
  rpg::SaveManager::save(account);
}

void rpg::mmc::showDungeonStats(std::istream&, std::ostream& out, rpg::HeroAccount& account)
{
  if (account.dungeon_stats.empty()) {
    out << "No dungeon statistics\n";
    return;
  }
  out << "DUNGEON STATISTICS:\n";
  out << std::string(40, '-') << "\n";
  for (const auto& stat : account.dungeon_stats) {
    out << "  " << stat.dungeon_name << ": " << stat.attempts << " attempts, " << stat.completions << " completions";
    if (account.active_dungeon == stat.dungeon_name) {
      out << (account.isDungeonStarted() ? " *ACTIVE*" : " *CHOSEN*");
    }
    out << "\n";
  }
}

void rpg::mmc::showDungeons(std::istream&, std::ostream& out, rpg::HeroAccount&)
{
  DIR* dir = opendir("dungeons");
  if (!dir) {
    out << "No dungeons found (dungeons/ folder missing)\n";
    return;
  }

  std::vector< std::string > dungeons;
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name.length() > 4 && name.substr(name.length() - 4) == ".dng") {
      dungeons.push_back(name.substr(0, name.length() - 4));
    }
  }
  closedir(dir);

  if (dungeons.empty()) {
    out << "No dungeons available\n";
    return;
  }

  out << "AVAILABLE DUNGEONS:\n";
  out << std::string(40, '-') << "\n";
  for (size_t i = 0; i < dungeons.size(); i++) {
    out << "  " << (i + 1) << ". " << dungeons[i] << "\n";
  }
}

void rpg::mmc::newAccount(std::istream& in, std::ostream& out, rpg::HeroAccount& account)
{
  std::string name;
  in >> std::quoted(name);

  if (rpg::SaveManager::heroExists(name)) {
    out << "Account already exists\n";
    return;
  }
  if (name == "") {
    out << "Please, enter the name\n";
    return;
  }
  account.account_name = name;
  account.party.clear();
  account.party_ptrs.clear();
  rpg::SaveManager::save(account);
}

void rpg::mmc::showAccount(std::istream& in, std::ostream& out, rpg::HeroAccount&)
{
  std::string name;
  in >> std::quoted(name);

  if (!rpg::SaveManager::heroExists(name)) {
    out << "Hero not found\n";
    return;
  }

  auto account = rpg::SaveManager::load(name);

  out << "Name: " << account.account_name << "\n";
  out << "Your heroes:\n";
  for (size_t i = 0; i < account.party.size(); i++) {
    out << account.party[i].name << " " << rpg::getClassNameByHeroClass(account.party[i].hero_class) << "\n";
  }
  out << "Stage: " << account.current_stage << "\n";
}

void rpg::mmc::showAccounts(std::istream& in, std::ostream& out, rpg::HeroAccount&)
{
  (void)in;
  auto heroes = rpg::SaveManager::findSavedHeroes();

  if (heroes.empty()) {
    out << "No heroes\n";
    return;
  }

  for (const auto& name : heroes) {
    out << name << "\n";
  }
}

void rpg::mmc::login(std::istream& in, std::ostream& out, rpg::HeroAccount& account)
{
  std::string name;
  in >> std::quoted(name);

  if (!rpg::SaveManager::heroExists(name)) {
    out << "Account not found\n";
    return;
  }

  account = rpg::SaveManager::load(name);

  out << "Welcome, " << name << "\n";
}

void rpg::mmc::deleteAccount(std::istream& in, std::ostream& out, rpg::HeroAccount&)
{
  std::string name;
  in >> std::quoted(name);

  if (!rpg::SaveManager::heroExists(name)) {
    out << "Account not found\n";
    return;
  }

  rpg::SaveManager::deleteSave(name);
  out << "Account deleted\n";
}

void rpg::mmc::joinWorld(std::istream&, std::ostream& out, rpg::HeroAccount& account)
{
  if (account.account_name == "") {
    out << "Please, enter in the account or create new before play the game\n";
    return;
  }
  if (!account.hasDungeon()) {
    std::cout << "No dungeon chosen. Use 'choose-dungeon <name>'\n";
    return;
  }
  out << "Weclome to dungeon, " << account.account_name << "\n";
  out << "Your " << account.party.size() << " heroes:\n";
  for (size_t i = 0; i < account.party.size(); i++) {
    rpg::UnitSaveData hero_data = account.party[i];
    out << i + 1 << ") " << hero_data.name << " " << rpg::getClassNameByHeroClass(hero_data.hero_class) << "\n";
    out << "current hp: " << hero_data.current_health << "\n";
    out << "current resource: " << hero_data.current_resource << "\n";
  }
}

void rpg::mmc::exit(std::istream&, std::ostream&, rpg::HeroAccount&)
{
  std::cout << "Exiting game...\n";
  std::exit(0);
}

void rpg::pg::exit()
{
  std::cout << "Exiting game...\n";
  std::exit(0);
}

void rpg::pg::save(rpg::HeroAccount& account)
{
  rpg::SaveManager::save(account);
  std::cout << "Game saved successfully!\n";
}

void rpg::pg::exitToMenu(rpg::HeroAccount& account)
{
  rpg::SaveManager::save(account);
  std::cout << "Returning to main menu...\n";
}

void rpg::pg::fight()
{}

void rpg::pg::choosePath()
{}

void rpg::pg::useSkill()
{}

void rpg::pg::showBonuses()
{}

void rpg::pg::chooseBonus()
{}

static bool admin_active = false;
static bool modify_active = false;
static rpg::DungeonAdmin dungeonAdmin;

rpg::DungeonAdmin& rpg::adm::getDungeonAdmin()
{
  return dungeonAdmin;
}

static void autoSave()
{
  if (dungeonAdmin.activeDungeon()) {
    dungeonAdmin.saveDungeon(dungeonAdmin.activeDungeonName());
  }
}

void rpg::adm::adminMode(std::istream&, std::ostream& out, rpg::HeroAccount&)
{
  admin_active = true;
  out << "\n╔═══════════════════════════════════════╗\n";
  out << "║           ADMIN MODE ACTIVE           ║\n";
  out << "╠═══════════════════════════════════════╣\n";
  out << "║  create-dungeon <name> <floors>       ║\n";
  out << "║  generate-dungeon <name> <floors>     ║\n";
  out << "║  modify-dungeon <name>                ║\n";
  out << "║  delete-dungeon <name>                ║\n";
  out << "║  show-dungeon <name>                  ║\n";
  out << "║  show-dungeons                        ║\n";
  out << "║  exit-admin-mode                      ║\n";
  out << "║  exit                                 ║\n";
  out << "╚═══════════════════════════════════════╝\n";
}

void rpg::adm::exitAdminMode(std::istream&, std::ostream& out)
{
  admin_active = false;
  modify_active = false;
  out << "ADMIN MODE DISABLED\n";
}

void rpg::adm::dungeonCreate(std::istream& in, std::ostream& out)
{
  std::string name;
  size_t floors;
  in >> std::quoted(name) >> floors;

  if (dungeonAdmin.createDungeon(name, floors)) {
    auto* d = dungeonAdmin.activeDungeon();
    if (d) {
      for (size_t i = 0; i < floors; i++) {
        d->addFloor();
        for (size_t j = 0; j < 3; j++) {
          d->addRoom(i, "Path " + std::to_string(j + 1));
        }
      }
      d->save(name);
    }
    out << "DUNGEON CREATED: " << name << " (" << floors << " floors, " << (floors * 3) << " paths)\n";
  }
}

void rpg::adm::dungeonShowActive(std::istream&, std::ostream& out)
{
  auto* d = rpg::adm::getDungeonAdmin().activeDungeon();
  if (d)
    out << d->showFull();
  else
    out << "No active dungeon\n";
}

void rpg::adm::dungeonGenerate(std::istream& in, std::ostream& out)
{
  std::string name;
  size_t floors;
  in >> std::quoted(name) >> floors;

  if (dungeonAdmin.generateDungeon(name, floors)) {
    autoSave();
    out << "DUNGEON GENERATED: " << name << " (" << floors << " floors)\n";
  }
}

void rpg::adm::dungeonDelete(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> std::quoted(name);

  if (dungeonAdmin.deleteDungeon(name)) {
    out << "DUNGEON DELETED: " << name << "\n";
  }
}

void rpg::adm::dungeonShow(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> std::quoted(name);

  rpg::Dungeon temp;
  if (temp.load(name)) {
    out << temp.showFull();
  } else {
    out << "<INVALID COMMAND> — dungeon not found\n";
  }
}

void rpg::adm::modifyMode(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> std::quoted(name);

  if (!dungeonAdmin.loadDungeon(name)) {
    out << "<INVALID COMMAND> — dungeon not found. Load it first.\n";
    return;
  }

  modify_active = true;
  out << "\n╔══════════════════════════════════════════════════════════╗\n";
  out << "║ MODIFY MODE: " << std::setw(44) << std::left << name << "║\n";
  out << "╠══════════════════════════════════════════════════════════╣\n";
  out << "║      add-stage <stage>                                   ║\n";
  out << "║      remove-stage <stage>                                ║\n";
  out << "║      add-path <stage> [desc]                             ║\n";
  out << "║      remove-path <stage> <path>                          ║\n";
  out << "║      show-stage <stage>                                  ║\n";
  out << "║      show-path <stage> <path>                            ║\n";
  out << "║      add-monster <st> <p> <type> <lvl>                   ║\n";
  out << "║      remove-monster <st> <p> <idx>                       ║\n";
  out << "║      change-reward <st> <p> <common|rare>                ║\n";
  out << "║      show-monsters                                       ║\n";
  out << "║      show-dungeon                                        ║\n";
  out << "║      exit-modify-mode                                    ║\n";
  out << "║      exit                                                ║\n";
  out << "╚══════════════════════════════════════════════════════════╝\n";
}

void rpg::adm::exitModifyMode(std::istream&, std::ostream& out)
{
  if (dungeonAdmin.activeDungeon()) {
    dungeonAdmin.saveDungeon(dungeonAdmin.activeDungeonName());
    out << "DUNGEON SAVED\n";
  }
  modify_active = false;
  out << "EXIT MODIFY MODE\n";
}

void rpg::adm::addStage(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t index;
  in >> index;

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    size_t idx = d->insertFloor(index - 1);
    for (size_t j = 0; j < 3; j++) {
      d->addRoom(idx, "Path " + std::to_string(j + 1));
    }
    autoSave();
    out << "STAGE ADDED at position " << index << " (3 paths default)\n";
  }
}

void rpg::adm::removeStage(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage;
  in >> stage;

  auto* d = dungeonAdmin.activeDungeon();
  if (d && d->removeFloor(stage - 1)) {
    autoSave();
    out << "STAGE REMOVED: " << stage << "\n";
  } else {
    out << "<INVALID COMMAND> — stage not found\n";
  }
}

void rpg::adm::addPath(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage;
  std::string desc;
  in >> stage >> std::quoted(desc);
  if (!desc.empty() && desc[0] == ' ')
    desc = desc.substr(1);
  if (desc.empty())
    desc = "Path";

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    size_t idx = d->addRoom(stage - 1, desc);
    autoSave();
    out << "PATH ADDED: stage " << stage << ", path " << (idx + 1) << "\n";
  }
}

void rpg::adm::removePath(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage, path;
  in >> stage >> path;

  auto* d = dungeonAdmin.activeDungeon();
  if (d && d->removeRoom(stage - 1, path - 1)) {
    autoSave();
    out << "PATH REMOVED: stage " << stage << ", path " << path << "\n";
  } else {
    out << "<INVALID COMMAND> — path not found\n";
  }
}

void rpg::adm::showStage(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage;
  in >> stage;

  auto* d = dungeonAdmin.activeDungeon();
  if (!d)
    return;

  auto* f = d->floor(stage - 1);
  if (!f) {
    out << "<INVALID COMMAND> — stage not found\n";
    return;
  }

  out << "STAGE " << stage << " (" << f->getRoomCount() << " paths):\n";
  out << std::string(40, '-') << "\n";
  for (size_t i = 0; i < f->getRoomCount(); i++) {
    auto* r = f->room(i);
    out << "Path " << (i + 1) << ": " << (r->description.empty() ? "—" : r->description) << "\n";
    out << "  Monsters: ";
    if (r->monsters.empty()) {
      out << "none\n";
    } else {
      out << "\n";
      for (size_t k = 0; k < r->monsters.size(); k++) {
        out << "    " << (k + 1) << ". " << r->monsters[k]->getType() << " (Lvl " << r->monsters[k]->getStage()
            << ")\n";
      }
    }
    out << "  Reward: " << (r->reward_quality == rpg::DungeonRoom::RewardQuality::Rare ? "Rare" : "Common") << "\n\n";
  }
}

void rpg::adm::showPath(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage, path;
  in >> stage >> path;

  auto* d = dungeonAdmin.activeDungeon();
  if (!d)
    return;

  auto* f = d->floor(stage - 1);
  if (!f) {
    out << "<INVALID COMMAND> — stage not found\n";
    return;
  }

  auto* r = f->room(path - 1);
  if (!r) {
    out << "<INVALID COMMAND> — path not found\n";
    return;
  }

  out << "Stage " << stage << ", Path " << path << "\n";
  out << std::string(40, '-') << "\n";
  out << "Description: " << (r->description.empty() ? "—" : r->description) << "\n";
  out << "Monsters: ";
  if (r->monsters.empty()) {
    out << "none\n";
  } else {
    out << "\n";
    for (size_t i = 0; i < r->monsters.size(); i++) {
      out << "  " << (i + 1) << ". " << r->monsters[i]->getType() << " (Lvl " << r->monsters[i]->getStage() << ")\n";
    }
  }
  out << "Reward: " << (r->reward_quality == rpg::DungeonRoom::RewardQuality::Rare ? "Rare" : "Common") << "\n";
}

void rpg::adm::addMonster(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage, path, level;
  std::string type;
  in >> stage >> path >> std::quoted(type) >> level;

  if (!rpg::isValidEnemyType(type)) {
    out << "<INVALID COMMAND> — unknown monster type: " << type << "\n";
    out << "Use 'show-monsters' to see available types\n";
    return;
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    d->spawnMonster(stage - 1, path - 1, type, level);
    autoSave();
    out << "MONSTER ADDED: " << type << " (Lvl " << level << ")"
        << " to stage " << stage << ", path " << path << "\n";
  }
}

void rpg::adm::removeMonster(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage, path, index;
  in >> stage >> path >> index;

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    d->removeMonster(stage - 1, path - 1, index - 1);
    autoSave();
    out << "MONSTER REMOVED: index " << index << " from stage " << stage << ", path " << path << "\n";
  }
}

void rpg::adm::changeReward(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    out << "<INVALID COMMAND> — not in modify mode\n";
    return;
  }

  size_t stage, path;
  std::string quality;
  in >> stage >> path >> quality;

  rpg::DungeonRoom::RewardQuality rq;
  if (quality == "rare" || quality == "Rare" || quality == "RARE") {
    rq = rpg::DungeonRoom::RewardQuality::Rare;
  } else if (quality == "common" || quality == "Common" || quality == "COMMON") {
    rq = rpg::DungeonRoom::RewardQuality::Common;
  } else {
    out << "<INVALID COMMAND> — use 'common' or 'rare'\n";
    return;
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    d->setRewardQuality(stage - 1, path - 1, rq);
    autoSave();
    out << "REWARD CHANGED: stage " << stage << ", path " << path << " -> " << quality << "\n";
  }
}

void rpg::adm::showMonsterTypes(std::istream&, std::ostream& out)
{
  out << "AVAILABLE MONSTER TYPES:\n";
  out << std::string(40, '-') << "\n";
  auto types = rpg::getAvailableEnemyTypes();
  for (size_t i = 0; i < types.size(); i++) {
    out << "  " << std::setw(2) << (i + 1) << ". " << types[i] << "\n";
  }
}
