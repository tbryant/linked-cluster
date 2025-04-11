#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int G_Nsite;
#include "graph.c"
#include "bondgraph.c"
#include "topoutils.c"

typedef struct{
  int length;
  int ishair;
  int site[MAXBOND];
} bridgetypebig;

typedef struct{
  int topo;
  bondgraphbig embed;
  int Nbond;
  int Nsite;
  int Nnode;
  int Nbridge;
  int latconst;
  bridgetypebig bridge[2*MAXBOND]; 
  int topolabel[MAXSITE];
  int embedlabel[MAXSITE];
  int site_x[MAXSITE];
  int site_y[MAXSITE];
  int left[MAXBOND];
  int right[MAXBOND];
  int SA[MAXBOND][MAXSITE];
} topobig;

int main(int argc, char *argv[]){
  topo T;
  topobig Tb;
  int Nbond,Nsite,toponum=-1;
  int Tcount=0;
  char filename[100];
  FILE *infp,*outfp;
  int i,j;

  printf("topobig %i, topo %i\n",sizeof(topobig),sizeof(topo));
  
  if((argc!=3)&&(argc!=4)){
    printf("converttopolist.e Nbond Nsite [toponum]\n");
    exit(0);
  }
  
  Nbond=atoi(argv[1]);
  Nsite=atoi(argv[2]);
  if(argc==4)
    toponum=atoi(argv[3]);
  
  
  /* open topolist file */
  sprintf(filename,"graphs/bondgraph_topolist%i_%i.big",Nbond,Nsite);
  printf("opening %s\n\n",filename);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for intput\n",filename);
    exit(1);
  }


  /* open new topolist file */
  sprintf(filename,"graphs/bondgraph_topolist%i_%i.dat",Nbond,Nsite);
  printf("opening %s\n\n",filename);
  if ((outfp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for intput\n",filename);
    exit(1);
  }
  while(fread(&Tb,sizeof(topobig),1,infp)){
    /* init T */
    T.topo=Tb.topo;
    T.embed=Tb.embed;
    T.Nbond=Tb.Nbond;
    T.Nsite=Tb.Nsite;    
    T.Nnode=Tb.Nnode;
    T.Nbridge=Tb.Nbridge;
    T.latconst=Tb.latconst;
    for(i=0;i<MAXBOND;i++){
      T.bridge[i].length=Tb.bridge[i].length;
      T.bridge[i].ishair=Tb.bridge[i].ishair;
      for(j=0;j<MAXSITE;j++){
	T.bridge[i].site[j]=Tb.bridge[i].site[j];}
    }
    for(i=0;i<MAXSITE;i++){
      T.topolabel[i]=Tb.topolabel[i];
      T.embedlabel[i]=Tb.embedlabel[i];
      T.site_x[i]=Tb.site_x[i];
      T.site_y[i]=Tb.site_y[i];
    }
    for(i=0;i<MAXBOND;i++){
      T.left[i]=Tb.left[i];
      T.right[i]=Tb.right[i];
  }
    for(i=0;i<MAXSITE;i++){
      for(j=0;j<MAXSITE+1;j++){
	T.SA[i][j]=Tb.SA[i][j];
      }
    }
    
    Tcount++;
/*     if((toponum==-1)||(toponum==T.topo)) */
/*       displaytopo(T); */
   
    fwrite(&T,sizeof(topo),1,outfp);
  }
  printf("Tcount = %i\n",Tcount);
  
  fclose(infp);

  
  return 0;
}
