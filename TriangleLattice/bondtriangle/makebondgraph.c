/* make bond graphs from site graphs in ../sitesquare/graphs/embedlist#.dat */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;

#include "graph.c"
#include "bondgraph.c"
#include "dlbondgraphbig.c"


int addtoclist(int Nsite, int Nbond, int *clist, int *left, int *right, int *listp){
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
  
int connectedandspans(bondgraphbig B, graph M){
  /* returns 1 if B is connected and spans all sites in M*/
  /* 0 otherwise */
  int i,j,listp,sitex,sitey;
  int left[3*MAXSITE],right[3*MAXSITE];
  int clist[MAXSITE];
  
  /* build left and right */
  /* at first clist is the coordination number */
  /* and fill in clist to make sure that all sites are used */
  for(i=0;i<B.Nbond;i++){
    /* left site of bond */
    sitex=B.bond_x[i];
    sitey=B.bond_y[i]; 
    
    for(j=0;j<G_Nsite;j++){
      if((sitex==x(M.site[j]))&&(sitey==y(M.site[j]))){
	/* j is the correct site for the left of bond i */
	left[i]=j;
	clist[j]++;
      }
    }
    
    /* right site of bond */
    if(B.bonddir[i]==0)	  
      sitex += 1;
    if(B.bonddir[i]==1)	  
      sitey += 1;
    if(B.bonddir[i]==2){
      sitex += 1;
      sitey += 1;
    }
    
  
    for(j=0;j<G_Nsite;j++){
      if((sitex==x(M.site[j]))&&(sitey==y(M.site[j]))){
	/* j is the correct site for the right of bond i */
	right[i]=j;
	clist[j]++;
      }
    }
    
  }
    
  /* now check to see that clist is full  */
  for(j=0;j<G_Nsite;j++){
    if(clist[j]==0){
      /* the graph doesn't span all sites */
      return 0;
    }
    /* init clist (now it is being used to check connectedness) */
    clist[j]=0;
  }
  /* now check to see if the graph is connected */
  clist[0] = left[0];
  clist[1] = right[0];
  listp=2;
  while(addtoclist(G_Nsite,B.Nbond,clist,left,right,&listp)){
    if(listp==G_Nsite){
      /* it is connected */
      return 1;
    }
  }
  return 0;
}


bgbnode *findinhashtable(bondgraphbig B,unsigned int key,bgbnode*head){
  /* returns the pointer to the match, or NULL if there is no match */
  while((head!=NULL)&&(head->key==key)){
    if(comparebgb(B,head->data)==0){
      /* match! */
      /*       printf("found in hash table!\n"); */
      return head;
    }
    head = head->next;
  }
  /* it wasn't a match */
  return NULL;  
}

int main(int argc, char *argv[]){
  graph M;
  bondgraphbig Bfull,Bfullrotated,B;
  char filename[100];
  FILE *infp, *outfp[3*MAXSITE] ,*logfp;
  int Nbondmax=0,Nbond,Nbondmax_input;
  int curbond,Bcount[3*MAXSITE],Bcounttotal[3*MAXSITE];
  int isfirst,Mcount=0;
  int i,j,sitecount,listsize;
  bgbnode *first=NULL,*tempnode,*head=NULL;
  unsigned int key;
  bgbnode **hash;
  

  if(argc!=3){
    printf("makebondgraph.e Nbondmax_input Nsite\n");
    exit(0);
  }
  
  Nbondmax_input = atoi(argv[1]);
  G_Nsite = atoi(argv[2]);
  printf("Nsite = %i\n",G_Nsite);
  
  
  /*   hash[i] gives the location of the first embedding with key=i */
  if((hash = (bgbnode **)calloc(BGBHASHSIZE,sizeof(bgbnode *)))==NULL){
    printf("unable to allocate memory for hash table\n");
    exit(0);
  }
  /* init outfp */
  for(i=0;i<3*MAXSITE;i++){
    Bcount[i]=0;
    Bcounttotal[i]=0;
    outfp[i]=NULL;
  }
  /* open sitegraph file */
  
  sprintf(filename,"../sitetriangle/graphs/embedlist%i.dat",G_Nsite);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }
  fread(&Mcount,sizeof(int),1,infp);
  printf("Sitegraphs %i site,  count=%i\n",G_Nsite,Mcount);
    
  /* read the sitegraph into M*/
  sitecount=0;
  while(fread(&M,sizeof(graph),1,infp)){
    sitecount++;
    /* first make one bondgraph with every possible bond */
    /* go through every site */


    curbond=0;

    /*       display(M); */
    for(i=0;i<G_Nsite;i++){
      /* go through every other site */
      for(j=0;j<G_Nsite;j++){
	/* check if there is a horizontal bond */
	if(p(x(M.site[i])+1,y(M.site[i]))==M.site[j]){	    
	  Bfull.bond_x[curbond] = x(M.site[i]);
	  Bfull.bond_y[curbond] = y(M.site[i]);
	  Bfull.bonddir[curbond] = 0;
	  curbond++;
	}
	/* check for a vertical bond */
	if(p(x(M.site[i]),y(M.site[i])+1)==M.site[j]){	    
	  Bfull.bond_x[curbond] = x(M.site[i]);
	  Bfull.bond_y[curbond] = y(M.site[i]);	    
	  Bfull.bonddir[curbond] = 1;
	  curbond++;
	}
	/* and for a diagonal / bond */
	if(p(x(M.site[i])+1,y(M.site[i])+1)==M.site[j]){	    
	  Bfull.bond_x[curbond] = x(M.site[i]);
	  Bfull.bond_y[curbond] = y(M.site[i]);	    
	  Bfull.bonddir[curbond] = 2;
	  curbond++;
	}
      }
    }
    Bfull.Nbond=curbond;
    Bfull.latconst=1;
    Bfullrotated=Bfull;
    /*       printf("Bfull\n"); */
    rotate(&Bfullrotated);

    /*       printf("sitecount %i\n",sitecount); */
    /*       display(M); */
    /*       printf("G_Nsite=%i\n",G_Nsite); */
    /*       displaybgb(Bfull); */
      
      
    /* start a linked list of graphs */
    isfirst=1;
    
    /* this one must be connected, so put it in the list*/
    /* 	printf("beginning a list...\n"); */
    if(Bfullrotated.Nbond<=Nbondmax_input){
      key = bgbkeygen(Bfullrotated);
      bgbinsert(&first,Bfullrotated,key);
      hash[key]=first;
      head=first;
      isfirst=0;
    }
  
    /*       printf("displaying Bfull\n"); */
    /*       displaybgb(Bfull); */
      

    /* loop over all subgraphs */
    /* by taking away some bonds */
    for(i=0;i<pow(2,Bfull.Nbond)-1;i++){
      /* make B */
      /* first count the number of bonds present */
      B.Nbond=0;
      /* start latconst at 1, at the end, multiply by M.latconst */
      B.latconst=1;
      for(j=0;j<Bfull.Nbond;j++){
	B.Nbond+=(i>>j)&1;
      }
      /* 	printf("i = 0x%X B.Nbond = %i\n",i,B.Nbond); */
      /* check the Nbonds, if it is less than the number of sites - 1 */
      if((B.Nbond<=Nbondmax_input)&&(B.Nbond>=G_Nsite-1)){
	curbond=0;
	for(j=0;j<Bfull.Nbond;j++){
	  if((i>>j)&1){
	    /* bond present */
	    B.bond_x[curbond]=Bfull.bond_x[j];
	    B.bond_y[curbond]=Bfull.bond_y[j];
	    B.bonddir[curbond] = Bfull.bonddir[j];
	    curbond++;

	  }
	}

	if(connectedandspans(B,M)){
	  /* check that it spans all sites */
	  /* and that it is connected */
	  /* 	    printf("\n\nconnected:\n"); */
	  /* 	    displaybgb(B); */

	      
	  /* rotate to a canonical rotation */
	  rotate(&B);
	  key = bgbkeygen(B);
	    
	    
	  /* check for it in the list */
	  if(isfirst){
	    bgbinsert(&first,B,key);
	    hash[key]=first;
	    head=first;
	    isfirst=0;

	  }
	  else{ 
	    if(hash[key]==NULL){
	      /* it isn't in the hash table yet, so put it in */
	      bgbinsert(&head,B,key);
	      hash[key]=head;
		
	      /* 		printf("adding one not it the hash table \n"); */
	      /* 		displaybgb(head->data); */
	    }
	    else{
	      tempnode=findinhashtable(B,key,hash[key]);
	      if(tempnode==NULL){
		/* if it doesn't exist, but shares a key, insert it at hash[key] */
		tempnode=hash[key];
		bgbinsert(&tempnode,B,key);
		/* 		  printf("this one shared a key in the hash table \n"); */
		/* 		  displaybgb(head->data); */
		/* make sure not to chop off the end */	   
		while((head->next)!=NULL)
		  head = head->next;
	      }
	      else{
		/* if it is there, add to latconst */
		  
		/* 		  printf("this one existed in the hash table, so the latconst is being incremented \n"); */
		/* 		  displaybgb(tempnode->data); */
		tempnode->data.latconst++;
	      }
	    }   
	  }
	}
      }
      /* write the list into the appropriate files */
      /* and clear the hash table */

      
      head=first;
      listsize=0;
      while(head!=NULL){

	/* multiply latconst by M.latconst (now that rotations have been accounted for) */
	head->data.latconst = head->data.latconst*M.latconst;
	if(head->data.latconst>6){
	  printf("WARNING!, latconst > 6 (M.latconst=%i) in \n",M.latconst);
	  displaybgb(head->data);
	}

	Bcount[head->data.Nbond]++;
	Bcounttotal[head->data.Nbond]+=head->data.latconst;
	
/* 	printf("writing to file\n"); */
/* 	displaybgb(head->data); */
	
	/* 	/\* compress for writing to the file *\/ */
	/* 	Bsmall=bondgraphbigtosmall(head->data); */

	/* open output files */
	if(outfp[head->data.Nbond]==NULL){
	  /* check if Nbondmax should be increased */
	  if(head->data.Nbond>Nbondmax)
	    Nbondmax=head->data.Nbond;
	  sprintf(filename,"graphs/bondgraphlist%i_%i.dat",head->data.Nbond,G_Nsite);
	  if ((outfp[head->data.Nbond] = fopen(filename, "w"))==NULL){
	    printf("Cannot open file %s for output\n",filename);
	    exit(1);
	  }
	}
     
	
	writebgbtofile(head->data,outfp[head->data.Nbond]);
	
	/* clear hash table */
	hash[head->key]=NULL;
	listsize++;
	head=head->next;
      }    
      bgbfree(first);
      isfirst=1;
      first=NULL;
    }
  }

  
  fclose(infp);
  free(hash);

  /* close output files */
  for(Nbond=1;Nbond<=Nbondmax;Nbond++){
    if(outfp[Nbond]!=NULL){
      fclose(outfp[Nbond]);
    }
  }


  sprintf(filename,"graphs/bondgraph.log");
  if ((logfp = fopen(filename, "a"))==NULL){
    printf("Cannot open file %s for append\n",filename);
    exit(1);
  }
  
  /* display output counts */
  for(Nbond=1;Nbond<=Nbondmax;Nbond++){
    if(Bcount[Nbond]!=0){
      printf("%i %i %i %i\n",Nbond,G_Nsite,Bcount[Nbond],Bcounttotal[Nbond]);
      fprintf(logfp,"%i %i %i %i\n",Nbond,G_Nsite,Bcount[Nbond],Bcounttotal[Nbond]);
    }
  }


  return 0;
}
