#include "magicFatigue.h"

MagicFatigue::MagicFatigue(std::string name, Board* board) : Enchantment{name, board} {
	enchantmentName = "Magic Fatigue";
	enchantmentDescription = "Enchanted minion's activated ability costs 2 more";
	cost = 0;
}

void MagicFatigue::play(bool testing, int owner, int targetPlayer, int minion, bool actOnRitual) {
	//enchanted minion's activated ability costs 2 more
	Player* p = board->getPlayer(owner);
	Player* t = board->getPlayer(targetPlayer);
    int mana = p->getMana();
	if (cost > mana && testing == false) throw InputException{"Player doesn't have enough mana"};
	int lastMinion = t->getNumMinions()-1;
	if (minion < 0) throw InputException{"Please specify a target to play this card"};
	if (minion > lastMinion) throw InputException{"Target doesn't have minion at " + std::to_string(minion+1)};
	auto m = std::make_shared<MagicFatigue>(name,board);
	m->minion = t->getMinion(minion);
	m->actions = m->minion->getAction();
	m->attack = m->minion->getAttack();
	m->defence = m->minion->getDefence();
	m->gainAction = m->minion->gaining();
	m->name = m->minion->getName();
	m->activationCost = m->minion->getActivationCost()+2;
	t->replaceMinion(minion,m);
	if (cost > mana) p->setMana(0);
	else p->setMana(mana-cost);
}
void MagicFatigue::useAbility(int activePlayer, int targetPlayer, int minion, bool testing) { this->minion->useAbility(activePlayer,targetPlayer,minion,testing); }
void MagicFatigue::useTriggered(int owner, int playedMinion, bool isOwnerActive, When when) { minion->useTriggered(owner,playedMinion,isOwnerActive,when); }

std::shared_ptr<AbstractMinion> MagicFatigue::getMinion(bool forDisplay) {
	if (minion == nullptr) throw InputException{"Unexpected! Enchantment not attached to a minion"};
	minion->setDefence(defence);
	minion->setAttack(attack);
	minion->setCurrentAction(actions);
	if (forDisplay) {
		minion->setActivationCost(activationCost);
	} else {
		minion->setActivationCost(activationCost-2);
	}
	minion->setGaining(gainAction);
	return minion;
}
