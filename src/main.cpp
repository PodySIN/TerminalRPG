#include "knight.hpp"
#include <iostream>
#include <memory>

int main()
{
  auto knight = std::make_unique< rpg::Knight >();
  auto knight2 = std::make_unique< rpg::Knight >();
  std::cout << "Start\n";
  std::cout << knight->getStats().getHealth().getTotal() << "\n";
  std::cout << "Hp: " << knight2->getStats().getCurrentHealth() << "\n";
  knight->getSkillManager().useSkill(0, knight2.get());
  std::cout << "Hp: " << knight2->getStats().getCurrentHealth() << "\n";
  knight->getSkillManager().useSkill(1, knight2.get());
}
