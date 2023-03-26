#include <stdlib.h>
#include <stdio.h>
#include "future.h"
#include "deck.h"
#include "cards.h"
#include "eval.h"
#include "input.h"

int main (int argc, char ** argv) {

  card_t someCards[13];
  someCards[0].value=5;
  someCards[0].suit=SPADES;
  someCards[1].value=14;
  someCards[1].suit=HEARTS;
  someCards[2].value=9;
  someCards[2].suit=CLUBS;
  someCards[3].value=11;
  someCards[3].suit=SPADES;
  someCards[4].value=2;
  someCards[4].suit=CLUBS;
  someCards[5].value=3;
  someCards[5].suit=CLUBS;
  someCards[6].value=4;
  someCards[6].suit=CLUBS;
  someCards[7].value=5;
  someCards[7].suit=CLUBS;
  someCards[8].value=6;
  someCards[8].suit=CLUBS;
  someCards[9].value=7;
  someCards[9].suit=CLUBS;
  someCards[10].value=8;
  someCards[10].suit=CLUBS;
  someCards[11].value=9;
  someCards[11].suit=CLUBS;
  someCards[12].value=10;
  someCards[12].suit=CLUBS;
  deck_t shufDeck;
  shufDeck.n_cards=13;
  shufDeck.cards = malloc(shufDeck.n_cards*sizeof(*shufDeck.cards));
  shufDeck.cards[0]=someCards+0;
  shufDeck.cards[1]=someCards+1;
  shufDeck.cards[2]=someCards+2;
  shufDeck.cards[3]=someCards+3;
  shufDeck.cards[4]=someCards+4;
  shufDeck.cards[5]=someCards+5;
  shufDeck.cards[6]=someCards+6;
  shufDeck.cards[7]=someCards+7;
  shufDeck.cards[8]=someCards+8;
  shufDeck.cards[9]=someCards+9;
  shufDeck.cards[10]=someCards+10;
  shufDeck.cards[11]=someCards+11;
  shufDeck.cards[12]=someCards+12;

  shuffle(&shufDeck);
  future_cards_t * fc=malloc(sizeof(*fc));
  fc->decks=NULL;
  fc->n_decks = 0;
  
  FILE * f = fopen(argv[1],"r");

  size_t n_hands = 0;

  deck_t ** inDecks;
  print_hand(&shufDeck);
  printf("\n-------------------------------\n");
  
  inDecks=read_input(f,&n_hands, fc);

  for (int i =0; i< n_hands; i++) {
    print_hand(inDecks[i]);
    printf("\n");
  }
  printf("\n------------------------------\n");



  future_cards_from_deck(&shufDeck ,fc);
  for (int i =0; i< fc->n_decks; i++) {
    print_hand(fc->decks+i);
    printf("\n");
  }
  printf("\n------------------------------\n");

 
  for (int i =0; i< n_hands; i++) {
    print_hand(inDecks[i]);
    printf("\n");
    free_deck(inDecks[i]);
  }
  printf("\n------------------------------\n");
  free(shufDeck.cards);
  free(fc);
  return EXIT_SUCCESS;
}
