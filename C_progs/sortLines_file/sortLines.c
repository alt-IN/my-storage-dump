#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}

void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void freeMem(char ** ptr, size_t num) {

  if (num == 0) {
    free(*ptr);
  }
  if (num > 0) {
    for (size_t i = 0; i < num; i++) {
      free(ptr[i]);
    }
    free(ptr);
  }
}

char ** readFromFiles(char * file, size_t * linecount) {
  
  char ** lines = NULL;
  char * curr = NULL;
  size_t sz = 0;
  FILE * f;
  
  f = fopen(file, "r");
  if (f==NULL) {
    perror("Error reading file\n");
    exit(EXIT_FAILURE);
  }
  while (getline(&curr, &sz, f) >= 0) {
    lines = realloc (lines, (*linecount+1)*sizeof(*lines));
    lines[*linecount] = curr;
    curr = NULL;
    *linecount=*linecount+1;    
  }
  if(fclose(f) != 0) {
    perror("Error close file\n");
    freeMem(&curr,0);
    freeMem(lines,*linecount);
    exit(EXIT_FAILURE);
  }
  
  
  freeMem(&curr,0);
  return lines;
}

char ** readFromStdIn (size_t * linecount) {

  char ** lines = NULL;
  char * curr = NULL;
  size_t sz;
  
  
  while (getline(&curr, &sz, stdin) >= 0) {
    lines = realloc (lines, (*linecount+1)*sizeof(*lines));
    lines[*linecount] = curr;
    curr = NULL;
    *linecount=*linecount+1;    
  }
  freeMem(&curr,0);
  return lines;
}

void printSortText (char ** text, size_t linecount) {

  for (size_t i = 0; i< linecount; i++) {
    fprintf(stdout,"%s",text[i]);
  }
}



int main(int argc, char ** argv) {
  
  char ** text;
    
  if(argc==1) {
    size_t cl = 0;
    if((text = readFromStdIn(&cl))!=NULL) {
      sortData(text,cl);
      printSortText(text,cl);
      freeMem(text,cl);
    }
  }
  if(argc >= 2) {
    for(int i = 1 ; i < argc; i++) {
      size_t cl = 0;
      if((text = readFromFiles(argv[i],&cl))!=NULL) {
	  sortData(text,cl);
	  printSortText(text,cl);
	  freeMem(text,cl);
      }
    }
  }
  return EXIT_SUCCESS;
}
