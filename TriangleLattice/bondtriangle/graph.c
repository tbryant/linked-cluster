/* MAXSITE defines the struct size (must be big enough for the max) */
#define MAXSITE 16
/* MAXL is used for the bond coordinates x+MAXL*y*/
#define MAXL 100
/* CENTER is the location of the 0th element */
#define CENTER p(20,20)

typedef struct{
  int latconst;
  int site[MAXSITE];
} graph;


int p(int x, int y){
  return x+MAXL*y;
}

int x(int p){
  return p%MAXL;
}

int y(int p){
  return p/MAXL;
}


#define SHOWSIZE 10
int display(graph M){
  int j,k,l;
  int xc,yc;
  int show[SHOWSIZE][SHOWSIZE];

  printf("Lattice Constant: %i \n",M.latconst);
  
  printf("Sites:\n");
    for(k=0;k<SHOWSIZE;k++){
      for(l=0;l<SHOWSIZE;l++){
	show[k][l]=-1;
      }
    }
    for(j=0;j<G_Nsite;j++){
      printf("%i:(%i,%i)\n",j,x(M.site[j]),y(M.site[j]));

      xc = x(M.site[j]);
      yc = y(M.site[j]);
      
      show[xc][yc]=j;
    }
    for(k=SHOWSIZE-1;k>=0;k--){
      for(j=0;j<SHOWSIZE;j++){
	if(show[j][k]==-1)
	  printf("  ");
	else printf("%2i",show[j][k]);      
      }
      printf("\n");
    }
    printf("\n");
    return 0;
}
