#define CONVERTMAXBONDS 24

typedef struct{
  int tnnodes;
  int tnbonds;
  int tnsites;
  int bond[CONVERTMAXBONDS];
} mgraph;

typedef struct{
  int tnbonds;
  int subgr_mult;
  int subgr_topo;
} msubgraph;

void fwrite_mgraph(mgraph G,FILE *fp){
  int i;
  char x,y[2];

  x = (char)(G.tnnodes);
  fwrite(&x,sizeof(char),1,fp);
  x = (char)(G.tnsites);
  fwrite(&x,sizeof(char),1,fp);
  for(i=0;i<G.tnbonds;i++){
    y[0] = (char)((G.bond[i])&0xff);
    y[1] = (char)((G.bond[i]>>8)&0xff);
    fwrite(&y,2*sizeof(char),1,fp);
  }  
}

int fread_mgraph(int tnbonds,mgraph *G,FILE *fp){
  int i;
  char x,y[2];

  (*G).tnbonds = tnbonds;

  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*G).tnnodes=(int)x;
  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*G).tnsites=(int)x;
  for(i=0;i<tnbonds;i++){
    if(fread(&y,2*sizeof(char),1,fp)==0) return 0;
    (*G).bond[i]=((((int)(y[1]))&0xff)<<8) + (int)(y[0]&0xff);
  }
  return 1;  
}

void display_mgraph(mgraph G){
  int i;
  printf("tnbonds=%i tnnodes=%i tnsites=%i ",G.tnbonds,G.tnnodes,G.tnsites);
  for(i=0;i<G.tnbonds;i++){
    printf("%i ",G.bond[i]);
  }
  printf("\n");
}

void display_msubgraph(msubgraph S){
  printf("tnbonds=%i subgr_mult=%i subgr_topo=%i\n",S.tnbonds,S.subgr_mult,S.subgr_topo);
}

void fwrite_msubgraph(msubgraph S,FILE *fp){
  char x,y[2];

  x = (char)(S.tnbonds);
  fwrite(&x,sizeof(char),1,fp);
  
  if(S.subgr_mult>=pow(2,15)){
    /* check to see that it will fit in 2 Bytes */
    printf("subgr_mult too large to fit into 2 Bytes!\n");
    display_msubgraph(S);
    exit(1);
  }
  y[0] = (char)(S.subgr_mult&0xff);
  y[1] = (char)((S.subgr_mult>>8)&0xff);
  fwrite(&y,2*sizeof(char),1,fp);

  fwrite(&(S.subgr_topo),sizeof(int),1,fp);
}

int fread_msubgraph(msubgraph *S,FILE *fp){
  char x,y[2];

  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*S).tnbonds=(int)x;
  
  if(fread(&y,2*sizeof(char),1,fp)==0) return 0;
  (*S).subgr_mult=((((int)(y[1]))&0xff)<<8) + (int)(y[0]&0xff);
  
  if(fread(&((*S).subgr_topo),sizeof(int),1,fp)==0) return 0;
  
  
  return 1;  
}
