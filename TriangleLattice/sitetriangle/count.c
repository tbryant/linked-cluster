#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int G_Nsite;

#include "graph.c"
#include "dlembed.c" 


int compare(const void *p1, const void *p2 ){
  return *(int *)p1 - *(int *)p2;
}

int order(graph *M){
  /* sort sites */
  qsort((void *)(*M).site,G_Nsite,sizeof(int),compare);
  return 0;
}

/* int translate(graph *M){ */
/*   /\* translate sites so that embedding is at CENTER *\/ */
/*   int i,r; */
 
/*   r=CENTER-(*M).site[0]; */
  
/*   for(i=0;i<G_Nsite;i++) */
/*     (*M).site[i]+=r; */
/*   return 0; */
/* } */

int translate(graph *M){
  /* translate sites so that embedding is at lower left (no negative coords) */
  int i,r,lowx,lowy;
 
  lowx = x((*M).site[0]);
  lowy = y((*M).site[0]);
  
  for(i=1;i<G_Nsite;i++){
    if(lowx>x((*M).site[i]))
      lowx = x((*M).site[i]);
    
    if(lowy>y((*M).site[i]))
      lowy = y((*M).site[i]);
  }
  r=p(lowx,lowy);

  for(i=0;i<G_Nsite;i++)
    (*M).site[i]-=r;

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

int hashcheck(graph M,unsigned int key,dlnode *head){
/*   printf("hashcheck "); */
  /* returns 0 if M is in the list, 1 otherwise */
  if(head==NULL){
/*     printf("returns 1 (head NULL)\n"); */
    return 1;
  }

  /* check with all embeddings of the same key */
  while((head!=NULL)&&(head->key==key)){
    if(comparegraphs(M,head->data)==0){
/*       printf("The following two are the same, returning 0\n"); */
/*       display(M); */
/*       display(head->data); */

/*       printf("returns 0 (match)\n"); */
      return 0;    
    }
    head = head->next;
  }
  
/*   printf("returns 1 (no match)\n"); */
  return 1;
}

void rotations(graph *M){
  /* check symmetries of the graph to get lattice constant */
  int i,rot;
  int xc[MAXSITE],yc[MAXSITE];
  graph Mlowest,Mrot,Mrotnew[6];
  int newi=1;

  Mrotnew[0]=(*M);
  Mlowest=(*M);

/*   for(i=0;i<G_Nsite;i++){ */
/*     xc[i]=x((*M).site[i])-x(CENTER); */
/*     yc[i]=y((*M).site[i])-y(CENTER); */
/*   } */

  for(i=0;i<G_Nsite;i++){
    xc[i]=x((*M).site[i]);
    yc[i]=y((*M).site[i]);
  }
  
  for(rot=0;rot<5;rot++){
    /* five (not the identity) symmetries of the 2d triangle */
    switch(rot){
      /* 0: x,y -> -x,-y */
    case 0 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)-xc[i],y(CENTER)-yc[i]);
      break;

      /* 1: x,y -> -y,x-y */
    case 1 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)-yc[i],y(CENTER)+xc[i]-yc[i]);
      break;

      /* 2: x,y ->  y,y-x */
    case 2 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)+yc[i],y(CENTER)+yc[i]-xc[i]);
      break;

      /* 3: x,y -> x-y,x */
    case 3 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)+xc[i]-yc[i],y(CENTER)+xc[i]);
      break;

      /* 4: x,y -> y-x,-x */
    case 4 :
      for(i=0;i<G_Nsite;i++)
	Mrot.site[i]=p(x(CENTER)+yc[i]-xc[i],y(CENTER)-xc[i]);
      break;  
	
    default :	break;
    }
    
    order(&Mrot);
    translate(&Mrot);

    /* look for Mrot in Mrotnew */
    i=0;
    while((i<newi)&&(comparegraphs(Mrot,Mrotnew[i])!=0)){
      i++;
    }
    if(i==newi){
      /* it was not there */
      Mrotnew[i]=Mrot;
      newi++;
    }


    if(comparegraphs(Mlowest,Mrot)>0)
      Mlowest=Mrot;
  }
  /* put the lowest rotation into M */
  (*M)=Mlowest;
  (*M).latconst=newi;

  
}

