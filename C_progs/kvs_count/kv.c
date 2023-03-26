#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"

kvpair_t * makeKV(char * text) {
  kvpair_t * kv = malloc(sizeof(*kv));
  kv->key=NULL;
  kv->value=NULL;
  char * ptr = strchr(text, '=');
  char * ptr2 = strchr(text, '\n');
  if (ptr2!=NULL) {
    *ptr2='\0';
  }

  if(ptr==NULL) {
    fprintf(stderr,"Incorrect input file format\n");
    exit(EXIT_FAILURE);
  }

  *ptr='\0';
  kv->key=malloc((ptr-text+1)*(sizeof(*(kv->key))));
  strcpy(kv->key,text);
  ptr++;
     
  kv->value=malloc((strlen(ptr)+1)*sizeof(*(kv->value)));
  strcpy(kv->value,ptr);
  
  return kv;
}

kvarray_t * readKVs(const char * fname) {
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open file for reading\n");
    exit(EXIT_FAILURE);
  }
  char * line = NULL;
  size_t lsz = 0;
  kvarray_t * pairs = malloc(sizeof(*pairs));
  pairs->pairNum = 0;
  pairs->pairAr = NULL;
  
  while(getline(&line, &lsz, f)>=0) {
    
    pairs->pairAr = realloc(pairs->pairAr, (pairs->pairNum+1)*sizeof(*(pairs->pairAr)));
    pairs->pairAr[pairs->pairNum] = makeKV(line);
    pairs->pairNum++;
    free(line);
    line = NULL;
    
  }
  free(line);
  if (fclose(f)!=0) {
    fprintf(stderr, "Cannot close file\n");
    freeKVs(pairs);
    exit(EXIT_FAILURE);
  }

  return pairs;
}

void freeKVs(kvarray_t * pairs) {
  for (int i = 0; i < pairs->pairNum; i++) {
    free(pairs->pairAr[i]->key);
    free(pairs->pairAr[i]->value);
    free(pairs->pairAr[i]);
  }
  free(pairs->pairAr);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  for (int i = 0; i < pairs->pairNum; i++) {
    printf("key = '%s' value = '%s'\n", pairs->pairAr[i]->key,pairs->pairAr[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  int i = 0;
  
  while (i < pairs->pairNum) {
    if (strcmp(pairs->pairAr[i]->key, key) == 0) {
      return pairs->pairAr[i]->value;
    }
    i++;
  }
  return NULL;

  
}
