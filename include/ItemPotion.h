#ifndef ITEM_POTION_H
#define ITEM_POTION_H

#include <vector>
#include <string>

#include "Item.h"
#include "CmnData.h"

class Potion: public Item {
public:
  Potion(ItemDataT* const itemData) :
    Item(itemData) {}

  virtual ~Potion() {}

  ConsumeItem activateDefault(Actor* const actor) override {
    quaff(actor);
    return ConsumeItem::yes;
  }

  Clr getInterfaceClr() const override {return clrBlueLgt;}

  void quaff(Actor* const actor);

  void collide(const Pos& pos, Actor* actor);

  void identify(const bool IS_SILENT_IDENTIFY) override;

  std::vector<std::string> getDescr() const override final {
    if(data_->isIdentified) {
      return getDescrIdentified();
    } else {
      return data_->baseDescr;
    }
  }

protected:
  virtual const std::string getRealTypeName() = 0;

  virtual std::vector<std::string> getDescrIdentified() const = 0;

  virtual void collide_(const Pos& pos, Actor* const actor) = 0;

  virtual void quaff_(Actor* const actor) {(void) actor;}

  std::string getNameInf() const override final;
};

class PotionVitality: public Potion {
public:
  PotionVitality(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionVitality() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Vitality";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"This elixir heals all wounds and physical maladies."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionSpirit: public Potion {
public:
  PotionSpirit(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionSpirit() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Spirit";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Restores the spirit."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionBlindness: public Potion {
public:
  PotionBlindness(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionBlindness() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Blindness";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Causes temporary loss of vision."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionParal: public Potion {
public:
  PotionParal(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionParal() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Paralyzation";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Causes paralysis."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionDisease: public Potion {
public:
  PotionDisease(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionDisease() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Disease";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"This foul liquid causes a horrible disease."};
  }

  void collide_(const Pos& pos, Actor* const actor) override {
    (void)pos;
    (void)actor;
  }
};

class PotionConf: public Potion {
public:
  PotionConf(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionConf() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Confusion";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Causes confusion."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionFrenzy: public Potion {
public:
  PotionFrenzy(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionFrenzy() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Frenzy";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Causes uncontrollable fury."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionFortitude: public Potion {
public:
  PotionFortitude(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionFortitude() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Fortitude";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Gives the consumer complete peace of mind."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionPoison: public Potion {
public:
  PotionPoison(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionPoison() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Poison";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"A deadly brew."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionInsight: public Potion {
public:
  PotionInsight(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionInsight() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Insight";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"This strange concoction causes a sudden flash of intuition."};
  }

  void collide_(const Pos& pos, Actor* const actor) override {
    (void)pos;
    (void)actor;
  }
};

class PotionClairv: public Potion {
public:
  PotionClairv(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionClairv() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Clairvoyance";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Bestows visions of the surrounding area."};
  }

  void collide_(const Pos& pos, Actor* const actor) override {
    (void)pos;
    (void)actor;
  }
};

class PotionRFire: public Potion {
public:
  PotionRFire(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionRFire() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Fire Resistance";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Protects the consumer from fire."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionAntidote: public Potion {
public:
  PotionAntidote(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionAntidote() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Antidote";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Cures poisoning."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionRElec: public Potion {
public:
  PotionRElec(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionRElec() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Insulation";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Protects the consumer from electricity."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionRAcid: public Potion {
public:
  PotionRAcid(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionRAcid() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Acid Resistance";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"Protects the consumer from acid."};
  }

  void collide_(const Pos& pos, Actor* const actor) override;
};

class PotionDescent: public Potion {
public:
  PotionDescent(ItemDataT* const itemData) :
    Potion(itemData) {}
  ~PotionDescent() {}
  void quaff_(Actor* const actor) override;
private:
  const std::string getRealTypeName() override {return "Descent";}

  std::vector<std::string> getDescrIdentified() const override {
    return {"A bizarre liquid that causes the consumer to dematerialize and sink "
            "through the ground."
           };
  }

  void collide_(const Pos& pos, Actor* const actor) override {
    (void)pos;
    (void)actor;
  }
};

struct PotionLook {
  std::string namePlain;
  std::string nameA;
  Clr clr;
};

namespace PotionNameHandling {

void init();

void storeToSaveLines(std::vector<std::string>& lines);
void setupFromSaveLines(std::vector<std::string>& lines);

void setClrAndFalseName(ItemDataT& d);

} //PotionNameHandling

#endif
