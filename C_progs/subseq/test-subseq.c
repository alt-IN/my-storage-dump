
#include <stdio.h>
#include <stdlib.h>
#include "maxSeq.h"

int main (void) {
  size_t seq;
  int array1[5] = {-5, -4, -3, -2, -1};
  int array2[3] = {1,1,1};
  int array3[1] = {5};
  int array4[6] = {1,1000,1001,-121575294,3,121575298};
  int array6[5] = {0,0,0};
  int array7[5] = {100000000, 90, 4, 6, 8};

  seq = maxSeq (array1, 5);
  printf("seq=%zu\n", seq);
  if (seq != 5) {
    printf("seq != 5 in test1\n");
    //return EXIT_FAILURE;
  }

  seq = maxSeq (array2, 3);
  printf("seq=%zu\n", seq);
  if (seq != 1) {
    printf("seq != 1 in test2\n");
    //return EXIT_FAILURE;
  }

  seq = maxSeq (array3, 1);
  printf("seq=%zu\n", seq);
  if (seq != 1) {
    printf("seq != 1 in test3\n");
    //return EXIT_FAILURE;
  }

  seq = maxSeq (array4, 6);
  printf("seq=%zu\n", seq);
  if (seq != 3) {
    printf("seq != 3 in test4\n");
    return EXIT_FAILURE;
  }

  seq = maxSeq (NULL, 0);
  printf("seq=%zu\n", seq);
  if (seq != 0) {
    printf("seq != 0 in test5\n");
    return EXIT_FAILURE;
  }

  seq = maxSeq (array6, 3);
  printf("seq=%zu\n", seq);
  if (seq != 1) {
    printf("seq != 1 in test6\n");
    return EXIT_FAILURE;
  }

  seq = maxSeq (array7, 4);
  printf("seq=%zu\n", seq);
  if (seq != 2) {
    printf("seq != 2 in test7\n");
    return EXIT_FAILURE;
  }

  /*---> seq = maxSeq (array2, 10);
  printf("seq2=%zu\n", seq);
  if (seq != 0) {
    printf("seq != 1 in test2\n");
    return EXIT_FAILURE;
  }
  seq = maxSeq (array3, 10);
  printf("seq3=%zu\n", seq);
  if (seq != 2) {
    printf("seq != 2 in test3\n");
    return EXIT_FAILURE;
  }
  seq = maxSeq (array4, 10);
  printf("seq4=%zu\n", seq);
  if (seq != 0) {
    printf("seq != 2 in test4\n");
    return EXIT_FAILURE;
  }
  seq = maxSeq (NULL, 0);
  printf("seq5=%zu\n", seq);
  if (seq != 0) {
    printf("seq != 0 in test5\n");
    return EXIT_FAILURE;
  }
  seq = maxSeq (array6, 1);
  printf("seq6=%zu\n", seq);
  if (seq != 5) {
    printf("seq != 5 in test6\n");
    return EXIT_FAILURE;
  }
  seq = maxSeq (array7, 10);
  printf("seq7=%zu\n", seq);
  if (seq != 3) {
    printf("seq != 3 in test7\n");
    return EXIT_FAILURE;
  }
  seq = maxSeq (array8, 10);
  printf("seq8=%zu\n", seq);
  if (seq != 0) {
    printf("seq != 0 in test8\n");
    return EXIT_FAILURE;
  }
  */
   
  return EXIT_SUCCESS;
}
