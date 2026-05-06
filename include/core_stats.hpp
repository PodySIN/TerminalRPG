#ifndef COMBAT_STATS_HPP
#define COMBAT_STATS_HPP

namespace rpg {

  struct BaseStat {
    float base_ = 10.0f;
    void addBase(float value);
    void setBase(float value);
    float getBase() const;
  };

  struct ComplexStat : public BaseStat {
    float flat_ = 0.0f;
    float amplify_ = 1.0f;
    float multiply_ = 1.0f;

    void addFlat(float value);
    void addAmplify(float value);
    void addMultiply(float value);
    float getTotal() const;
  };

  class CoreStats {
  public:
    ComplexStat& getEvasion();
    const ComplexStat& getEvasion() const;
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

    BaseStat& getEffectResistance();
    const BaseStat& getEffectResistance() const;
    BaseStat& getDamageBonus();
    const BaseStat& getDamageBonus() const;
    BaseStat& getCritDamage();
    const BaseStat& getCritDamage() const;
    BaseStat& getCritChance();
    const BaseStat& getCritChance() const;
    BaseStat& getDamageReduction();
    const BaseStat& getDamageReduction() const;
    BaseStat& getBlockDamage();
    const BaseStat& getBlockDamage() const;
    BaseStat& getMagicResistance();
    const BaseStat& getMagicResistance() const;
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
    ComplexStat evasion_;
    ComplexStat speed_;
    BaseStat damage_bonus_;
    BaseStat crit_damage_;
    BaseStat crit_chance_;
    BaseStat effect_resistance_;
    BaseStat magic_resistance_;
    BaseStat damage_reduction_;
    BaseStat block_damage_;
  };
} // namespace rpg

#endif
