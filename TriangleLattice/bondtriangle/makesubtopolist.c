#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;
#include "graph.c"
#include "bondgraph.c"
#include "topoutils.c"
#include "dltoposub.c"


int addtoclist(int Nsite, int Nbond, int clist[MAXBOND+1], int left[MAXBOND], int right[MAXBOND], int *listp){
  /* returns 1 if clist is extended, 0 otherwise */
  int i,j,k;  
  int ret=0;
  int lnew,rnew;

  if(*listp>=Nsite)
    return 0;

  for(i=0;i<Nbond;i++){
    /* check every bond */
    
    for(j=0;j<(*listp);j++){
      lnew=0;
      rnew=0;

      /* go through each site on clist */
      if(left[i]==clist[j]){
	rnew=1;
	/* if one is the left of the bond */
	for(k=0;k<(*listp);k++){
	  /* check to see if the right is on the list already */
	  if(right[i]==clist[k])
	    rnew=0;
	}
      }
      
      if(right[i]==clist[j]){
	lnew=1;
	/* if one is the left of the bond */
	for(k=0;k<(*listp);k++){
	  /* check to see if the right is on the list already */
	  if(left[i]==clist[k])
	    lnew=0;
	}
      }
      
      if(lnew){
	/* left side of bond is new */
	clist[(*listp)]=left[i];
	(*listp)++;
	ret=1;
      }
      
      if(rnew){
	/* right side of bond is new */
	clist[(*listp)]=right[i];
	(*listp)++;
	ret=1;
      }
    }
  }
  return ret;
}
  
int connected(bondgraphbig B){
  /* returns 1 if B is connected*/
  /* 0 otherwise */
  int i,j,listp;
  int left[MAXBOND],right[MAXBOND];
  int sitec=0;
  int site_x[MAXBOND+1],site_y[MAXBOND+1];
  int clist[MAXBOND+1];
  
  /* label the sites */
  /* build left and right */
  
  for(i=0;i<B.Nbond;i++){
    /* left site of bond */
    site_x[sitec]=B.bond_x[i];
    site_y[sitec]=B.bond_y[i]; 
    
    /* check that the site isn't already labeled */
    j=0;
    while((j<sitec)&&((site_x[j]!=site_x[sitec])||(site_y[j]!=site_y[sitec]))){
      j++;
    }
    if(j==sitec){
      /* new site */
      left[i]=j;
      sitec++;
    }
    else{
      /* site already exits */
      left[i]=j;
    }

    /* right site of bond */
    if(B.bonddir[i]==0){	  
      site_x[sitec]=B.bond_x[i]+1;
      site_y[sitec]=B.bond_y[i];
    }	  
    if(B.bonddir[i]==1){
      site_x[sitec]=B.bond_x[i];
      site_y[sitec]=B.bond_y[i]+1; 
    }	  
    if(B.bonddir[i]==2){
      site_x[sitec]=B.bond_x[i]+1;
      site_y[sitec]=B.bond_y[i]+1; 
    }
    
    /* check that the site isn't already labeled */
    j=0;
    while((j<sitec)&&((site_x[j]!=site_x[sitec])||(site_y[j]!=site_y[sitec]))){
      j++;
    }
    if(j==sitec){
      /* new site */
      right[i]=j;
      sitec++;
    }
    else{
      /* site already exits */
      right[i]=j;
    }
  }

/*   printf("sitec=%i\n",sitec); */

  /* now check to see if the graph is connected */
  clist[0] = left[0];
  clist[1] = right[0];
  listp=2;
  if(listp==sitec)
    return 1;
  /* if it has more than 2 sites, keep trying to add more */
  while(addtoclist(sitec,B.Nbond,clist,left,right,&listp)){
    if(listp==sitec){
      /* it is connected */
      return 1;
    }
  }
  /* not connected */
  return 0;
}

