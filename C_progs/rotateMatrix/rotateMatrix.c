#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define N 10

void rotate (char matrix [N][N]) {
  char temp [N][N];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      temp[j][9-i] = matrix[i][j];
      //printf("temp[%d][%d]=%c, matrix[%d][%d]=%c\n", j, 9-i, temp[j][9-i],i,j,matrix[i][j]);
    }
    //    printf("\n\n");
  }
  //  printf("---------------------------------------");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = temp[i][j];
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
  
}


void setMatrix (FILE * f) {

  char inputMatrix [N][N];
  for (size_t i =0; i < N; i++) {
    for (size_t j=0; j < N; j++) {
      inputMatrix[i][j]='\n';
      // printf("%d", inputMatrix[i][j]);
    }
    //printf("\n");
  }

  char c[11];
  //  int count = 0;
  //  char * ptr;
  size_t i=0;
  size_t j=0;
  char * ptr=NULL;
  
  while (fgets(c,12,f)!=NULL) {
    if(i>N-1){
      fprintf(stderr,"Too much rows\n");
      exit(EXIT_FAILURE);
    }
    ptr=strchr(c,'\n');
    if(ptr==NULL && *(c+10)!=EOF) {
      fprintf(stderr,"Row:%zu is too long\n",i+1);
      exit(EXIT_FAILURE);
    }
    if(ptr!=c+10 && ptr!=c && ptr!=NULL) {
      fprintf(stderr,"Row:%zu is too short\n",i+1);
      exit(EXIT_FAILURE);
    }
    for (;j<N;j++) {
      inputMatrix[i][j]=*(c+j);
    }
    i++;
    j=0;
    
  }
  if (i<N) {
    fprintf(stderr,"Not enough rows\n");
    exit(EXIT_FAILURE);
  }
  rotate(inputMatrix);
}



int main (int argc, char ** argv) {

  if(argc!=2) {
    fprintf(stderr, "Usage: InputFilename\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file: ");
    return EXIT_FAILURE;
  }

  setMatrix(f);

  if (fclose(f) != 0) {
    perror("Could not close file: ");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
	   
}
