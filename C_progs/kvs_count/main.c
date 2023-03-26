#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  FILE * cFile = fopen(filename,"r");
  if (cFile == NULL) {
    fprintf(stderr, "Input read error\n");
    exit(EXIT_FAILURE);
  }
   
  counts_t * fc = createCounts();
  char * line = NULL;
  size_t lz = 0;
  while(getline(&line,&lz,cFile)>=0) {
    char * ptr = NULL;
    if ((ptr=strchr(line,'\n')) != NULL) {
      *ptr='\0';
    }
    addCount(fc,lookupValue(kvPairs,line));
    free(line);
    line = NULL;
   
  }
  free(line);
  if(fclose(cFile)!=0) {
    fprintf(stderr,"Cannot close input file");
    exit(EXIT_FAILURE);
  }
  
  return fc;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  if(argc < 3) {
    fprintf(stderr,"Incorrect usage\n");
    return EXIT_FAILURE;
  }
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * kv = readKVs(argv[1]);
 //count from 2 to argc (call the number you count i)
  for (int i = 2; i < argc; i++) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t * c = countFile(argv[i], kv);
  
    //compute the output file name from argv[i] (call this outName)
    char * outName = malloc((strlen(argv[i])+8)*sizeof(*outName));
    strcpy(outName,argv[i]);
    strcat(outName,".counts");

    //open the file named by outName (call that f)
    FILE * f = fopen(outName,"w");

    if(f == NULL) {
      fprintf(stderr,"Output error");
      return EXIT_FAILURE;
    }

    //print the counts from c into the FILE f
    printCounts(c,f);
    
    //close f
    if(fclose(f) != 0) {
      fprintf(stderr,"Error in closing output file");
      return EXIT_FAILURE;
    }
    
    //free the memory for outName and c
    free(outName);
    freeCounts(c);
  }

 //free the memory for kv
  freeKVs(kv);
  
  return EXIT_SUCCESS;
}
