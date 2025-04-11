#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "sitesquareutils.c"

int main(int argc, char *argv[]){
  topo T;
  int Nbond,toponum=-1;
  int Tcount=0;
  char filename[100];
  FILE *infp;

  
  if((argc!=2)&&(argc!=3)){
    printf("displaytopolist.e Nbond [toponum]\n");
    exit(0);
  }
  
  Nbond=atoi(argv[1]);
  if(argc==3)
    toponum=atoi(argv[2]);
  
  
  /* open topolist file */
  sprintf(filename,"graphs/sitegraph_topolist%i.dat",Nbond);
  printf("opening %s\n\n",filename);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }

  fread(&Tcount,sizeof(int),1,infp);
  printf("Tcount = %i\n",Tcount);

  while(fread(&T,sizeof(topo),1,infp)){
    if((toponum==-1)||(toponum==T.topo)){
      displaytopo(T);
    }
  }

  fclose(infp);

  
  return 0;
}
