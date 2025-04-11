#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sitesquareutils.c"
#include "dltopo.c"

int isneighbor(x1,y1,x2,y2){
  if((x1==x2)&&(abs(y1-y2)==1))
    return 1;
  
  if((abs(x1-x2)==1)&&(y1==y2))
    return 1;

  return 0;
}

int siteaddtoclist(const bondgraphbig B, int clist[MAXSITE], int *listp){
  /* returns 1 if clist is extended, 0 otherwise */
  int i,j,k;  

  if(*listp>=B.Nsite)
    return 0;
  
  for(i=0;i<(*listp);i++){
    /* i is a site on clist */
    for(j=0;j<B.Nsite;j++){
      /* j is another site */
      if(isneighbor(B.site_x[clist[i]],B.site_y[clist[i]],B.site_x[j],B.site_y[j])){
	/* if j is not on clist, add it, and return 1 */
	k=0;
	while((k<(*listp))&&(clist[k]!=j))
	  k++;
	if(k==(*listp)){
	  /* it isn't on the list */
	  clist[(*listp)]=j;
	  (*listp)++;
	  return 1;
	}
      }
    }
  }
  return 0;
}


int siteconnected(bondgraphbig B){
  /* returns 1 if B is connected*/
  /* 0 otherwise */
  int clist[MAXSITE],listp;
  /* don't check one site bgbs */
  if(B.Nsite<2)
    return 1;

  /* start with 0 */
  clist[0]=0;
  listp=1;
  /*  keep trying to add more sites */
  while(siteaddtoclist(B,clist,&listp)){
    if(listp==B.Nsite){
      /* it is connected */
      return 1;
    }
  }
  /* not connected */
  return 0;
}



toponode *findintopohashtable(topo T,unsigned int key,toponode*head){
  /* returns the pointer to the match, or NULL if there is no match */
  while((head!=NULL)&&(head->key==key)){
    if(issameSA(T,head->data)){
      /* match! */
/*       printf("found in hash table!\n"); */
      return head;
    }
    head = head->next;
  }
  /* it wasn't a match */
  return NULL;  
}

void writesitemap(FILE *sitefp,topo T){
  int i;
  fprintf(sitefp,"%i %i ",T.topo,T.Nsite);
  for(i=0;i<T.Nsite;i++)
    fprintf(sitefp,"%i ",T.topolabel[i]);
  fprintf(sitefp,"\n");
}

int main(int argc, char *argv[]){
  bondgraphbig B;
  subtopobig Sbig;
  subtoposmall Ssmall;
  int Scount=0;
  int Nsite,Nsitemax;
  topo T,Tcand;
  toponode *first,*head,*tempnode;
  toponode **hash;
  unsigned int key;
  char filename[100];
/*   int parenttopolabel[MAXBOND+1]; */
  int candmap[MAXSITE],Tcount[MAXSITE+1];
  FILE *infp[MAXSITE+1],*subtopofp;
  int i,candint;
  
  hash = (toponode **)calloc(TOPOHASHSIZE,sizeof(toponode *));
  
  if(argc!=2){
    printf("makesubtopolist.e Nsite\n");
    exit(0);
  }

  Nsitemax=atoi(argv[1]);
  
  /* open all topo files up to Nsitemax */
  for(Nsite=1;Nsite<=Nsitemax;Nsite++){
    sprintf(filename,"graphs/sitegraph_topolist%i.dat",Nsite);
    if ((infp[Nsite] = fopen(filename, "r"))==NULL){
      printf("Cannot open file %s for input\n",filename);
      exit(1);
    }
    fread(&(Tcount[Nsite]),sizeof(int),1,infp[Nsite]);
  }
  

  /* open subtopolist file */
  sprintf(filename,"graphs/sitegraph_subtopolist%i.dat",Nsitemax);
  if ((subtopofp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }


  /* make linked list of all topos smaller than Nsitemax*/  
  first=NULL;
  for(Nsite=1;Nsite<Nsitemax;Nsite++){ 
    while(fread(&T,sizeof(topo),1,infp[Nsite])){
      key = topokeygen(T);
      if(first==NULL){
	/* first one it list */
	topoinsert(&first,T,key);
	hash[key]=first;
	head = first;
      }
      /* otherwise check in the list*/
      if(hash[key]==NULL){
	/* it isn't in the hash table yet, so put it in */
	topoinsert(&head,T,key);
	hash[key]=head;
	
	/*       printf("adding one not it the hash table \n"); */
	/*       displaytopo(head->data); */
      }
      else{
	/* if it doesn't exist, but shares a key, insert it at hash[key] */
	tempnode=hash[key];
	topoinsert(&tempnode,T,key);
	/* hash key still points to the first with same key */
	/* 	printf("this one shared a key in the hash table \n"); */
	/* 	displaytopo(tempnode->data); */
	/* make sure not to chop off the end */	   
	while((head->next)!=NULL)
	  head = head->next;
      }
    }
    fclose(infp[Nsite]);
  }
  
  /* now read the topos from Nsitemax */
  while(fread(&T,sizeof(topo),1,infp[Nsitemax])){
    
/*     displaytopo(T); */

    /* go through all 2^Nsite - 2 possible candidates */
    for(candint=1;candint<(pow(2,Nsitemax)-1);candint++){
      /* make the bgb */
      /* and candmap which maps the sites */
      B = subbgb(T.embed,candint,candmap);

      /* check that it is connected */
      if(siteconnected(B)){
	
/* 	displaybgb(B); */
/* 	printf("is connected!\n"); */
	
        /* make the topo */
	Tcand = topology(B);
	key = topokeygen(Tcand);
	
	
/* 	displaytopo(Tcand); */
/* 	/\* show candmap *\/ */
/* 	printf("candmap "); */
/* 	for(i=0;i<Tcand.Nsite;i++) */
/* 	  printf("%i ",candmap[i]); */
/* 	printf("\n"); */

	/* find the topo */
	head = hash[key];
	if((head=findintopohashtable(Tcand,key,head))==NULL){
/* 	  printf("topo not in hashtable - key %i\n",key); */
/* 	  displaytopo(Tcand); */
/* 	  exit(0); */
	}
	else{
	Sbig.parenttopo = T.topo;
	Sbig.Nbond = head->data.Nbond;
	Sbig.topo = head->data.topo;
	Sbig.Nsite = Tcand.Nsite;
	for(i=0;i<Sbig.Nsite;i++){
	  Sbig.topositemap[i] = T.topolabel[candmap[Tcand.embedlabel[i]]];
	}
/* 	displaysubtopobig(Sbig); */
	Ssmall = subtopobigtosmall(Sbig);
	fwrite(&Ssmall,sizeof(subtoposmall),1,subtopofp);
	Scount++;
/* 	displaytopo(Tcand); */
	/* write the subtopo with site labels (be careful here) */
	}
      }
    }
  }

  printf("%16i %16i\n",Nsitemax,Scount);
  fclose(subtopofp);
  
  return 0;
}
