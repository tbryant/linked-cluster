#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int G_Nsite;
#include "graph.c"
#include "bondgraph.c"
#include "topoutils.c"

int main(int argc, char *argv[]){
  topo T;
  int toponum=-1;
  int Tcount=0;
  FILE *infp;

  
  if((argc!=2)&&(argc!=3)){
    printf("displaytopolist.e filename [toponum]\n");
    exit(0);
  }
  
  if(argc==4)
    toponum=atoi(argv[3]);
  
  
  /* open topolist file */
  printf("opening %s\n\n",argv[1]);
  if ((infp = fopen(argv[1], "r"))==NULL){
    printf("Cannot open file %s for intput\n",argv[1]);
    exit(1);
  }


/*   while(fread(&T,sizeof(topo),1,infp)){ */
  while(readtopofromfile(&T,infp)){
    Tcount++;
    if((toponum==-1)||(toponum==T.topo))
      displaytopo(T);
  }
  printf("Tcount = %i\n",Tcount);

  fclose(infp);

  
  return 0;
}
