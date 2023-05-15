#ifndef ABSTRACT_MINION_H
#define ABSTRACT_MINION_H

#include "card.h"

class AbstractMinion : public Card
{
	protected:
		int attack;
		int defence;
		int actions;
		bool gainAction = true;
		int activationCost = 0;
    std::string abilityDescription = "";

  public:
  	AbstractMinion(std::string name, Board* board);
  	void attackPlayer(int player);
  	void attackMinion(int ownPosition, int owner, int player, int target);
    // decrease defence by d
  	void damage(int d);
  	int getDefence() const;
  	int getAttack() const;
    void setDefence(int d);
    void setAttack(int a);
  	int getAction() const;
    // return true if the minion will gain actions at the start of the turn
  	bool gaining() const;
    void setGaining(bool gain);
  	int getActivationCost() const;
    void setActivationCost(int c);
  	void setAction(int owner, int ownPosition);
    void setCurrentAction(int a);
  	// remember, active player as param. Use minions activated ability
    virtual void useAbility(int activePlayer, int targetPlayer, int minion = -1, bool testing = false) = 0;
    // activate trigger
  	virtual void useTriggered(int owner, int playedMinion, bool isOwnerActive, When when) = 0;
    virtual std::shared_ptr<AbstractMinion> getMinion(bool forDisplay = false) = 0;
    virtual std::vector<card_template_t> inspectMinion() = 0;
    virtual std::shared_ptr<AbstractMinion> removeAllEnchantments() = 0;
};

#endif
