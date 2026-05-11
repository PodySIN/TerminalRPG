#include "save_manager.hpp"
#include "hero_factory.hpp"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace rpg {

  const std::string SaveManager::saves_dir_ = "saves/";

  void SaveManager::ensureSaveDirectory()
  {
    mkdir(saves_dir_.c_str(), 0777);
  }

  std::string SaveManager::getSavePath(const std::string& name)
  {
    return saves_dir_ + name + ".dat";
  }

  void SaveManager::saveUnit(std::ofstream& file, const UnitSaveData& unit)
  {
    // Сохраняем имя
    size_t name_len = unit.name.size();
    file.write(reinterpret_cast< const char* >(&name_len), sizeof(name_len));
    file.write(unit.name.c_str(), name_len);

    // Сохраняем класс
    int hero_class_int = static_cast< int >(unit.hero_class);
    file.write(reinterpret_cast< const char* >(&hero_class_int),
               sizeof(hero_class_int));

    // Сохраняем статы
    file.write(reinterpret_cast< const char* >(&unit.current_health),
               sizeof(unit.current_health));
    file.write(reinterpret_cast< const char* >(&unit.current_resource),
               sizeof(unit.current_resource));

    file.write(reinterpret_cast< const char* >(&unit.base_health),
               sizeof(unit.base_health));
    file.write(reinterpret_cast< const char* >(&unit.flat_health),
               sizeof(unit.flat_health));
    file.write(reinterpret_cast< const char* >(&unit.health_multiply),
               sizeof(unit.health_multiply));

    file.write(reinterpret_cast< const char* >(&unit.base_damage),
               sizeof(unit.base_damage));
    file.write(reinterpret_cast< const char* >(&unit.flat_damage),
               sizeof(unit.flat_damage));
    file.write(reinterpret_cast< const char* >(&unit.damage_multiply),
               sizeof(unit.damage_multiply));

    file.write(reinterpret_cast< const char* >(&unit.base_defense),
               sizeof(unit.base_defense));
    file.write(reinterpret_cast< const char* >(&unit.flat_defense),
               sizeof(unit.flat_defense));
    file.write(reinterpret_cast< const char* >(&unit.defense_multiply),
               sizeof(unit.defense_multiply));

    file.write(reinterpret_cast< const char* >(&unit.base_speed),
               sizeof(unit.base_speed));
    file.write(reinterpret_cast< const char* >(&unit.flat_speed),
               sizeof(unit.flat_speed));
    file.write(reinterpret_cast< const char* >(&unit.speed_multiply),
               sizeof(unit.speed_multiply));

    file.write(reinterpret_cast< const char* >(&unit.base_resource),
               sizeof(unit.base_resource));
    file.write(reinterpret_cast< const char* >(&unit.flat_resource),
               sizeof(unit.flat_resource));
    file.write(reinterpret_cast< const char* >(&unit.resource_multiply),
               sizeof(unit.resource_multiply));

    file.write(reinterpret_cast< const char* >(&unit.crit_chance),
               sizeof(unit.crit_chance));
    file.write(reinterpret_cast< const char* >(&unit.crit_damage),
               sizeof(unit.crit_damage));
    file.write(reinterpret_cast< const char* >(&unit.damage_bonus),
               sizeof(unit.damage_bonus));
    file.write(reinterpret_cast< const char* >(&unit.damage_reduction),
               sizeof(unit.damage_reduction));
  }

  void SaveManager::loadUnit(std::ifstream& file, UnitSaveData& unit)
  {
    size_t name_len;
    file.read(reinterpret_cast< char* >(&name_len), sizeof(name_len));
    unit.name.resize(name_len);
    file.read(&unit.name[0], name_len);

    int hero_class_int;
    file.read(reinterpret_cast< char* >(&hero_class_int),
              sizeof(hero_class_int));
    unit.hero_class = static_cast< HeroClass >(hero_class_int);

    file.read(reinterpret_cast< char* >(&unit.current_health),
              sizeof(unit.current_health));
    file.read(reinterpret_cast< char* >(&unit.current_resource),
              sizeof(unit.current_resource));
    file.read(reinterpret_cast< char* >(&unit.base_health),
              sizeof(unit.base_health));
    file.read(reinterpret_cast< char* >(&unit.flat_health),
              sizeof(unit.flat_health));
    file.read(reinterpret_cast< char* >(&unit.health_multiply),
              sizeof(unit.health_multiply));

    file.read(reinterpret_cast< char* >(&unit.base_damage),
              sizeof(unit.base_damage));
    file.read(reinterpret_cast< char* >(&unit.flat_damage),
              sizeof(unit.flat_damage));
    file.read(reinterpret_cast< char* >(&unit.damage_multiply),
              sizeof(unit.damage_multiply));

    file.read(reinterpret_cast< char* >(&unit.base_defense),
              sizeof(unit.base_defense));
    file.read(reinterpret_cast< char* >(&unit.flat_defense),
              sizeof(unit.flat_defense));
    file.read(reinterpret_cast< char* >(&unit.defense_multiply),
              sizeof(unit.defense_multiply));

    file.read(reinterpret_cast< char* >(&unit.base_speed),
              sizeof(unit.base_speed));
    file.read(reinterpret_cast< char* >(&unit.flat_speed),
              sizeof(unit.flat_speed));
    file.read(reinterpret_cast< char* >(&unit.speed_multiply),
              sizeof(unit.speed_multiply));

    file.read(reinterpret_cast< char* >(&unit.base_resource),
              sizeof(unit.base_resource));
    file.read(reinterpret_cast< char* >(&unit.flat_resource),
              sizeof(unit.flat_resource));
    file.read(reinterpret_cast< char* >(&unit.resource_multiply),
              sizeof(unit.resource_multiply));

    file.read(reinterpret_cast< char* >(&unit.crit_chance),
              sizeof(unit.crit_chance));
    file.read(reinterpret_cast< char* >(&unit.crit_damage),
              sizeof(unit.crit_damage));
    file.read(reinterpret_cast< char* >(&unit.damage_bonus),
              sizeof(unit.damage_bonus));
    file.read(reinterpret_cast< char* >(&unit.damage_reduction),
              sizeof(unit.damage_reduction));
  }

  void SaveManager::save(const HeroAccount& account)
  {
    ensureSaveDirectory();
    std::ofstream file(getSavePath(account.account_name), std::ios::binary);

    if (!file.is_open()) {
      std::cerr << "Save error\n";
      return;
    }

    size_t name_len = account.account_name.size();
    file.write(reinterpret_cast< const char* >(&name_len), sizeof(name_len));
    file.write(account.account_name.c_str(), name_len);

    file.write(reinterpret_cast< const char* >(&account.current_stage),
               sizeof(account.current_stage));

    size_t party_size = account.party.size();
    file.write(reinterpret_cast< const char* >(&party_size),
               sizeof(party_size));
    for (const auto& member : account.party) {
      saveUnit(file, member);
    }

    file.close();
    std::cout << "Saved\n";
  }

  HeroAccount SaveManager::load(const std::string& name)
  {
    std::ifstream file(getSavePath(name), std::ios::binary);

    if (!file.is_open()) {
      std::cerr << "Save error\n";
      return HeroAccount{};
    }

    HeroAccount account;
    account.account_name = name;

    size_t name_len;
    file.read(reinterpret_cast< char* >(&name_len), sizeof(name_len));
    file.seekg(name_len, std::ios::cur);

    file.read(reinterpret_cast< char* >(&account.current_stage),
              sizeof(account.current_stage));

    size_t party_size;
    file.read(reinterpret_cast< char* >(&party_size), sizeof(party_size));
    for (size_t i = 0; i < party_size; i++) {
      UnitSaveData member;
      loadUnit(file, member);
      account.party.push_back(std::move(member));
      account.party_ptrs.push_back(createHeroByClass(member.hero_class));
    }

    account.applyToHeroes();

    std::cout << "Loaded\n";
    return account;
  }

  std::vector< std::string > SaveManager::findSavedHeroes()
  {
    std::vector< std::string > heroes;

    DIR* dir = opendir(saves_dir_.c_str());
    if (!dir)
      return heroes;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
      std::string name = entry->d_name;
      if (name.length() > 4 && name.substr(name.length() - 4) == ".dat") {
        heroes.push_back(name.substr(0, name.length() - 4));
      }
    }
    closedir(dir);

    return heroes;
  }

  bool SaveManager::heroExists(const std::string& name)
  {
    std::ifstream file(getSavePath(name), std::ios::binary);
    return file.good();
  }

  void SaveManager::deleteSave(const std::string& name)
  {
    std::remove(getSavePath(name).c_str());
  }

} // namespace rpg
