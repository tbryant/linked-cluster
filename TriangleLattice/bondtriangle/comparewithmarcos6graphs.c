/* compare with marcos */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;

#include "graph.c"
#include "bondgraph.c"

int main(int argc, char *argv[]){
  bondgraphbig M;
  bondgraph S[214];
  char filename[100];
  FILE *infp,*mfp;
  int i=0;
  int latconst,mbond[6];
  int j,k,match;
  
  
  sprintf(filename,"graphs/bondgraphlist%i.dat",6);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }

  sprintf(filename,"graphs/marcos_GRAPHS_06_BONDS.dat");
  if ((mfp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }
  
  fread(S,sizeof(bondgraph),214,infp);

  for(j=0;j<222;j++){
    fscanf(mfp,"%i %i %i %i %i %i %i\n",&latconst,&(mbond[0]),&(mbond[1]),&(mbond[2]),&(mbond[3]),&(mbond[4]),&(mbond[5]));
    /*     printf("latconst = %i",latconst); */
    /*     for(i=0;i<6;i++){ */
    /*       printf(" %4i",mbond[i]); */
    /*     } */
    /*     printf("\n"); */
    M.Nbond=6;
    M.latconst=latconst;
    M.bonddir=0;
    for(i=0;i<6;i++){
      if(mbond[i]%2==1){
	/* horizontal bond */	
	M.bond_x[i]=((mbond[i]%100)-1)/2;
	M.bond_y[i]=((mbond[i]/100)+1)/2;
      }
      else{
	/* vertical bond */
	M.bonddir+=1<<i;
	M.bond_x[i]=(mbond[i]%100)/2;
	M.bond_y[i]=(mbond[i]/100)/2;
      }
    }
/*     displaybgb(M); */
    order(&M);
    translate(&M);
    rotate(&M);
    
    match=0;
    k=0;
    while((match==0)&&(k<214)){
      if(comparebgb(smalltobgb(S[k++]),M)==0){
	match=1;
      }
    }
    if(match==0){
      /* no match */
      printf("no match\n");
      displaybgb(M);
    }
  }

  fclose(infp);
  fclose(mfp);
  
  return 0;
}
