#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;
#include "../graph.c"
#include "../bondgraph.c"
#include "../topoutils.c"
#include "convertutils.c"

int convert_bond(int x,int y,int dir){
  int mx,my;
  mx = 50+2*x;
  my = 50+2*y;
  if(dir==0){
    mx++;
  }
  if(dir==1){
    my++;
  }
  if(dir==2){
    mx++;
    my++;
  }
  return (my-1)*100+mx;
}

mgraph convert_topo(topo T){
  int i;
  mgraph G;
  G.tnnodes=T.Nnode;
  G.tnbonds=T.Nbond;
  G.tnsites=T.Nsite;
  for(i=0;i<G.tnbonds;i++){
    G.bond[i]=convert_bond(T.embed.bond_x[i],T.embed.bond_y[i],T.embed.bonddir[i]);
  }
  return G;
}

int main(int argc, char *argv[]){
  int i,j;
  int Nbond,Nsite;
  int Nsitemax;
  int temp1,temp2;
  int topocount[MAXBOND][MAXSITE];
  int *topoindex[MAXBOND][MAXSITE];
  int toposum[MAXBOND];
  int graphcount[MAXBOND][MAXSITE];
  int graphsum[MAXBOND];
  int minsite[MAXBOND];
  int subcount,curtopo;
  int done;
  subtopo S;
  msubgraph ms;
  msubgraph sublist[65536]; 	/* should be at least as big as the largest sublist 2^MAXSITE */
  char filename[100];
  int Tcount,Mcount;
  topo T;
  mgraph G;
  FILE *inputlogfp,*infofp;
  FILE *ttopofp,*tsubfp;
  FILE *mtopofp,*mmultfp;
  
  /* init */
  for(i=0;i<MAXBOND;i++){
    for(j=0;j<MAXSITE;j++){
      topocount[i][j]=0;
      graphcount[i][j]=0;
    }
    toposum[i]=0;
    graphsum[i]=0;
    minsite[i]=0;
  }
  
  if(argc!=3){
    printf("convertformarcos.e Nsitemax inputlogfile\n");
    exit(0);
  }
  Nsitemax=atoi(argv[1]);
  
  printf("opening %s\n\n",argv[2]);
  if ((inputlogfp = fopen(argv[2], "r"))==NULL){
    printf("Cannot open file %s for intput\n",argv[2]);
    exit(1);
  }
  
  while(fscanf(inputlogfp,"%i %i %i %i\n",&Nbond,&Nsite,&temp1,&temp2)!=EOF){
/*     printf("%i %i %i %i\n",Nbond,Nsite,temp1,temp2); */
    topocount[Nbond][Nsite]=temp1;
    graphcount[Nbond][Nsite]=temp2;
  }
  fclose(inputlogfp);

  for(j=0;j<=Nsitemax;j++){
    for(i=0;i<MAXBOND;i++){
      if(topocount[i][j]!=0){
	/* allocate space for topoindex */
	topoindex[i][j]=(int *)malloc(topocount[i][j]*sizeof(int));
	if(minsite[i]==0)
	  minsite[i]=j;
	/* 	printf("topocount[%i][%i]=%i\n",i,j,topocount[i][j]); */
	toposum[i]+=topocount[i][j];
	graphsum[i]+=graphcount[i][j];
      }
    }
  }
  
  /* open info file */
  sprintf(filename,"output%i/TOPOLOGY_INFORMATION.dat",Nsitemax);
  printf("writing %s\n",filename);
  if ((infofp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  fprintf(infofp,"# of bonds     # of topologies   Total # of graphs  Minimum # of sites\n");
  for(i=0;i<MAXBOND;i++){
    if(toposum[i]!=0){
      fprintf(infofp,"%10i%20i%20i%20i\n",i,toposum[i],graphsum[i],minsite[i]);
    }
  }
  
    fclose(infofp);
  
  /* open site info file */
  sprintf(filename,"output%i/SITENUMBER_INFORMATION.dat",Nsitemax);
  printf("writing %s\n",filename);
  if ((infofp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  fprintf(infofp,"  #sites");
  for(i=0;i<MAXBOND;i++){
    if(toposum[i]!=0)
      fprintf(infofp,"%8i",i);
  }
  fprintf(infofp,"\n\n");
  for(j=0;j<=Nsitemax;j++){
    fprintf(infofp,"%8i",j);
    for(i=0;i<MAXBOND;i++){ 
      if(toposum[i]!=0)     
	fprintf(infofp,"%8i",topocount[i][j]);
    }
    fprintf(infofp,"\n");
  }
  
  fclose(infofp);
  
  /* make topo graph files */
  for(Nbond=1;Nbond<MAXBOND;Nbond++){
    if(toposum[Nbond]!=0){
      /* open output files for Nbond */
      sprintf(filename,"output%i/TOPO_GRAPH_%02i_BONDS.bin",Nsitemax,Nbond);
      printf("writing %s\n",filename);
      if ((mtopofp = fopen(filename, "w"))==NULL){
	printf("Cannot open file %s\n",filename);
	exit(1);
      }
      sprintf(filename,"output%i/TOPO_MULT_%02i_BONDS.bin",Nsitemax,Nbond);
      printf("writing %s\n",filename);
      if ((mmultfp = fopen(filename, "w"))==NULL){
	printf("Cannot open file %s\n",filename);
	exit(1);
      }
    
      Mcount=1;
    
      for(Nsite=2;Nsite<=Nsitemax;Nsite++){
	if(topocount[Nbond][Nsite]!=0){
	  /* open input topofile (should it exist) */
	  sprintf(filename,"../graphs/bondgraph_topolist%i_%i.dat",Nbond,Nsite);
	  printf("reading %s\n",filename);
	  if ((ttopofp = fopen(filename, "r"))==NULL){
	    printf("Cannot open file %s for input\n",filename);
	    exit(1);
	  }
	  Tcount=0;
	
	  while(readtopofromfile(&T,ttopofp)){
	    /* topoindex maps to the toponumber (used in subgraph) */
	    if(Tcount!=T.topo)
	      printf("warning, T.topo=%i, Tcount=%i\n",T.topo,Tcount);
	    topoindex[Nbond][Nsite][Tcount]=Mcount;
	    Tcount++;
	    Mcount++;
	    /* write latconst to mult */
	    fwrite(&(T.latconst),sizeof(int),1,mmultfp);
	    G = convert_topo(T);
	    /* write graph into list */
	    fwrite_mgraph(G,mtopofp);
	  }
	  fclose(ttopofp);
	}
      }
      fclose(mtopofp);
      fclose(mmultfp);
    }
  }

  /* and now for the subgraphs */
  for(Nbond=1;Nbond<MAXBOND;Nbond++){
    if(toposum[Nbond]!=0){
      /* open output files for Nbond */
      sprintf(filename,"output%i/SUBGRAPH_LIST_%02i_BONDS.bin",Nsitemax,Nbond);
      printf("writing %s\n",filename);
      if ((mtopofp = fopen(filename, "w"))==NULL){
	printf("Cannot open file %s\n",filename);
	exit(1);
      }
      sprintf(filename,"output%i/SUBGRAPH_TOPOMULT_%02i_BONDS.bin",Nsitemax,Nbond);
      printf("writing %s\n",filename);
      if ((mmultfp = fopen(filename, "w"))==NULL){
	printf("Cannot open file %s\n",filename);
	exit(1);
      }
    
      for(Nsite=2;Nsite<=Nsitemax;Nsite++){
	if(topocount[Nbond][Nsite]!=0){
	  /* open input subgraphfile (should it exist) */
	  sprintf(filename,"../graphs/bondgraph_subtopolist%i_%i.dat",Nbond,Nsite);
	  printf("reading %s\n",filename);
	  if ((tsubfp = fopen(filename, "r"))==NULL){
	    printf("Cannot open file %s for input\n",filename);
	    exit(1);
	  }
	  
	  curtopo=0;
	  subcount=0;
	  while(readsubtopofromfile(&S,tsubfp)){
/* 	    displaysubtopo(S); */
	    if(curtopo!=S.parenttopo){
	      /* write previous sublist */
	      /* first write count */
	      fwrite(&subcount,sizeof(int),1,mmultfp);
	      /* then write the list */
	      for(i=0;i<subcount;i++){
		  fwrite_msubgraph(sublist[i],mtopofp);
	      }
	      /* start a new sublist */
	      curtopo=S.parenttopo;
	      sublist[0].tnbonds = S.Nbond;
	      sublist[0].subgr_mult = 1;
	      sublist[0].subgr_topo = topoindex[S.Nbond][S.Nsite][S.topo];
	      subcount=1;
	    }
	    else{
	      ms.tnbonds = S.Nbond;
	      ms.subgr_mult = 1;
	      ms.subgr_topo = topoindex[S.Nbond][S.Nsite][S.topo];
/* 	      printf("topoindex[%i][%i][%i]=%i\n",S.Nbond,S.Nsite,S.topo,topoindex[S.Nbond][S.Nsite][S.topo]); */
	      /* check to see if subgraph is already in list */
	      done=0;
	      i=0;
	      while((i<subcount)&&(done==0)){
		if((ms.tnbonds==sublist[i].tnbonds)&&(ms.subgr_topo==sublist[i].subgr_topo)){
		  sublist[i].subgr_mult++;
		  done=1;
		}
		i++;
	      }
	      if(done==0){
		/* add to list */
		sublist[subcount] = ms;
		subcount++;
	      }
	    }
	  }
	  /* write final sublist */
	  /* first write count */
	  fwrite(&subcount,sizeof(int),1,mmultfp);
	  /* then write the list */
	  for(i=0;i<subcount;i++){
	    fwrite_msubgraph(sublist[i],mtopofp);
	  }
	  
	  fclose(tsubfp);
	}
      }
      fclose(mtopofp);
      fclose(mmultfp);
    }
  }
  
  for(Nbond=1;Nbond<MAXBOND;Nbond++){
    for(Nsite=2;Nsite<=Nsitemax;Nsite++){
      if(topocount[Nbond][Nsite]!=0){
	free(topoindex[Nbond][Nsite]);
      }
    }
  }

  return 0;
}
