/* make bond graphs from site graphs in ../sitesquare/graphs/embedlist#.dat */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite,G_Nbond;

#include "graph.c"
#include "bondgraph.c"


int compare(const void *p1, const void *p2 ){
  return *(int *)p1 - *(int *)p2;
}

int order(graph *M){
  /* sort sites */
  qsort((void *)(*M).site,G_Nsite,sizeof(int),compare);
  return 0;
}

int translate(graph *M){
  /* translate sites so that embedding is at CENTER */
  int i,r;
 
  r=CENTER-(*M).site[0];
  
  for(i=0;i<G_Nsite;i++)
    (*M).site[i]+=r;
  return 0;
}

int noduplicates(graph M){
  /* returns 0 if the newest site is a duplicate */
  int i;
  for(i=0;i<G_Nsite-1;i++){
    if(M.site[i]==M.site[G_Nsite-1])
      return 0;
  }
  return 1;
}

int comparegraphs(graph M1, graph M2){
  /* returns +1 if M1 is bigger, -1 if M2 is bigger, 0 if they are identical */
  int i=0;
  while((i<G_Nsite)&&(M1.site[i]==M2.site[i])){
    i++;
  }
 
  /* if they match */
  if(i==G_Nsite)
    return 0;  

/*   printf("The following two graphs do not match\n"); */
/*   display(M1); */
/*   display(M2); */

  /* if they do not */
  if(M1.site[i]>M2.site[i])
    return 1;
  else
    return -1;
}  

void rotations(graph *M){
  /* check symmetries of the graph to get lattice constant */
  int i,rot,samecount=1;
  int xc[MAXSITE],yc[MAXSITE];
  graph Mlowest,Mrot;

  Mlowest=(*M);

  for(i=0;i<G_Nsite;i++){
    xc[i]=x((*M).site[i])-x(CENTER);
    yc[i]=y((*M).site[i])-y(CENTER);
  }
  
  for(rot=0;rot<7;rot++){
    /* seven (not the identity) symmetries of the 2d square */
    switch(rot){
      /* 0: x,y -> -x,y */
    case 0 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)-xc[i],y(CENTER)+yc[i]);
      break;

      /* 1: x,y -> x,-y */
    case 1 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)+xc[i],y(CENTER)-yc[i]);
      break;

      /* 2: x,y -> -x,-y */
    case 2 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)-xc[i],y(CENTER)-yc[i]);
      break;

      /* 3: x,y -> y,x */
    case 3 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)+yc[i],y(CENTER)+xc[i]);
      break;

      /* 4: x,y -> -y,x */
    case 4 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)-yc[i],y(CENTER)+xc[i]);
      break;

      /* 5: x,y -> y,-x */
    case 5 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)+yc[i],y(CENTER)-xc[i]);
      break;

      /* 6: x,y -> -y,-x */
    case 6 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)-yc[i],y(CENTER)-xc[i]);
      break;
	
    default :	break;
    }
    
    order(&Mrot);
    translate(&Mrot);
    if(comparegraphs(*M,Mrot)==0)
      samecount++;
    
    if(comparegraphs(Mlowest,Mrot)>0)
      Mlowest=Mrot;
  }
  /* put the lowest rotation into M */
  (*M)=Mlowest;
  (*M).latconst=8/samecount;
/*   printf("samecount %i, latconst %i\n",samecount,8/samecount); */
}

int main(int argc, char *argv[]){
  graph M;
  bondgraph B;
  char filename[100];
  FILE *infp, *outfp;
  int curbond,Bcount=0;
  int Mcount=0;
  int i,j;
  

  if(argc!=2){
    printf("makebondgraph.e Nbond\n");
    exit(0);
  }
  
  G_Nbond=atoi(argv[1]);
  
  /* open output file */
  sprintf(filename,"graphs/bondgraphlist%i.dat",G_Nbond);
  if ((outfp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  /* open sitegraph files up to Nbond+1*/
  for(G_Nsite=2;G_Nsite<=G_Nbond+1;G_Nsite++){
    sprintf(filename,"../sitesquare/graphs/embedlist%i.dat",G_Nsite);
    if ((infp = fopen(filename, "r"))==NULL){
      printf("Cannot open file %s for input\n",filename);
      exit(1);
    }
    fread(&Mcount,sizeof(int),1,infp);
    printf("Mcount = %i\n",Mcount);
    
    /* read the sitegraph into M*/
    while(fread(&M,sizeof(graph),1,infp)){
      /* first, just for debugging, make one bondgraph with every possible bond */
      /* go through every site */

      curbond=0;
      B.bonddir=0;
      for(i=0;i<G_Nsite;i++){
	/* go through every other site */
	for(j=i+1;j<G_Nsite;j++){
	  /* the sites are in order */
	  /* check if there is a horizontal bond */
	  if(p(x(M.site[i])+1,y(M.site[i]))==M.site[j]){	    
	    B.bond[curbond] = makebond(x(M.site[i]),y(M.site[i]));
	    curbond++;
	   /*  B.bonddir remains 0 for this bond */
	  }
	  /* check for a vertical bond */
	  else
	    if(p(x(M.site[i]),y(M.site[i])+1)==M.site[j]){	    
	      B.bond[curbond] = makebond(x(M.site[i]),y(M.site[i]));
	      B.bonddir += (1<<curbond);
	      curbond++;
	    }
	}
      }
      B.xNbond_ylatconst=makebond(curbond,M.latconst);

      display(M);
      displaybondgraph(B);
    }

    fclose(infp);
  } 
  
  
    
  return 0;
}
