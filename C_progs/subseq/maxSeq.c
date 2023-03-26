
#include <stdio.h>
#include <stdlib.h>
#include "maxSeq.h"

size_t maxSeq(int * array, size_t n) {
  //  int fistseqnum = array[0];
  if ( n == 0 || array == NULL)
   {
     return (size_t) 0;
   }
  size_t seqlen = 1;
  size_t seqlenmax = 1;
  for (size_t i = 1 ; i < n ; i++) {
    if (array[i] > array[i-1]) {
      seqlen ++;
    }
    else {
      if (seqlen > seqlenmax) {
	seqlenmax = seqlen;
      }
      seqlen = 1;
    }
  }
  if (seqlen > seqlenmax) {
    seqlenmax = seqlen;
  }
  return seqlenmax;
}
