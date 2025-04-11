#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;
#include "graph.c"

int main(int argc, char *argv[]){
  graph M;
  char filename[100];
  FILE *infp;
  int Mcount,i=0;
  
  
  if(argc!=2){
    printf("displayM.e Nsite\n");
    exit(0);
  }

  G_Nsite=atoi(argv[1]);
  
  sprintf(filename,"graphs/embedlist%i.dat",G_Nsite);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }

  fread(&Mcount,sizeof(int),1,infp);
  printf("Mcount = %i\n",Mcount);

  while(fread(&M,sizeof(graph),1,infp)){
    printf("embedding %i:\n",i++);
    display(M);
  }
  
  fclose(infp);
  
  return 0;
}
