#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHA_SIZE 26

int maxnum (int * p) {

  int temp = 0;
  for (int i=0; i<ALPHA_SIZE; i++) {
    if (p[temp] < p[i]) {
      temp = i;
    }
  }
  return temp;
}

int calckey (int a) {

  int b = a-4;
  if (b < 0) {
    return ALPHA_SIZE + b;
  }
  return b;
}


void decrypt(FILE * f) {    
  int c;
  int alph[ALPHA_SIZE] = {0};
    
  while ((c = fgetc(f)) != EOF) {        
    if (isalpha(c)) {
      c = tolower(c);
      switch (c) {
      case 97:
	alph[0]+=1;
	break;
      case 98:
	alph[1]+=1;
	break;
      case 99:
	alph[2]+=1;
	break;
      case 100:
	alph[3]+=1;
	break;
      case 101:
	alph[4]+=1;
	break;
      case 102:
	alph[5]+=1;
	break;
      case 103:
	alph[6]+=1;
	break;
      case 104:
	alph[7]+=1;
	break;
      case 105:
	alph[8]+=1;
	break;
      case 106:
	alph[9]+=1;
	break;
      case 107:
	alph[10]+=1;
	break;
      case 108:
	alph[11]+=1;
	break;
      case 109:
	alph[12]+=1;
	break;
      case 110:
	alph[13]+=1;
	break;
      case 111:
	alph[14]+=1;
	break;
      case 112:
	alph[15]+=1;
	break;
      case 113:
	alph[16]+=1;
	break;
      case 114:
	alph[17]+=1;
	break;
      case 115:
	alph[18]+=1;
	break;
      case 116:
	alph[19]+=1;
	break;
      case 117:
	alph[20]+=1;
	break;
      case 118:
	alph[21]+=1;
	break;
      case 119:
	alph[22]+=1;
	break;
      case 120:
	alph[23]+=1;
	break;
      case 121:
	alph[24]+=1;
	break;
      case 122:
	alph[25]+=1;
	break;	
      }
    }    
  }
  int max_alph = maxnum(alph);
  printf("%d\n", calckey (max_alph));
}


int main(int argc, char ** argv) {  
  if (argc != 2) {    
    fprintf(stderr,"Usage: inputFileName\n");    
    return EXIT_FAILURE;  
  }  
  FILE * f = fopen(argv[1], "r");  
  if (f == NULL) {    
    perror("Could not open file");    
    return EXIT_FAILURE;  
  }  
  decrypt(f);  
  if (fclose(f) != 0) {    
    perror("Failed to close the input file!");    
    return EXIT_FAILURE;  
  }  
  return EXIT_SUCCESS;
}
