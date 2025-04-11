#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "convertutils.c"

int main(int argc, char *argv[]){
  msubgraph S;
  int Scount=0;
  int i,submult;
  FILE *subfp,*multfp;
  
  
  if(argc!=3){
    printf("displaytopolist.e subtopofilename multfilename\n");
    exit(0);
  }
  
  
  /* open topolist file */
  printf("opening %s\n\n",argv[1]);
  if ((subfp = fopen(argv[1], "r"))==NULL){
    printf("Cannot open file %s for input\n",argv[1]);
    exit(1);
  }

  /* open mult file */
  printf("opening %s\n\n",argv[2]);
  if ((multfp = fopen(argv[2], "r"))==NULL){
    printf("Cannot open file %s for input\n",argv[2]);
    exit(1);
  }

  
  while(fread(&submult,sizeof(int),1,multfp)){
    printf("submult = %i\n",submult);
    for(i=0;i<submult;i++){
      Scount++;
      if(fread_msubgraph(&S,subfp)==0)
	printf("warning, out of subgraphs, Scount = %i\n",Scount);
      display_msubgraph(S);
    }
  }
  printf("Scount = %i\n",Scount);

  fclose(subfp);
  fclose(multfp);

  
  return 0;
}
