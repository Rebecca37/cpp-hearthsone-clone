#ifndef MINION_H
#define MINION_H

#include "abstractMinion.h"

class Minion : public AbstractMinion
{
  public:
  	Minion(std::string name, Board* board);
  	void play(bool testing, int owner, int targetPlayer, int minion = -1, bool actOnRitual = false) override;
  	void useAbility(int activePlayer, int targetPlayer, int minion = -1, bool testing = false) override;
  	void useTriggered(int owner, int playedMinion, bool isOwnerActive, When when) override;
  	std::shared_ptr<AbstractMinion> getMinion(bool forDisplay = false) override;
  	card_template_t displayCard(bool forInspect = false) override;
  	std::vector<card_template_t> inspectMinion() override;
  	std::shared_ptr<AbstractMinion> removeAllEnchantments() override;
};

#endif