int main(int argc, char *argv[]){
  dlnode *first=NULL,*tempdlnode;
  dlnode *head;
  graph M,Madd;
  char filename[100];
  FILE *infp, *outfp;
  int Mcount=0,Totalcount=0,keysused=0;
  int isfirst=1;
  int j,k,l;
  int neighborx[6],neighbory[6];
  unsigned int key;
  dlnode **hash;
  

  if(argc!=2){
    printf("count.e Nsite\n");
    exit(0);
  }

  G_Nsite=atoi(argv[1]);

  /* special case for the one site graph */
  if(G_Nsite==1){    
    /* open output file */
    sprintf(filename,"graphs/embedlist%i.dat",G_Nsite);
    if ((outfp = fopen(filename, "w"))==NULL){
      printf("Cannot open file %s for output\n",filename);
      exit(1);
    }
    M.latconst=1;
    M.site[0]=p(0,0);
    Mcount=1;
    /* write to file */
    fwrite(&Mcount,sizeof(int),1,outfp);
    fwrite(&M,sizeof(graph),1,outfp);
    fclose(outfp);
    printf("1 1 1\n");
    return 0;
  }
  

  /*   hash[i] gives the location of the first embedding with key=i */
  if((hash = (dlnode **)calloc(HASHSIZE,sizeof(dlnode *)))==NULL){
    printf("unable to allocate memory for hash table\n");
    exit(0);
  }
  
  
  /* open files */
  sprintf(filename,"graphs/embedlist%i.dat",G_Nsite-1);
  if ((infp = fopen(filename, "r"))==NULL){
    printf("Cannot open file %s for input\n",filename);
    exit(1);
  }
  fread(&Mcount,sizeof(int),1,infp);
/*   printf("Adding to %i %i site graphs\n",Mcount,G_Nsite-1); */

  sprintf(filename,"graphs/embedlist%i.dat",G_Nsite);
  if ((outfp = fopen(filename, "w"))==NULL){
    printf("Cannot open file %s for output\n",filename);
    exit(1);
  }

  /* 6 neighbors for a triangle */
  neighborx[0]=1;
  neighbory[0]=0;

  neighborx[1]=0;
  neighbory[1]=1;

  neighborx[2]=-1;
  neighbory[2]=0;

  neighborx[3]=0;
  neighbory[3]=-1;

  neighborx[4]=1;
  neighbory[4]=1;

  neighborx[5]=-1;
  neighbory[5]=-1;

  Mcount=0;
  while(fread(&Madd,sizeof(graph),1,infp)){
    /* add one site to existing embedding  */
    for(j=0;j<G_Nsite-1;j++){/* cycle through each site  */     
      for(k=0;k<6;k++){/* add each of six neighbors */
/* 	printf("j=%i,k=%i\n",j,k); */
	
	/* copy existing sites shifted up by (1,1) (so that the new neighbor doesn't have a negative coordinate)*/
	for(l=0;l<G_Nsite-1;l++){
	  M.site[l]=Madd.site[l]+p(1,1);
	}  
	/* add neighbor (again, shifted by (1,1)*/
	M.site[l]=p(x(Madd.site[j])+1+neighborx[k],y(Madd.site[j])+1+neighbory[k]);
	
	/* check for duplicate sites */
	if(noduplicates(M)){
	  
	  /* now we need to check the candidate embedding */
	  /* first put it in order */
	  order(&M);	
	  
	  /* then move to the lower left */
	  translate(&M);
	  
	  /* get latconst */
	  rotations(&M);
	  
	  /* generate key */
	  key=keygen(M);
	

	  /* if it is the first one, initialize the list */
	  if(isfirst){
	    isfirst=0;
	    dlinsert(&first,M);
	    head=first;
	    head->key=key;
	    hash[key]=head;
	    keysused++;
	    Mcount++;
/* 	    printf("%i,first graph\n",key); */
/* 	    display(M); */
	  }
	  else{
	    /* check if it is already in the list */
	    if(hash[key]==NULL){
	      /* if the hash table is empty, append to the end */
	      dlinsert(&head,M);
	      head->key=key;
	      Mcount++;
	      hash[key]=head;
	      keysused++;
/* 	      printf("%i, new to the hash table\n",key); */
/* 	      display(M); */
	    }
	    else if(hashcheck(M,key,hash[key])==1){
	      /* if it doesn't exist, but shares a key, insert it at hash[key] */
	      tempdlnode = hash[key];
	      dlinsert(&tempdlnode,M);
	      tempdlnode->key=key;
	      Mcount++;	 
	      /* make sure not to chop off the end */
	      while((head->next)!=NULL) 
		head = head->next;
/* 	      printf("%i another graph has the same key\n",key); */
/* 	      display(M); */
	    }
	  }
	}
      }
    }
  }	 

  /* write out to outfp */
  fwrite(&Mcount,sizeof(int),1,outfp);
  head = first;
  
  
  while(head != NULL){
    Totalcount+=head->data.latconst;
    fwrite(&(head->data),sizeof(graph),1,outfp);
    head=head->next;
  }

  dlfree(first);
  
  fclose(infp);
  fclose(outfp);

  printf("%i %i %i %f%%\n",G_Nsite,Mcount,Totalcount,100*(float)keysused/Mcount);

  return 0;
}
