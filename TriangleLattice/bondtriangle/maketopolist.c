#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;
#include "graph.c"
#include "bondgraph.c"
#include "topoutils.c"
#include "dltopoSA.c"


toponode *findintopohashtable(topoSA T,unsigned int key,toponode*head){
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

void writesitemap(FILE *fp,topo T){
  int i;
  char x;
  
  fwrite(&(T.topo),sizeof(int),1,fp);
  
  x = (char)T.Nsite;
  fwrite(&x,sizeof(char),1,fp);
  for(i=0;i<T.Nsite;i++){
    x = (char)T.topolabel[i];
    fwrite(&x,sizeof(char),1,fp);
  }
}

int main(int argc, char *argv[]){
  bondgraphbig B;
  int Nbond,Nsite;
  int totalcount,bcount;
  topo T;
  topoSA TSA;
  toponode *first,*head,*tempnode;
  toponode **hash;
  unsigned int key;
  char filename[100];
  FILE *infp,*sitefp,*topofp,*tmpfp;
  int curtopo=0;
  
  hash = (toponode **)calloc(TOPOHASHSIZE,sizeof(toponode *));
  
  if(argc!=3){
    printf("maketopolist.e Nbond Nsite\n");
    exit(0);
  }

  Nbond=atoi(argv[1]);
  Nsite=atoi(argv[2]);
  
  /* open embeddings file */
  sprintf(filename,"graphs/bondgraphlist%i_%i.dat",Nbond,Nsite);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }

  /* open sitemap file for embeddings */
  sprintf(filename,"graphs/bondgraph_sitemap%i_%i.dat",Nbond,Nsite);
  if ((sitefp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }


  /* open topolist file */
  sprintf(filename,"graphs/bondgraph_topolist.tmp");
  if ((tmpfp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  curtopo=0;
  /* make first in the linked list */
  readbgbfromfile(&B,Nbond,infp);

  T = topology(B);
  T.topo=curtopo;
  key = topokeygen(T);
  TSA = maketopoSA(T);
  first=NULL;
  topoinsert(&first,TSA,key);
  hash[key]=first;
  curtopo++;
  head = first;
  /* write to sitemapfile */
  writesitemap(sitefp,T);
  /* write topo to file */
  /*   fwrite(&T,sizeof(topo),1,tmpfp); */
  writetopotofile(T,tmpfp);
  

  /*   displaybgb(B); */
  /*   printf("key %i\n",key); */
  /*   displaytopo(T); */
  
  
  /* now read the rest */
  bcount=0;
  while(readbgbfromfile(&B,Nbond,infp)){
    bcount++;

    /* display keys in list so far */
    /*     tempnode=first; */
    /*     printf("hash "); */
    /*     while(tempnode!=NULL){ */
    /*       printf("%i ",tempnode->key); */
    /*       tempnode=tempnode->next; */
    /*     } */
    /*     printf("\n"); */

    T = topology(B);
    T.topo=curtopo; 
    key = topokeygen(T);
    TSA = maketopoSA(T);

    /*     displaybgb(B); */
    /*     printf("key %i\n",key); */
    /*     displaytopo(T); */
 
    /* look for duplicates */
    if(hash[key]==NULL){
      /* it isn't in the hash table yet, so put it in */
      topoinsert(&head,TSA,key);
      /*       fwrite(&T,sizeof(topo),1,tmpfp); */
      writetopotofile(T,tmpfp);

      curtopo++;
      hash[key]=head;
      
      /*       printf("adding one not it the hash table \n"); */
      /*       displaytopo(T); */
    }
    else{
      tempnode=findintopohashtable(TSA,key,hash[key]);
      if(tempnode==NULL){
	/* if it doesn't exist, but shares a key, insert it at hash[key] */
	tempnode=hash[key];
	topoinsert(&tempnode,TSA,key);
	/* 	fwrite(&T,sizeof(topo),1,tmpfp); */
	writetopotofile(T,tmpfp);
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
	/* 	displaytopo(T); */

      }
    }      
    /* write a sitemap for every embedding */
    writesitemap(sitefp,T);

  }

  printf("%i %i %i ",Nbond,Nsite,curtopo);

  /* now read through the file again, and put the correct latconst with every topo */

  fclose(tmpfp);
  /* open temp topolist file for input */
  sprintf(filename,"graphs/bondgraph_topolist.tmp");
  if ((tmpfp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }

  
  /* open topolist file for output */
  sprintf(filename,"graphs/bondgraph_topolist%i_%i.dat",Nbond,Nsite);
  if ((topofp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }
  
  totalcount=0;
  /*   while(fread(&T,sizeof(topo),1,tmpfp)){ */
  while(readtopofromfile(&T,tmpfp)){
    key = topokeygen(T);
    TSA = maketopoSA(T);
    tempnode=findintopohashtable(TSA,key,hash[key]);
    T.latconst=tempnode->data.latconst;
    totalcount+=T.latconst;
    /*     fwrite(&T,sizeof(topo),1,topofp); */
    writetopotofile(T,topofp);
  }
  printf("%i\n",totalcount);

  topofree(first);

  free(hash);

  fclose(infp);
  fclose(sitefp);
  return 0;
}
