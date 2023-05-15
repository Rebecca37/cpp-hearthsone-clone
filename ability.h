#ifndef ABILITY_H
#define ABILITY_H

#include "enchantment.h"

class Ability : public Enchantment
{
  public:
  	Ability(std::string name, Board* board, std::shared_ptr<AbstractMinion> minion);
  	void play(bool testing, int owner, int targetPlayer, int minion = -1, bool actOnRitual = false) override;	
  	void useAbility(int activePlayer, int targetPlayer, int minion = -1, bool testing = false) override; 	
  	void useTriggered(int owner, int playedMinion, bool isOwnerActive, When when) override;
  	std::shared_ptr<AbstractMinion> getMinion(bool forDisplay = false) override;
};

#endif
