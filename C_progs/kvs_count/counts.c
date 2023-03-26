#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"

counts_t * createCounts(void) {
  counts_t * ARcounts = malloc(sizeof(*ARcounts));
  ARcounts->countAr=NULL;
  ARcounts->sizeAr=0;
  ARcounts->unknownNum=0;
  return ARcounts;
}

void addCount(counts_t * c, const char * name) {
  int flag = 0;
  if (name != NULL) {
    for(int i = 0 ; i < c->sizeAr; i++) {
      if(strcmp(c->countAr[i]->value, name)==0) {
	c->countAr[i]->count++;
	flag++;
      }
    }
    if(flag==0 && name != NULL) {
      c->countAr = realloc(c->countAr,(c->sizeAr + 1)*sizeof(*(c->countAr)));
      c->countAr[c->sizeAr] = malloc(sizeof(*(c->countAr[c->sizeAr])));
      c->countAr[c->sizeAr]->value = malloc((strlen(name)+1)*sizeof(*(c->countAr[c->sizeAr]->value)));
      strcpy(c->countAr[c->sizeAr]->value,name);
      c->countAr[c->sizeAr]->count=1;
      c->sizeAr++;
    }
      
  } 
  if(name == NULL) {
    c->unknownNum++;
  }
}
  

void printCounts(counts_t * c, FILE * outFile) {
  for(int i = 0; i < c->sizeAr; i++) {
    fprintf(outFile,"%s: %d\n",c->countAr[i]->value,c->countAr[i]->count);
  }
  if(c->unknownNum > 0) {
    fprintf(outFile,"<unknown> : %d\n", c->unknownNum);
  }
}

void freeCounts(counts_t * c) {
  for (int i = 0; i < c->sizeAr; i++) {
    free (c->countAr[i]->value);
    free (c->countAr[i]);
  }
  free(c->countAr);
  free(c);
}
