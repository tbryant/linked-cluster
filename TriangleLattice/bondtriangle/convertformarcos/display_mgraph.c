#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "convertutils.c"

int main(int argc, char *argv[]){
  mgraph G;
  int tnbonds,Gcount=0;
  int latconst;
  FILE *topofp,*multfp;
  
  
  if(argc!=4){
    printf("displaytopolist.e tnbonds topofilename multfilename\n");
    exit(0);
  }
  
  tnbonds=atoi(argv[1]);
  
  
  /* open topolist file */
  printf("opening %s\n\n",argv[2]);
  if ((topofp = fopen(argv[2], "r"))==NULL){
    printf("Cannot open file %s for input\n",argv[2]);
    exit(1);
  }

  /* open mult file */
  printf("opening %s\n\n",argv[3]);
  if ((multfp = fopen(argv[3], "r"))==NULL){
    printf("Cannot open file %s for input\n",argv[3]);
    exit(1);
  }
  
  while(fread_mgraph(tnbonds,&G,topofp)){
    Gcount++;
    fread(&latconst,sizeof(int),1,multfp);
    printf("mult=%i ",latconst);
    display_mgraph(G);
  }
  printf("Gcount = %i\n",Gcount);

  fclose(topofp);
  fclose(multfp);

  
  return 0;
}
