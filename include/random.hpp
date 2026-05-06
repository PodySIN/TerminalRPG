#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace rpg {
  class Random {
  public:
    static int getInt(int min, int max);
    static float getFloat(float min, float max);

    Random() = delete;
  };
} // namespace rpg

#endif
