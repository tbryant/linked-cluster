/* make bond graphs from site graphs in ../sitesquare/graphs/embedlist#.dat */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;

#include "graph.c"
#include "bondgraph.c"
#include "dlbondgraphbig.c"


int main(int argc, char *argv[]){
  bondgraphbig B;
  unsigned int key=1;
  int i,j,k,l;
  bgbnode *first=NULL,*head=NULL;
     
  for(i=0;i<10000;i++){
    if(i%1000==1)
      printf(".");
    bgbinsert(&first,B,key);
    head=first;
    for(j=0;j<100000;j++){
/*       for(k=0;k<1000;k++){ */
/* 	for(l=0;l<1000;l++){ */
/* 	} */
/*       } */
      bgbinsert(&head,B,key);
    }
    bgbfree(first);
  }
  return 0;
}
