#include "commands.hpp"
#include "actor.hpp"
#include "hero_account.hpp"
#include "hero_factory.hpp"
#include "save_manager.hpp"
#include "types.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

namespace rpg {
  namespace mmc {
    void newTeammate(std::istream& in, std::ostream& out, HeroAccount& account)
    {
      std::string name, class_name;
      in >> std::quoted(name) >> class_name;
      if (account.party_size >= 3) {
        out << "You have 3 teammates, this is maximum party size\n";
        return;
      }
      HeroClass teammate_class = rpg::getHeroClassByString(class_name);
      if (teammate_class == HeroClass::NoClass) {
        out << "Class: " << class_name << " doesnt exist\n";
        return;
      }
      std::unique_ptr< Hero > new_teammate_ptr =
          createHeroByClass(teammate_class);
      UnitSaveData new_teammate_data;
      new_teammate_data.loadFrom(*new_teammate_ptr);
      new_teammate_data.name = name;
      account.party.push_back(new_teammate_data);
      account.party_ptrs.push_back(std::move(new_teammate_ptr));
      account.party_size++;
      SaveManager::save(account);
    }

    void newHero(std::istream& in, std::ostream& out, HeroAccount& account)
    {
      std::string name;
      in >> std::quoted(name);

      if (SaveManager::heroExists(name)) {
        out << "Hero already exists\n";
        return;
      }

      std::string class_name;
      in >> class_name;

      HeroClass main_class = getHeroClassByString(class_name);
      if (main_class == HeroClass::NoClass) {
        out << "Class: " << class_name << " doesnt exist\n";
        return;
      }
      account.name = name;
      account.main_hero_ptr = createHeroByClass(main_class);
      if (!account.main_hero_ptr) {
        out << "Failed to create hero\n";
        return;
      }

      account.main_hero.loadFrom(*account.main_hero_ptr);
      account.main_hero.name = name;
      account.party.clear();
      account.party_ptrs.clear();

      SaveManager::save(account);
    }

    void showHero(std::istream& in, std::ostream& out, HeroAccount&)
    {
      std::string name;
      in >> std::quoted(name);

      if (!SaveManager::heroExists(name)) {
        out << "Hero not found\n";
        return;
      }

      auto account = SaveManager::load(name);

      out << "Name: " << account.name << "\n";
      out << "Stage: " << account.current_stage << "\n";
    }

    void showHeroes(std::istream& in, std::ostream& out, HeroAccount&)
    {
      (void)in;
      auto heroes = SaveManager::findSavedHeroes();

      if (heroes.empty()) {
        out << "No heroes\n";
        return;
      }

      for (const auto& name : heroes) {
        out << name << "\n";
      }
    }

    void playAs(std::istream& in, std::ostream& out, HeroAccount& account)
    {
      std::string name;
      in >> std::quoted(name);

      if (!SaveManager::heroExists(name)) {
        out << "Hero not found\n";
        return;
      }

      account = SaveManager::load(name);

      out << "Playing as " << name << "\n";
      out << "Stage " << account.current_stage << "\n";
    }

    void deleteHero(std::istream& in, std::ostream& out, HeroAccount&)
    {
      std::string name;
      in >> std::quoted(name);

      if (!SaveManager::heroExists(name)) {
        out << "Hero not found\n";
        return;
      }

      SaveManager::deleteSave(name);
      out << "Hero deleted\n";
    }

  } // namespace mmc
} // namespace rpg
