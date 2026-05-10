#include "save_manager.hpp"
#include "hero_factory.hpp"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace rpg {
  const std::string SaveManager::saves_dir_ = "saves/";

  std::string SaveManager::getSavePath(const std::string& name)
  {
    return saves_dir_ + name + ".txt";
  }

  void SaveManager::saveUnit(std::ofstream& file, const UnitSaveData& unit)
  {
    file << "hero_class=" << static_cast< int >(unit.hero_class) << "\n";
    file << "current_health=" << unit.current_health << "\n";
    file << "current_resource=" << unit.current_resource << "\n";

    file << "base_health=" << unit.base_health << "\n";
    file << "flat_health=" << unit.flat_health << "\n";
    file << "health_multiply=" << unit.health_multiply << "\n";
    file << "health_amplify=" << unit.health_amplify << "\n";

    file << "base_damage=" << unit.base_damage << "\n";
    file << "flat_damage=" << unit.flat_damage << "\n";
    file << "damage_multiply=" << unit.damage_multiply << "\n";
    file << "damage_amplify=" << unit.damage_amplify << "\n";

    file << "base_defense=" << unit.base_defense << "\n";
    file << "flat_defense=" << unit.flat_defense << "\n";
    file << "defense_multiply=" << unit.defense_multiply << "\n";
    file << "defense_amplify=" << unit.defense_amplify << "\n";

    file << "base_speed=" << unit.base_speed << "\n";
    file << "flat_speed=" << unit.flat_speed << "\n";
    file << "speed_multiply=" << unit.speed_multiply << "\n";
    file << "speed_amplify=" << unit.speed_amplify << "\n";

    file << "base_evasion=" << unit.base_evasion << "\n";
    file << "flat_evasion=" << unit.flat_evasion << "\n";
    file << "evasion_multiply=" << unit.evasion_multiply << "\n";
    file << "evasion_amplify=" << unit.evasion_amplify << "\n";

    file << "base_resource=" << unit.base_resource << "\n";
    file << "flat_resource=" << unit.flat_resource << "\n";
    file << "resource_multiply=" << unit.resource_multiply << "\n";
    file << "resource_amplify=" << unit.resource_amplify << "\n";

    file << "crit_chance=" << unit.crit_chance << "\n";
    file << "crit_damage=" << unit.crit_damage << "\n";
    file << "damage_bonus=" << unit.damage_bonus << "\n";
    file << "effect_resistance=" << unit.effect_resistance << "\n";
    file << "magic_resistance=" << unit.magic_resistance << "\n";
    file << "damage_reduction=" << unit.damage_reduction << "\n";
    file << "block_damage=" << unit.block_damage << "\n";
  }

  void SaveManager::loadUnit(std::ifstream& file, UnitSaveData& unit)
  {
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty() || line[0] == '[')
        break;

      std::istringstream iss(line);
      std::string key, value;
      std::getline(iss, key, '=');
      std::getline(iss, value);

      if (key == "hero_class")
        unit.hero_class = static_cast< HeroClass >(std::stoi(value));
      else if (key == "current_health")
        unit.current_health = std::stof(value);
      else if (key == "current_resource")
        unit.current_resource = std::stof(value);
      else if (key == "base_health")
        unit.base_health = std::stof(value);
      else if (key == "flat_health")
        unit.flat_health = std::stof(value);
      else if (key == "health_multiply")
        unit.health_multiply = std::stof(value);
      else if (key == "health_amplify")
        unit.health_amplify = std::stof(value);
      else if (key == "base_damage")
        unit.base_damage = std::stof(value);
      else if (key == "flat_damage")
        unit.flat_damage = std::stof(value);
      else if (key == "damage_multiply")
        unit.damage_multiply = std::stof(value);
      else if (key == "damage_amplify")
        unit.damage_amplify = std::stof(value);
      else if (key == "base_defense")
        unit.base_defense = std::stof(value);
      else if (key == "flat_defense")
        unit.flat_defense = std::stof(value);
      else if (key == "defense_multiply")
        unit.defense_multiply = std::stof(value);
      else if (key == "defense_amplify")
        unit.defense_amplify = std::stof(value);
      else if (key == "base_speed")
        unit.base_speed = std::stof(value);
      else if (key == "flat_speed")
        unit.flat_speed = std::stof(value);
      else if (key == "speed_multiply")
        unit.speed_multiply = std::stof(value);
      else if (key == "speed_amplify")
        unit.speed_amplify = std::stof(value);
      else if (key == "base_evasion")
        unit.base_evasion = std::stof(value);
      else if (key == "flat_evasion")
        unit.flat_evasion = std::stof(value);
      else if (key == "evasion_multiply")
        unit.evasion_multiply = std::stof(value);
      else if (key == "evasion_amplify")
        unit.evasion_amplify = std::stof(value);
      else if (key == "base_resource")
        unit.base_resource = std::stof(value);
      else if (key == "flat_resource")
        unit.flat_resource = std::stof(value);
      else if (key == "resource_multiply")
        unit.resource_multiply = std::stof(value);
      else if (key == "resource_amplify")
        unit.resource_amplify = std::stof(value);
      else if (key == "crit_chance")
        unit.crit_chance = std::stof(value);
      else if (key == "crit_damage")
        unit.crit_damage = std::stof(value);
      else if (key == "damage_bonus")
        unit.damage_bonus = std::stof(value);
      else if (key == "effect_resistance")
        unit.effect_resistance = std::stof(value);
      else if (key == "magic_resistance")
        unit.magic_resistance = std::stof(value);
      else if (key == "damage_reduction")
        unit.damage_reduction = std::stof(value);
      else if (key == "block_damage")
        unit.block_damage = std::stof(value);
    }
  }

  void SaveManager::save(const HeroAccount& account)
  {
    std::string path = getSavePath(account.name);
    std::ofstream file(path);

    if (!file.is_open()) {
      std::cerr << "Ошибка: не удалось сохранить " << path << "\n";
      return;
    }

    file << "name=" << account.name << "\n";
    file << "current_stage=" << account.current_stage << "\n";

    file << "[main_hero]\n";
    saveUnit(file, account.main_hero);

    file << "[party]\n";
    file << "size=" << account.party.size() << "\n";
    for (const auto& member : account.party) {
      saveUnit(file, member);
    }

    file.close();
    std::cout << "Сохранение " << account.name << " завершено.\n";
  }

  HeroAccount SaveManager::load(const std::string& name)
  {
    std::string path = getSavePath(name);
    std::ifstream file(path);

    if (!file.is_open()) {
      std::cerr << "Ошибка: не удалось загрузить " << path << "\n";
      return HeroAccount{};
    }

    HeroAccount account;
    account.name = name;
    std::string line;

    while (std::getline(file, line)) {
      if (line.empty())
        continue;

      if (line == "[main_hero]") {
        loadUnit(file, account.main_hero);
      } else if (line == "[party]") {
        std::getline(file, line); // читаем size=
        std::istringstream iss(line);
        std::string key, value;
        std::getline(iss, key, '=');
        std::getline(iss, value);
        int party_size = std::stoi(value);

        for (int i = 0; i < party_size; i++) {
          UnitSaveData member;
          loadUnit(file, member);
          account.party.push_back(std::move(member));
        }
      } else {
        std::istringstream iss(line);
        std::string key, value;
        std::getline(iss, key, '=');
        std::getline(iss, value);

        if (key == "current_stage") {
          account.current_stage = std::stoi(value);
        }
      }
    }

    file.close();

    account.main_hero_ptr = createHeroByClass(account.main_hero.hero_class);

    for (const auto& member : account.party) {
      account.party_ptrs.push_back(createHeroByClass(member.hero_class));
    }
    account.applyToHeroes();

    std::cout << "Загрузка " << account.name << " завершена.\n";
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
      if (name.length() > 4 && name.substr(name.length() - 4) == ".txt") {
        heroes.push_back(name.substr(0, name.length() - 4));
      }
    }
    closedir(dir);

    return heroes;
  }

  bool SaveManager::heroExists(const std::string& name)
  {
    std::ifstream file(getSavePath(name));
    return file.good();
  }

  void SaveManager::deleteSave(const std::string& name)
  {
    std::remove(getSavePath(name).c_str());
  }

} // namespace rpg
