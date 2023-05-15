#include "player.h"
#include "card.h"
#include "abstractMinion.h"
#include "ritual.h"

#include <iostream>

Player::Player(int playerNum, std::string name, std::vector<std::shared_ptr<Card>> deck)
  : playerNum{playerNum}
  , name{name}
  , life{20}
  , mana{3}
  , deck{deck}
  {
    while (hand.size() < 5) {
      auto deckTopCard = this->deck.at(this->deck.size()-1);
      hand.push_back(deckTopCard);
      this->deck.pop_back();
    }
  }

// The player gains 1 mana.
void Player::startTurn()
{
  mana++;
  draw();
  for (int i = 0; i < (int)minions.size(); ++i) minions.at(i)->setAction(playerNum,i);
}

// orders minion i to attack the opposing player o
void Player::attack(int i, int o)
{
  minions.at(i)->attackPlayer(o);
}

// orders the active player’s minion i to attack the opposing player o’s minion t.
void Player::attack(int i, int o, int t)
{
  minions.at(i)->attackMinion(i, playerNum, o, t);
}

// plays the ith card in the active player’s hand with no target.
void Player::play(int i, bool testing)
{
  hand.at(i)->play(testing, playerNum, playerNum);
  hand.erase(hand.begin()+i);
}

// plays the ith card in the active player’s hand on card t owned by player p. 
// p may be equal to 1 or 2 to represent player 1 or 2 respectively.
// t is either 1, 2, 3, 4, 5 (the ith minion owned by player p) or
// r (the ritual owned by player p). 
// This can be used to play enchantments and spells with targets.
void Player::play(int i, int p, char t, bool testing)
{
  if (t == 'r') {
    hand.at(i)->play(testing, playerNum, p, -1, true); // true = attack on ritual
    return;
  }
  int m = t - '0';
  hand.at(i)->play(testing, playerNum, p, m-1);
  hand.erase(hand.begin()+i);
}

// i refers to the ith minion owned by the current player
// and the command orders that minion to use its activated ability on the provided target (or on no target).
// If no target = the current player itself
void Player::use(int i, bool testing)
{
  minions.at(i)->useAbility(playerNum, playerNum, -1, testing);
}

void Player::use(int i, int p, char t, bool testing)
{
  int m = t - '0';
  minions.at(i)->useAbility(playerNum, p, m-1, testing);
}

/////// testing mode only ////////

// draws a card if their deck is non-empty and their hand has less than 5 cards.
void Player::draw() {
  if (deck.size() != 0 && hand.size() < 5) {
  	auto deckTopCard = deck.at(deck.size()-1);
  	hand.push_back(deckTopCard);
  	deck.pop_back();
  }
}

// discards the ith card in the player’s hand, simply removing it from their hand and destroying it.
void Player::discard(int i) {
  hand.erase(hand.begin() + i);
}

int Player::getPlayerNum()
{
  return playerNum;
}

// Any action that would make a minion return to the player’s hand (e.g., Unsummon)
// will instead make the minion vanish from play and be destroyed if the hand is already full.
void Player::moveToHand(int i)
{
  auto minion = minions.at(i);
  minions.erase(minions.begin() + i);
  if (hand.size() != maxHandSize) {
    hand.push_back(minion);
  }
} 

// Resurrect the top minion in your graveyard and set its defence to 1
void Player::resurrect()
{
  auto minion = graveyard.at(graveyard.size()-1);
  graveyard.pop_back();
  minions.push_back(minion);
}

int Player::getLife()
{
  return life;
}

void Player::damage(int d)
{
  life -= d;
} 

int Player::getMana()
{
  return mana;
}

void Player::setMana(int m)
{
  mana = m;
}

bool Player::hasRitual() { return (ritual != nullptr); }

std::shared_ptr<Ritual> Player::getRitual()
{
  return ritual;
}

void Player::setRitual(std::shared_ptr<Ritual> ritual)
{
  this->ritual = ritual;
}

void Player::removeRitual()
{
  ritual = nullptr;
}

std::shared_ptr<AbstractMinion> Player::getMinion(int i)
{
  return minions.at(i);
}

int Player::getNumMinions()
{
  return minions.size();
}

// If all 5 slots are occupied, the minion cannot be played. Minions occupy the leftmost spots on the board, and
// new minions are always added to the right of older minions.
void Player::addMinion(std::shared_ptr<AbstractMinion> m)
{
  minions.push_back(m);
}

void Player::replaceMinion(int i, std::shared_ptr<AbstractMinion> m)
{
  minions.at(i) = m;
}

void Player::removeMinion(int i, bool moveToGrave) {
  if (moveToGrave) {
    auto minion = minions.at(i);
    graveyard.push_back(minion);
  }
  minions.erase(minions.begin() + i);
}

bool Player::isGraveyardEmpty() {
  return graveyard.size() == 0;
}

card_template_t Player::display() {
  return display_player_card(playerNum, name, life, mana);
}

std::vector<card_template_t> Player::displayMinions() {
  std::vector<card_template_t> vec;
  for (auto m : minions) vec.push_back(m->displayCard());
  return vec;
}

std::vector<card_template_t> Player::inspectMinion(int i) {
  std::vector<card_template_t> vec;
  if (minions.size() == 0) return vec;
  return minions.at(i)->inspectMinion();
}

std::vector<card_template_t> Player::displayHand() {
  std::vector<card_template_t> vec;
  for (auto c : hand) vec.push_back(c->displayCard());
  return vec;
}
  
card_template_t Player::showTopGraveyard() {
  card_template_t vec = CARD_TEMPLATE_BORDER;
  if (graveyard.size() == 0) return vec;
  else vec = graveyard.at(graveyard.size()-1)->displayCard();
  return vec;
}

card_template_t Player::showRitual() {
  card_template_t vec = CARD_TEMPLATE_BORDER;
  if (ritual == nullptr) return vec;
  else vec = ritual->displayCard();
  return vec;
}

std::vector<card_template_t> Player::displayDeck() {
  std::vector<card_template_t> vec;
  for (auto c : deck) vec.push_back(c->displayCard());
  return vec;
}
