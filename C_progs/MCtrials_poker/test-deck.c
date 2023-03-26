#include <stdio.h>
#include <stdlib.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"

int main (void) {

  deck_t * deck = malloc(sizeof(*deck));
  deck->cards = malloc(3*sizeof(*(deck->cards)));
  deck->n_cards = 3;
  for (int i = 0 ; i < deck->n_cards; i++) {
    deck->cards[i]=malloc(sizeof(*(deck->cards[i])));
  }

  deck->cards[0]->value = 3;
  deck->cards[0]->suit = CLUBS;
  deck->cards[1]->value = 7;
  deck->cards[1]->suit = HEARTS;
  deck->cards[2]->value = 10;
  deck->cards[2]->suit = SPADES;
  card_t card = { .value = 13, .suit = DIAMONDS };
  add_card_to(deck, card);

  add_empty_card(deck);

  deck_t * edeck = make_deck_exclude(deck);

  edeck = build_remaining_deck(&deck, 1);

  free_deck(deck);
  free_deck(edeck);
  
  return EXIT_SUCCESS;
}
