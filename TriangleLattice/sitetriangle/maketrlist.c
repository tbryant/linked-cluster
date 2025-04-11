#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "sitesquareutils.c"
#include "dltopo.c"


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
  graph M;
  bondgraphbig B;
  int Mcount,totalcount=0;
  int Nsite;
  topo T;
  toponode *first,*head,*tempnode;
  toponode **hash;
  unsigned int key;
  char filename[100];
  FILE *infp,*sitefp,*topofp;
  int curtopo=0;
  
  hash = (toponode **)calloc(TOPOHASHSIZE,sizeof(toponode *));
  
  if(argc!=2){
    printf("maketopolist.e Nsite\n");
    exit(0);
  }

  Nsite=atoi(argv[1]);
  
  /* open embeddings file */
  sprintf(filename,"graphs/embedlist%i.dat",Nsite);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }
  
  /* read header Mcount */
  fread(&Mcount,sizeof(int),1,infp);
/*   printf("Mcount = %i\n",Mcount); */

  /* open sitemap file for embeddings */
  sprintf(filename,"graphs/sitegraph_sitemap%i.dat",Nsite);
  if ((sitefp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }


  /* open topolist file */
  sprintf(filename,"graphs/sitegraph_topolist%i.dat",Nsite);
  if ((topofp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  curtopo=0;
  Mcount=0;
  /* make first in the linked list */
  fread(&M,sizeof(graph),1,infp);
  B = sitegraphtobgb(M,Nsite);
  T = topology(B);
  T.topo=curtopo;  
  key = topokeygen(T);
  first=NULL;
  topoinsert(&first,T,key);
  hash[key]=first;
  curtopo++;      
  head = first;
  /* write to sitemapfile */
  writesitemap(sitefp,T);

/*   printf("embedding %i\n",Mcount); */
/*   displaygraph(M,Nsite); */
/*   displaybgb(B); */
/*   printf("key %i\n",key); */
/*   displaytopo(T); */
  
  
  /* now read the rest */
  while( fread(&M,sizeof(graph),1,infp)){
    Mcount++;
    
    /* display keys in list so far */
/*     tempnode=first; */
/*     printf("hash "); */
/*     while(tempnode!=NULL){ */
/*       printf("%i ",tempnode->key); */
/*       tempnode=tempnode->next; */
/*     } */
/*     printf("\n"); */


    B = sitegraphtobgb(M,Nsite);
    T = topology(B);
    T.topo=curtopo; 
    key = topokeygen(T);

/*     printf("embedding %i\n",Mcount); */
/*     displaybgb(B); */
/*     printf("key %i\n",key); */
/*     displaytopo(T); */
 
    /* look for duplicates */
    if(hash[key]==NULL){
      /* it isn't in the hash table yet, so put it in */
      topoinsert(&head,T,key);
      curtopo++;
      hash[key]=head;
      
/*       printf("adding one not it the hash table \n"); */
/*       displaytopo(head->data); */
    }
    else{
      tempnode=findintopohashtable(T,key,hash[key]);
      if(tempnode==NULL){
	/* if it doesn't exist, but shares a key, insert it at hash[key] */
	tempnode=hash[key];
	topoinsert(&tempnode,T,key);
	/* hash key still points to the first with same key */
	curtopo++;

/* 	printf("this one shared a key in the hash table \n"); */
/* 	displaytopo(tempnode->data); */

	/* make sure not to chop off the end */	   
	while((head->next)!=NULL)
	  head = head->next;
      }
      else{
	/* if it is there, add to latconst */
	/* and change T.topo to match */
	T.topo = tempnode->data.topo;
	tempnode->data.latconst+=B.latconst;

/* 	printf("this one existed in the hash table, so the latconst is being incremented \n"); */
/* 	displaytopo(tempnode->data); */

      }
    }      
    /* write a sitemap for every embedding */
    writesitemap(sitefp,T);
    
  }

  /* write header tcount */
  fwrite(&curtopo,sizeof(int),1,topofp);

  head=first;
  while(head!=NULL){
    fwrite(&(head->data),sizeof(topo),1,topofp);
    totalcount+=head->data.latconst;
    head=head->next;
  }

  printf("%16i %16i %16i\n",Nsite,curtopo,totalcount);

  topofree(first);

  free(hash);

  fclose(infp);
  fclose(sitefp);
  fclose(topofp);
  
  return 0;
}
