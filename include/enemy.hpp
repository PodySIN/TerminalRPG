#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "actor.hpp"

namespace rpg {

  class Enemy : public Actor {
  public:
    Enemy(const ActorConfig& config);
    virtual ~Enemy() = default;
    void die() override;
    virtual std::string getType() const = 0;

    virtual std::string getName() const
    {
      return name_;
    }

    void setName(const std::string& name)
    {
      name_ = name;
    }

    size_t getStage() const
    {
      return stage_;
    }

    void setStage(size_t stage)
    {
      stage_ = stage;
    }

  protected:
    std::string name_;
    size_t stage_ = 1;
  };

  class Goblin : public Enemy {
  public:
    Goblin(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Goblin";
    }
  };

  class Wolf : public Enemy {
  public:
    Wolf(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Wolf";
    }
  };

  class Skeleton : public Enemy {
  public:
    Skeleton(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Skeleton";
    }
  };

  class Slime : public Enemy {
  public:
    Slime(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Slime";
    }
  };

  class Bat : public Enemy {
  public:
    Bat(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Bat";
    }
  };

  class Spider : public Enemy {
  public:
    Spider(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Spider";
    }
  };

  class Zombie : public Enemy {
  public:
    Zombie(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Zombie";
    }
  };

  class Cultist : public Enemy {
  public:
    Cultist(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Cultist";
    }
  };

  class Bandit : public Enemy {
  public:
    Bandit(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Bandit";
    }
  };

  class Imp : public Enemy {
  public:
    Imp(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Imp";
    }
  };

  class Harpy : public Enemy {
  public:
    Harpy(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Harpy";
    }
  };

  class Mummy : public Enemy {
  public:
    Mummy(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Mummy";
    }
  };

  class OrcWarrior : public Enemy {
  public:
    OrcWarrior(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Orc Warrior";
    }
  };

  class DarkElf : public Enemy {
  public:
    DarkElf(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Dark Elf";
    }
  };

  class StoneGolem : public Enemy {
  public:
    StoneGolem(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Stone Golem";
    }
  };

  class FireElemental : public Enemy {
  public:
    FireElemental(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Fire Elemental";
    }
  };

  class IceElemental : public Enemy {
  public:
    IceElemental(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Ice Elemental";
    }
  };

  class Thunderbird : public Enemy {
  public:
    Thunderbird(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Thunderbird";
    }
  };

  class ShadowAssassin : public Enemy {
  public:
    ShadowAssassin(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Shadow Assassin";
    }
  };

  class AncientTreant : public Enemy {
  public:
    AncientTreant(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Ancient Treant";
    }
  };

  class TrollKing : public Enemy {
  public:
    TrollKing(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Troll King";
    }
  };

  class Dragon : public Enemy {
  public:
    Dragon(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Dragon";
    }
  };

  class Lich : public Enemy {
  public:
    Lich(size_t stage = 1);
    void setupSkills();

    std::string getType() const override
    {
      return "Lich";
    }
  };

}

#endif
