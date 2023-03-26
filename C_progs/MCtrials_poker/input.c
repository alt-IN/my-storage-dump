#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "input.h"
#include "cards.h"
#include "deck.h"


deck_t * hand_from_string(const char * str, future_cards_t * fc) {
  const char * ptr=str;
  int flag = 0;
  const char * ptrSuit;
  while(*ptr == ' ') {
    ptr++;
  }
       
  deck_t * deckHand = malloc(sizeof(*deckHand));
  deckHand->n_cards = 0;
  deckHand->cards=NULL;
    
  do {
    ptrSuit = ptr+flag+1;
    if(*(ptr+flag) == '?') {
      card_t * eCardPtr = add_empty_card(deckHand);
      assert(*ptrSuit>='0' && *ptrSuit<='9');
      size_t inIndex = atoi(ptrSuit);
      add_future_card(fc,inIndex,eCardPtr);
    }

    if(*(ptr+flag) != '?' && *(ptr+flag) != '\n' && *(ptr+flag) != ' ') {
      card_t inpCard = card_from_letters(*(ptr+flag), *ptrSuit);
      add_card_to(deckHand,inpCard);
    }
    flag=1;
    ptr++;
  }
  while ((ptr=strchr(ptr,' '))!=NULL && *ptr!='\0');

    
  /*while(*ptrVal!='\0' && *ptrVal!='\n') {
    if(*ptrVal == '?') {
      card_t * eCardPtr = add_empty_card(deckHand);
      size_t inIndex = atoi(ptrSuit);
      add_future_card(fc,inIndex,eCardPtr);
    }

    if(*ptrVal != '?') {
      card_t inpCard = card_from_letters(*ptrVal, *ptrSuit);
      add_card_to(deckHand,inpCard);
    }
    ptrVal=ptrVal+3;
    ptrSuit=ptrVal+1;
  }
  */
  if(deckHand->n_cards < 5) {
    fprintf(stderr,"Not enough cards for hand or incorrect input\n");
    exit (EXIT_FAILURE);
  }
  
  return deckHand;
}


deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc) {
  deck_t ** inputHands = NULL;
  char * line = NULL;
  size_t sz;
  *n_hands=0;
  while(getline(&line,&sz,f)>=0) {
    if(*line!='\n') {
      inputHands=realloc(inputHands,(*n_hands+1)*sizeof(*inputHands));
      inputHands[*n_hands]=hand_from_string(line, fc);
      *n_hands=*n_hands+1;
    }
    free(line);
    line=NULL;
  }
  free(line);
  /*if (*n_hands > 3) {
    fprintf(stderr,"Too much hands to compare\n");
    exit(EXIT_FAILURE);
  }
  */
  return inputHands;
}
