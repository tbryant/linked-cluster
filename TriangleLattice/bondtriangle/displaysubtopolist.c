#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int G_Nsite;
#include "graph.c"
#include "bondgraph.c"
#include "topoutils.c"

int main(int argc, char *argv[]){
  subtopo S;
  int Nbond,Nsite,subtoponum=-1;
  int Scount=0;
  char filename[100];
  FILE *infp;

  
  if((argc!=3)&&(argc!=4)){
    printf("displaysubtopolist.e Nbond Nsite [subtoponum]\n");
    exit(0);
  }
  
  Nbond=atoi(argv[1]);
  Nsite=atoi(argv[2]);

  if(argc==4)
    subtoponum=atoi(argv[3]);
  
  
  /* open subtopolist file */
  sprintf(filename,"graphs/bondgraph_subtopolist%i_%i.dat",Nbond,Nsite);
  printf("opening %s\n\n",filename);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }


  while(readsubtopofromfile(&S,infp)){
    if((subtoponum==-1)||(subtoponum==S.parenttopo)){
      displaysubtopo(S);
      Scount++;
    }
  }
  printf("Scount = %i\n",Scount);

  fclose(infp);

  
  return 0;
}
