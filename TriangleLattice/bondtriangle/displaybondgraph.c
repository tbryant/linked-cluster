#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;
#include "graph.c"
#include "bondgraph.c"

int main(int argc, char *argv[]){
  bondgraphbig B;
  char filename[100];
  FILE *infp;
  int Nsite,Nbond;
  int i=0;
  
  
  if(argc!=3){
    printf("displaybondgraphs.e Nbond Nsite\n");
    exit(0);
  }

  Nbond=atoi(argv[1]);
  Nsite=atoi(argv[2]);
  
  sprintf(filename,"graphs/bondgraphlist%i_%i.dat",Nbond,Nsite);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }
  
  

  printf("opening file %s \n",filename);
  while(readbgbfromfile(&B,Nbond,infp)){
    printf("bondgraph %i:\n",i++);
    displaybgb(B);
  }
  printf("%i graphs\n",i);
  fclose(infp);
  
  return 0;
}