toponode *findintopohashtable(toposub Tsub,unsigned int key,toponode*head){
  /* returns the pointer to the match, or NULL if there is no match */
  while((head!=NULL)&&(head->key==key)){
    if(issameSAsub(Tsub,head->data)){
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
  subtopo S;
  int Scount=0;
  int Nbond,Nbondmax,Nsite,Nsitemax;
  topo T,Tcand;
  toposub Tsub,Tcandsub;
  toponode *first,*head,*tempnode;
  toponode **hash;
  unsigned int key;
  char filename[100];
/*   int parenttopolabel[MAXBOND+1]; */
  int candmap[MAXBOND+1];
  int using[MAXBOND+1][MAXSITE+1];
  FILE *infp[MAXBOND+1][MAXSITE+1],*subtopofp;
  int i,j,candint,candsite,cursite;
  
  hash = (toponode **)calloc(TOPOHASHSIZE,sizeof(toponode *));
  
/*   printf("sizeof(topo) = %i , sizeof(topoSA) = %i sizeof(toposub) = %i\n",sizeof(topo),sizeof(topoSA),sizeof(toposub)); */
 
  if(argc!=3){
    printf("makesubtopolist.e Nbond Nsite\n");
    exit(0);
  }

  Nbondmax=atoi(argv[1]);
  Nsitemax=atoi(argv[2]);

  
  for(Nbond=1;Nbond<=Nbondmax;Nbond++){
    for(Nsite=1;Nsite<=Nsitemax;Nsite++){
	using[Nbond][Nsite] = 0;
    }
  }
  
  /* open all topo files up to Nbondmax */
  for(Nbond=1;Nbond<=Nbondmax;Nbond++){
    for(Nsite=1;Nsite<=Nsitemax;Nsite++){
      sprintf(filename,"graphs/bondgraph_topolist%i_%i.dat",Nbond,Nsite);
      if ((infp[Nbond][Nsite] = fopen(filename, "r"))!=NULL){
	printf("%s\n",filename);
	using[Nbond][Nsite] = 1;
      }
    }
  }
  

  /* open subtopolist file */
  sprintf(filename,"graphs/bondgraph_subtopolist%i_%i.dat",Nbondmax,Nsitemax);
  if ((subtopofp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }


  /* make linked list of all topos smaller than Nbondmax*/  
  first=NULL;
  for(Nbond=1;Nbond<Nbondmax;Nbond++){ 
    for(Nsite=1;Nsite<=Nsitemax;Nsite++){
      if(using[Nbond][Nsite] == 1){
/* 	while(fread(&T,sizeof(topo),1,infp[Nbond][Nsite])){ */
	while(readtopofromfile(&T,infp[Nbond][Nsite])){
	  key = topokeygen(T);
	  Tsub = maketoposub(T);
	  
	  if(first==NULL){
	    /* first one it list */
	    topoinsert(&first,Tsub,key);
	    hash[key]=first;
	    head = first;
	  }
	  /* otherwise check in the list*/
	  if(hash[key]==NULL){
	    /* it isn't in the hash table yet, so put it in */
	    topoinsert(&head,Tsub,key);
	    hash[key]=head;
	    
	    /*       printf("adding one not it the hash table \n"); */
	    /*       displaytopo(head->data); */
	  }
	  else{
	    /* if it doesn't exist, but shares a key, insert it at hash[key] */
	    tempnode=hash[key];
	    topoinsert(&tempnode,Tsub,key);
	    /* hash key still points to the first with same key */
	    /* 	printf("this one shared a key in the hash table \n"); */
	    /* 	displaytopo(tempnode->data); */
	    /* make sure not to chop off the end */	   
	    while((head->next)!=NULL)
	      head = head->next;
	  }
	}
	fclose(infp[Nbond][Nsite]);
      }
    }
  }
      
  /* now read the topos from Nbondmax */
  /*   while(fread(&T,sizeof(topo),1,infp[Nbondmax][Nsitemax])){ */
  while(readtopofromfile(&T,infp[Nbondmax][Nsitemax])){
    
/*     displaytopo(T); */
    
    /* go through all 2^Nbond - 2 possible candidates */
    for(candint=1;candint<(pow(2,Nbondmax)-1);candint++){
      /* make the bgb */
      B.Nbond=0;

      B.latconst=1;
      cursite=0;
      for(i=0;i<Nbondmax;i++){
	if(((candint>>i)&1)==1){
	  /* bond present */
	  B.bonddir[B.Nbond] = T.embed.bonddir[i];
	  B.bond_x[B.Nbond]=T.embed.bond_x[i];
	  B.bond_y[B.Nbond]=T.embed.bond_y[i];
	  B.Nbond++;
	  
	  /* make candmap for sites */
	  /* left first */
	  candsite=T.left[i];
	  j=0;
	  while((j<cursite)&&(candsite!=candmap[j])){
	    /* check for the site already being in the list */
	    j++;
	  }
	  if(j==cursite){
	    /* new site */
	    candmap[cursite]=candsite;
	    cursite++;
	  }

	  /* then right */
	  candsite=T.right[i];
	  j=0;
	  while((j<cursite)&&(candsite!=candmap[j])){
	    /* check for the site already being in the list */
	    j++;
	  }
	  if(j==cursite){
	    /* new site */
	    candmap[cursite]=candsite;
	    cursite++;
	  }
	}
      }
      
      /*       displaybgb(B); */
      /* check that it is connected */
      if(connected(B)){
	/* 	printf("is connected!\n"); */
	/* make the topo */
	Tcand = topology(B);
	key = topokeygen(Tcand);
	Tcandsub = maketoposub(Tcand);
	

/* 	displaytopo(Tcand); */

	/* find the topo */
	head = hash[key];
	if((head=findintopohashtable(Tcandsub,key,head))==NULL){
	  printf("Error, topo not in hashtable\nkey %i\n",key);
	  displaytopo(Tcand);
	  exit(0);
	}
	
	S.parenttopo = T.topo;
	S.Nbond = head->data.embed.Nbond;
	S.topo = head->data.topo;
	S.Nsite = Tcand.Nsite;
	for(i=0;i<S.Nsite;i++){
	  S.topositemap[i] = T.topolabel[candmap[Tcand.embedlabel[i]]];
	}
/* 	displaysubtopo(S); */
	writesubtopotofile(S,subtopofp);
	Scount++;
/* 	displaytopo(Tcand); */
	/* write the subtopo with site labels (be careful here) */
      }
    }
  }

  printf("%i %i %i\n",Nbondmax,Nsitemax,Scount);
  fclose(subtopofp);
  
  return 0;
}
