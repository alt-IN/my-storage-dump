#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"



int main(int argc, char ** argv) {
  //YOUR CODE GOES HERE
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Incorrect usage: input filename [+optional: number of trials\n");
    return EXIT_FAILURE;
  }

  int numTrials = 10000;
  if(argc == 3) {
    int inNumTrails;
    if((inNumTrails=atoi(argv[2]))!=0) {
      numTrials = inNumTrails;
    }
  } 
     
  FILE * f=fopen(argv[1],"r");
  if (f==NULL) {
    fprintf(stderr, "Error: Cannot open file");
    return EXIT_FAILURE;
  }
  
  deck_t ** inputHands;
  deck_t * remainDeck;
  size_t numHands = 0;
  future_cards_t * futCards = malloc(sizeof(*futCards));
  futCards->decks=NULL;
  futCards->n_decks=0;
  
  inputHands=read_input(f, &numHands, futCards);
  remainDeck=build_remaining_deck(inputHands, numHands);

  int * winCount = malloc((numHands+1)*sizeof(*winCount));
  for (size_t i = 0; i<numHands+1;i++) {
    winCount[i]=0;
  }

  int countTrial = numTrials;
  while(countTrial>0) {
    shuffle(remainDeck);
    future_cards_from_deck(remainDeck,futCards);
    deck_t * winHand = inputHands[0];
    deck_t * pwinHand = NULL;
    size_t indWHand = 0;
    int tieRes = 0;
    
    if(numHands>1) {
      for(size_t i = 1; i <numHands; i++) {
	int compRes=compare_hands(winHand,inputHands[i]);
	if(compRes == -1) {
	  pwinHand = winHand;
	  winHand = inputHands[i];
	  indWHand = i;
	}
	if(compRes == 0) {
	  pwinHand = winHand;
	  winHand = inputHands[i];
	}
      }
      if(pwinHand!=NULL && compare_hands(winHand,pwinHand)==0) {
	tieRes = 1;
      }
    }
    if (tieRes == 1) {
      winCount[numHands]=winCount[numHands]+1;
    }
    if (tieRes != 1) {
      winCount[indWHand]=winCount[indWHand]+1;
    }
    
    countTrial--;
  }

  for(size_t i = 0; i < numHands; i++) {
    printf("Hand %zu won %u / %u times (%.2f%%)\n", i, winCount[i], numTrials,winCount[i]*100.0/numTrials);
  }
  printf("And there were %u ties\n",winCount[numHands]);
    
  free_deck(remainDeck);
  for(size_t i = 0; i< numHands; i++) {
    free_deck(inputHands[i]);
  }
  free(inputHands);
  for(size_t i = 0; i<futCards->n_decks; i++) {
    free(futCards->decks[i].cards);
    
  }
  free(futCards->decks);
  free(futCards);
  free(winCount);
  
  if (fclose(f)!=0) {
    fprintf(stderr, "Error: Cannot close file");
    return EXIT_FAILURE;
  }
  
  
  return EXIT_SUCCESS;
}
