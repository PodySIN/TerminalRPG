#include "knight.hpp"
#include <iostream>
#include <memory>

int main()
{
  auto knight = std::make_unique< rpg::Knight >();
  std::cout << "Start\n";
  std::cout << knight->getStats().getHealth().getTotal() << "\n";
}
