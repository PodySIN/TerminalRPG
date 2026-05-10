#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include "hero_account.hpp"
#include <string>
#include <vector>

namespace rpg {

  struct heroAccount {};
  class SaveManager {
  public:
    SaveManager();

    static void save(const HeroAccount& account);
    static HeroAccount load(const std::string& name);
    static std::vector< std::string > findSavedHeroes();
    static bool heroExists(const std::string& name);
    static void deleteSave(const std::string& name);

  private:
    static std::string getSavePath(const std::string& name);
    static void ensureSaveDirectory();
    static void saveUnit(std::ofstream& file, const UnitSaveData& unit);
    static void loadUnit(std::ifstream& file, UnitSaveData& unit);

    static const std::string saves_dir_;
  };

} // namespace rpg

#endif
