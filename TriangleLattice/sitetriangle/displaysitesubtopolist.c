#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "sitesquareutils.c"


int main(int argc, char *argv[]){
  subtoposmall S;
  int Nbond,subtoponum=-1;
  int Scount=0;
  char filename[100];
  FILE *infp;

  
  if((argc!=2)&&(argc!=3)){
    printf("displaysubtopolist.e Nbond [subtoponum]\n");
    exit(0);
  }
  
  Nbond=atoi(argv[1]);
  if(argc==3)
    subtoponum=atoi(argv[2]);
  
  
  /* open subtopolist file */
  sprintf(filename,"graphs/sitegraph_subtopolist%i.dat",Nbond);
  printf("opening %s\n\n",filename);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }


  while(fread(&S,sizeof(subtoposmall),1,infp)){
    if((subtoponum==-1)||(subtoponum==S.parenttopo)){
      displaysubtopobig(subtoposmalltobig(S));
      Scount++;
    }
  }
  printf("Scount = %i\n",Scount);

  fclose(infp);

  
  return 0;
}
