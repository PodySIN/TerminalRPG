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
    void newHero(std::istream& in, std::ostream& out, HeroAccount& account)
    {
      std::string name, class_name;
      in >> std::quoted(name) >> class_name;
      if (account.party.size() >= 4) {
        out << "You have 4 heroes, this is maximum party size\n";
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
      out << "Now in your party: " << account.party.size() << " heroes\n";
      SaveManager::save(account);
    }

    void newAccount(std::istream& in, std::ostream& out, HeroAccount& account)
    {
      std::string name;
      in >> std::quoted(name);

      if (SaveManager::heroExists(name)) {
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
      SaveManager::save(account);
    }

    void showAccount(std::istream& in, std::ostream& out, HeroAccount&)
    {
      std::string name;
      in >> std::quoted(name);

      if (!SaveManager::heroExists(name)) {
        out << "Hero not found\n";
        return;
      }

      auto account = SaveManager::load(name);

      out << "Name: " << account.account_name << "\n";
      out << "Your heroes:\n";
      for (size_t i = 0; i < account.party.size(); i++) {
        out << account.party[i].name << " "
            << getClassNameByHeroClass(account.party[i].hero_class) << "\n";
      }
      out << "Stage: " << account.current_stage << "\n";
    }

    void showAccounts(std::istream& in, std::ostream& out, HeroAccount&)
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

    void login(std::istream& in, std::ostream& out, HeroAccount& account)
    {
      std::string name;
      in >> std::quoted(name);

      if (!SaveManager::heroExists(name)) {
        out << "Account not found\n";
        return;
      }

      account = SaveManager::load(name);

      out << "Welcome, " << name << "\n";
    }

    void deleteAccount(std::istream& in, std::ostream& out, HeroAccount&)
    {
      std::string name;
      in >> std::quoted(name);

      if (!SaveManager::heroExists(name)) {
        out << "Account not found\n";
        return;
      }

      SaveManager::deleteSave(name);
      out << "Account deleted\n";
    }

    void joinWorld(std::istream&, std::ostream& out, HeroAccount& account)
    {
      if (account.account_name == "") {
        out << "Please, enter in the account or create new before play the "
               "game\n";
        return;
      }
      out << "Weclome to dungeon, " << account.account_name << "\n";
      out << "Your " << account.party.size() << " heroes:\n";
      for (size_t i = 0; i < account.party.size(); i++) {
        UnitSaveData hero_data = account.party[i];
        out << i + 1 << ") " << hero_data.name << " "
            << getClassNameByHeroClass(hero_data.hero_class) << "\n";
        out << "current hp: " << hero_data.current_health << "\n";
        out << "current resource: " << hero_data.current_resource << "\n";
      }
    }

    void exit(std::istream&, std::ostream&, HeroAccount&)
    {
      std::cout << "Exiting game...\n";
      std::exit(0);
    }

  } // namespace mmc

  namespace pg {
    void save(HeroAccount account)
    {
      SaveManager::save(account);
      std::cout << "Game saved successfully!\n";
    }

    void exit()
    {
      std::cout << "Exiting game...\n";
      std::exit(0);
    }

    void exitToMenu(HeroAccount account)
    {
      SaveManager::save(account);
      std::cout << "Returning to main menu...\n";
      account = HeroAccount{};
    }

    void fight()
    {
    }

    void choosePath()
    {
    }

    void useSkill()
    {
    }

    void showBonuses()
    {
    }

    void chooseBonus()
    {
    }

  } // namespace pg
} // namespace rpg
