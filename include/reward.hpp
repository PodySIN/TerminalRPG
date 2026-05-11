#ifndef REWARD_HPP
#define REWARD_HPP

#include "hero.hpp"
#include <memory>
#include <string>
#include <vector>

namespace rpg {

  class Reward {
  public:
    virtual ~Reward() = default;
    Reward(const std::string& description);
    virtual void apply(Hero& hero) const = 0;
    std::string getDescription() const;

  private:
    std::string description_;
  };

  class HealthReward : public Reward {
  public:
    HealthReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class ResourceReward : public Reward {
  public:
    ResourceReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class DamageReward : public Reward {
  public:
    DamageReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class DefenseReward : public Reward {
  public:
    DefenseReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class SpeedReward : public Reward {
  public:
    SpeedReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class EvasionReward : public Reward {
  public:
    EvasionReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class CritChanceReward : public Reward {
  public:
    CritChanceReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class CritDamageReward : public Reward {
  public:
    CritDamageReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class BlockDamageReward : public Reward {
  public:
    BlockDamageReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class MagicResistanceReward : public Reward {
  public:
    MagicResistanceReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class PercentHealthReward : public Reward {
  public:
    PercentHealthReward(float percent);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class PercentDamageReward : public Reward {
  public:
    PercentDamageReward(float percent);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class RewardFactory {
  public:
    static std::unique_ptr< Reward > createRandomCommonReward();
    static std::unique_ptr< Reward > createRandomRareReward();
    static std::unique_ptr< Reward > createRandomEpicReward();
    static std::vector< std::unique_ptr< Reward > > generateRewards(int count,
                                                                    int floor);
  };

} // namespace rpg

#endif
