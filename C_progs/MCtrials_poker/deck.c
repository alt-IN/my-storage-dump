#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"

void print_hand(deck_t * hand){
  for (size_t i = 0 ; i < hand -> n_cards ; i++) {
    print_card(*(hand->cards[i]));
  }
}

int deck_contains(deck_t * d, card_t c) {
  for (size_t i = 0; i < d->n_cards; i++) {
    if (d->cards[i]->suit == c.suit && d->cards[i]->value == c.value) {
      return 1;
    }
  }
  return 0;
}

void shuffle(deck_t * d){
  int order;
  card_t * temp;

  for (size_t i = 0; i < (d -> n_cards)/2; i++) {
    order = rand() % (d -> n_cards);
    temp = d->cards[order];
    d->cards[order] = d->cards[i];
    d->cards[i] = temp;
  }
}

void assert_full_deck(deck_t * d) {
  deck_t h;
  card_t curr;
  for (size_t i = 0; i < d->n_cards-1; i++) {
    assert_card_valid (*(d->cards[i]));
    curr=*(d->cards[i]);
    h.cards = d->cards+i+1;
    h.n_cards = d->n_cards-i-1;
    assert((deck_contains (&h,curr) != 1 && "More than one card"));
  }
}

void add_card_to(deck_t * deck, card_t c) {
  deck->cards = realloc(deck->cards,(deck->n_cards+1)*sizeof(*(deck->cards)));
  deck->cards[deck->n_cards]=malloc(sizeof(*(deck->cards[deck->n_cards])));
  deck->cards[deck->n_cards]->value = c.value;
  deck->cards[deck->n_cards]->suit = c.suit;
  deck->n_cards++;
}
 
card_t * add_empty_card(deck_t * deck) {
  deck->cards = realloc(deck->cards,(deck->n_cards+1)*sizeof(*(deck->cards)));
  deck->cards[deck->n_cards]=malloc(sizeof(*(deck->cards[deck->n_cards])));
  deck->cards[deck->n_cards]->value = 0;
  deck->cards[deck->n_cards]->suit = 0;
  deck->n_cards++;
  return deck->cards[deck->n_cards-1
		     ];
}

deck_t * make_deck_exclude(deck_t * excluded_cards) {
  deck_t * excDeck = malloc(sizeof(*excDeck));
  excDeck->n_cards = 0;
  excDeck->cards=NULL;
  card_t temp;
  for(size_t i = 0; i < 52; i++) {
    temp = card_from_num(i);
    if(deck_contains(excluded_cards,temp) == 0) {
      excDeck->cards = realloc(excDeck->cards, (excDeck->n_cards+1)*sizeof(*(excDeck->cards)));
      excDeck->cards[excDeck->n_cards]=malloc(sizeof(*(excDeck->cards[excDeck->n_cards])));
      *(excDeck->cards[excDeck->
		       n_cards])=temp;
      excDeck->n_cards++;
    }
  }
  return excDeck;
}

deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands) {
  
  deck_t * knownDeck = malloc(sizeof(*knownDeck));
  knownDeck->cards=NULL;
  knownDeck->n_cards=0;

  for(int i = 0; i < n_hands; i++) {
    for(int j = 0; j < hands[i]->n_cards; j++) {
      add_card_to(knownDeck,*(hands[i]->cards[j]));
    }
  }
  
  deck_t * leftDeck = make_deck_exclude(knownDeck);
  free_deck(knownDeck);
  
  return leftDeck;
}

void free_deck(deck_t * deck) {
  for (size_t i = 0 ; i < deck->n_cards; i++) {
    free(deck->cards[i]);
  }
  free(deck->cards);
  free(deck);
}
