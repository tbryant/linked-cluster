#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "sitesquareutils.c"

int main(int argc, char *argv[]){
  topo *T;
  int *index;
  int i;
  int Nsite;
  int Tcount=0;
  char filename[100];
  FILE *infp,*outfp;

  
  if(argc!=2){
    printf("sorttopolist.e Nsite\n");
    exit(0);
  }
  
  Nsite=atoi(argv[1]);
  
  
  /* open topolist infile */
  sprintf(filename,"graphs/sitegraph_unsortedtopolist%i.dat",Nsite);
  printf("opening %s\n\n",filename);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }

  /* open topolist file */
  sprintf(filename,"graphs/sitegraph_topolist%i.dat",Nsite);
  printf("opening %s\n\n",filename);
  if ((outfp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  fread(&Tcount,sizeof(int),1,infp);
  printf("Tcount = %i\n",Tcount);

  fwrite(&Tcount,sizeof(int),1,outfp);

  if((T=(topo *)malloc(sizeof(topo)*Tcount))==NULL){
    printf("out of memory\n");
    exit(0);
  }

  if((index=(int *)malloc(sizeof(int)*Tcount))==NULL){
    printf("out of memory\n");
    exit(0);
  }
  
  fread(T,sizeof(topo),Tcount,infp);
  
  for(i=0;i<Tcount;i++){
    index[T[i].topo]=i;
  }

  /* write them out in order (they might be mixed up if they shared a key) */
  for(i=0;i<Tcount;i++){
    
    fwrite(&(T[index[i]]),sizeof(topo),1,outfp);
  }
  free(T);
  free(index);

  fclose(infp);
  fclose(outfp);
  
  return 0;
}
