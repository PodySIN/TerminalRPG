#ifndef COMBAT_STATS_HPP
#define COMBAT_STATS_HPP

namespace rpg {

  struct BaseStat {
    float base_ = 0.1f;
    void addBase(float value);
    void setBase(float value);
    float getBase() const;
  };

  struct ComplexStat : public BaseStat {
    float flat_ = 0.0f;
    float multiply_ = 1.0f;

    void addFlat(float value);
    void setFlat(float value);
    float getFlat() const;
    void addMultiply(float value);
    void setMultiply(float value);
    float getMultiply() const;
    float getTotal() const;
  };

  class CoreStats {
  public:
    ComplexStat& getHealth();
    const ComplexStat& getHealth() const;
    ComplexStat& getDefense();
    const ComplexStat& getDefense() const;
    ComplexStat& getDamage();
    const ComplexStat& getDamage() const;
    ComplexStat& getSpeed();
    const ComplexStat& getSpeed() const;
    ComplexStat& getResource();
    const ComplexStat& getResource() const;

    BaseStat& getDamageBonus();
    const BaseStat& getDamageBonus() const;
    BaseStat& getCritDamage();
    const BaseStat& getCritDamage() const;
    BaseStat& getCritChance();
    const BaseStat& getCritChance() const;
    BaseStat& getDamageReduction();
    const BaseStat& getDamageReduction() const;
    float& getCurrentHealth();
    const float& getCurrentHealth() const;
    float& getCurrentResource();
    const float& getCurrentResource() const;
    float calculatePhysicalResistance() const;

  private:
    ComplexStat health_;
    float current_health_;
    ComplexStat resource_;
    float current_resource_;
    ComplexStat defense_;
    ComplexStat damage_;
    ComplexStat speed_;
    BaseStat crit_damage_;
    BaseStat crit_chance_;
    BaseStat damage_bonus_;
    BaseStat damage_reduction_;
  };
} // namespace rpg

#endif
