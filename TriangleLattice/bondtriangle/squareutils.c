/* MAXSITE defines the struct size (must be big enough for the max) */
#define MAXSITE 16
/* MAXL is used for the bond coordinates x+MAXL*y*/
#define MAXL 100
/* CENTER is the location of the 0th element */
#define CENTER p(20,20)

typedef struct{
  int latconst;
  int site[MAXSITE];
} graph;


int p(int x, int y){
  return x+MAXL*y;
}

int x(int p){
  return p%MAXL;
}

int y(int p){
  return p/MAXL;
}


#define SHOWSIZE 10
int display(graph M){
  int j,k,l;
  int xc,yc;
  int show[SHOWSIZE][SHOWSIZE];

  printf("Lattice Constant: %i \n",M.latconst);
  
  printf("Sites:\n");
    for(k=0;k<SHOWSIZE;k++){
      for(l=0;l<SHOWSIZE;l++){
	show[k][l]=-1;
      }
    }
    for(j=0;j<G_Nsite;j++){
      printf("%i:(%i,%i)\n",j,x(M.site[j]),y(M.site[j]));

      xc = x(M.site[j]);
      yc = y(M.site[j]);
      
      show[xc][yc]=j;
    }
    for(k=SHOWSIZE-1;k>=0;k--){
      for(j=0;j<SHOWSIZE;j++){
	if(show[j][k]==-1)
	  printf("  ");
	else printf("%2i",show[j][k]);      
      }
      printf("\n");
    }
    printf("\n");
    return 0;
}
typedef struct{
  char xNbond_ylatconst;
  /* bond dir stores the orientation for the bond */
  /* one bit per, 0 for horizontal, 1 for vertical */
  int bonddir;
  /* bond stores the site coordinate for the left or lower site of the bond */
  char bond[MAXSITE-1];
} bondgraph;

typedef struct{
  int Nbond;
  int latconst;
  /* bond dir stores the orientation for the bond */
  /* one bit per, 0 for horizontal, 1 for vertical */
  int bonddir;
  /* bond stores the site coordinate for the left or lower site of the bond */
  int bond_x[2*MAXSITE];
  int bond_y[2*MAXSITE];
} bondgraphbig;

char makebond(int x,int y){
  int temp;
  if((x>15)||(y>15)){
    printf("error!! makebond(%i,%i), coord greater than 15\n",x,y);
    exit(0);
  }
  temp = ((y&0x0f)<<4)+(x&0x0f);

  return (char)temp;
}

int bondx(char b){
  return ((int)b)&0x0f;  
}

int bondy(char b){
  return (((int)b)>>4)&0x0f;  
}

bondgraphbig smalltobgb(bondgraph B){
  bondgraphbig Bbig;
  int i;

  Bbig.Nbond = bondx(B.xNbond_ylatconst);
  Bbig.latconst = bondy(B.xNbond_ylatconst);
  Bbig.bonddir = B.bonddir;
  for(i=0;i<Bbig.Nbond;i++){
    Bbig.bond_x[i]=bondx(B.bond[i]);   
    Bbig.bond_y[i]=bondy(B.bond[i]);
  }
  return Bbig;
}

bondgraph bondgraphbigtosmall(bondgraphbig Bbig){
  bondgraph B;
  int i;

  B.xNbond_ylatconst=makebond(Bbig.Nbond,Bbig.latconst);
  B.bonddir=Bbig.bonddir;
  for(i=0;i<Bbig.Nbond;i++){
    B.bond[i]=makebond(Bbig.bond_x[i],Bbig.bond_y[i]);
  }
  return B;
}


int displaybgb(bondgraphbig B){
  int j,k,l;
  int Nbond,xc,yc,bdir;
  int show[SHOWSIZE][SHOWSIZE];

  Nbond = B.Nbond;
  printf("Nbond: %i\n",Nbond);
  printf("Lattice Constant: %i \n",B.latconst);
  
  printf("Bonds:\n");
  for(k=0;k<SHOWSIZE;k++){
    for(l=0;l<SHOWSIZE;l++){
      show[k][l]=-1;
    }
  }
  for(j=0;j<Nbond;j++){
    
    xc = B.bond_x[j];
    yc = B.bond_y[j];
    bdir = (B.bonddir>>j)&1;
    
    printf("%i:(%i,%i) dir %i\n",j,xc,yc,bdir);
    
    if(bdir==0)
      show[2*xc+1][2*yc]='-';
    else
      show[2*xc][2*yc+1]='|';
/*     if(bdir==0) */
/*       show[2*xc+1][2*yc]=j; */
/*     else */
/*       show[2*xc][2*yc+1]=j; */
  }
  for(k=SHOWSIZE-1;k>=0;k--){
    for(j=0;j<SHOWSIZE;j++){
      if(show[j][k]==-1)
	printf(" ");
      else{
	printf("%c",(char)show[j][k]);
      }
    }
    printf("\n");
  }
  printf("\n");
  return 0;   
}

void displaybondgraph(bondgraph B){
  displaybgb(smalltobgb(B));
}


/* int displaybondgraph(bondgraph B){ */
/*   int j,k,l; */
/*   int Nbond,xc,yc,bdir; */
/*   int show[SHOWSIZE][SHOWSIZE]; */

/*   Nbond = bondx(B.xNbond_ylatconst); */
/*   printf("Nbond: %i\n",Nbond); */
/*   printf("Lattice Constant: %i \n",bondy(B.xNbond_ylatconst)); */
  
/*   printf("Bonds:\n"); */
/*   for(k=0;k<SHOWSIZE;k++){ */
/*     for(l=0;l<SHOWSIZE;l++){ */
/*       show[k][l]=-1; */
/*     } */
/*   } */
/*   for(j=0;j<Nbond;j++){ */
    
/*     xc = bondx(B.bond[j]); */
/*     yc = bondy(B.bond[j]); */
/*     bdir = (B.bonddir>>j)&1; */

/*     printf("%i:(%i,%i) dir %i\n",j,xc,yc,bdir); */
    
/*     if(bdir==0) */
/*       show[2*xc+1][2*yc]=j; */
/*     else */
/*       show[2*xc][2*yc+1]=j; */
/*   } */
/*   for(k=SHOWSIZE-1;k>=0;k--){ */
/*     for(j=0;j<SHOWSIZE;j++){ */
/*       if(show[j][k]==-1) */
/* 	printf("  "); */
/*       else{ */
/* 	printf("%2i",show[j][k]); */
/*       } */
/*     } */
/*     printf("\n"); */
/*   } */
/*   printf("\n"); */
/*   return 0; */
/* } */

int comparebond(int x1,int y1,int d1,int x2,int y2,int d2){
  /* comparebond returns 1 for bond1>bond2 ; -1 for bond1<bond2 ; 0 for bond1==bond2  */
  /* to compare a bond */
  /* first check y coord */
  /* then check x coord */
  /* then check dir */
  if(y1>y2){
    /* bond1>bond2 */
    return 1;
  }
  else{
    if(y1==y2){
      if(x1>x2){
	/* bond1>bond2 */
	return 1;
      }
      else{
	if(x1==x2){
	  if(d1>d2){
	    /* bond1>bond2 */
	    return 1;
	  }
	  else{
	    if(d1==d2){
	      /* bond1==bond2 */
	      return 0;
	    }
	  }
	}
      }
    }
  }
  /* bond1<bond2 */
  return -1;
}

void order(bondgraphbig *B){
  /* sort bonds */
  int used[2*MAXSITE];
  int newbonddir=0;
  int new_x[2*MAXSITE],new_y[2*MAXSITE];
  int i,loopcount;
  int low_x,low_y,lowdir,lowi;
  
  for(i=0;i<(*B).Nbond;i++)
    used[i]=0;
  
  /* go through bonds remaining */
  /* and pick the lowest */
  loopcount=0;
  while(loopcount<(*B).Nbond){
    /* start off low_x and low_y at 100,100 */
    /* so that it is always higher than any bond */
    low_x = 100;
    low_y = 100;
    lowdir=1;
    
    for(i=0;i<(*B).Nbond;i++){
      if(used[i]==0){
	/* compare bonds */
	if(comparebond(low_x,low_y,lowdir,(*B).bond_x[i],(*B).bond_y[i],(((*B).bonddir>>i)&1))==1){
	  /* new low bond */
	  low_x=(*B).bond_x[i];
	  low_y=(*B).bond_y[i];
	  lowdir=(((*B).bonddir>>i)&1);
	  lowi=i;
	}

/* 	/\* first check with y coord *\/ */
/* 	if(low_y>(*B).bond_y[i]){ */
/* 	  /\* new low *\/ */
/* 	  low_x=(*B).bond_x[i]; */
/* 	  low_y=(*B).bond_y[i]; */
/* 	  lowi=i; */
/* 	} */
/* 	else{ */
/* 	  if(low_y==(*B).bond_y[i]){ */
/* 	    /\* same y coord, so check x *\/ */
/* 	    if(low_x>(*B).bond_x[i]){ */
/* 	      /\* new low *\/ */
/* 	      low_x=(*B).bond_x[i]; */
/* 	      low_y=(*B).bond_y[i]; */
/* 	      lowi=i; */
/* 	    } */
/* 	  } */
/* 	} */
      }
    }
    /* put lowi into loopcount's spot  */
    used[lowi]=1;
    newbonddir+=(((*B).bonddir>>lowi)&1)<<loopcount;
    new_x[loopcount]=(*B).bond_x[lowi];
    new_y[loopcount]=(*B).bond_y[lowi];
    loopcount++;
  }
  /* put new into B */
  (*B).bonddir = newbonddir;
  for(i=0;i<(*B).Nbond;i++){
    (*B).bond_x[i] =  new_x[i];
    (*B).bond_y[i] =  new_y[i];
  }
}

void translate(bondgraphbig *B){
  /* translate sites so that embedding is at lower left (no negative coords) */
  int i,lowx,lowy;
 
  lowx = (*B).bond_x[0];
  lowy = (*B).bond_y[0];
  
  for(i=1;i<(*B).Nbond;i++){
    if(lowx > (*B).bond_x[i])
      lowx = (*B).bond_x[i];
    
    if(lowy > (*B).bond_y[i])
      lowy = (*B).bond_y[i];
  }

  for(i=0;i<(*B).Nbond;i++){
    (*B).bond_x[i] -= lowx;
    (*B).bond_y[i] -= lowy;
  }
}


int comparebgb(bondgraphbig B1, bondgraphbig B2){
  /* returns >0 if B1 is bigger, <0 if B2 is bigger, 0 if they are identical */
  int i=0;
  /* check Nbond */
  if(B1.Nbond!=B2.Nbond)
    return B1.Nbond-B2.Nbond;
  

  /* check bonds */
  while((i<B1.Nbond)&&(comparebond(B1.bond_x[i],B1.bond_y[i],((B1.bonddir>>i)&1),B2.bond_x[i],B2.bond_y[i],((B2.bonddir>>i)&1))==0)){
    i++;
    /* if they match */
    if(i==B1.Nbond){
/*       printf("MATCH!\n"); */
/*       displaybgb(B1); */
/*       displaybgb(B2); */
      return 0;  
    }
  }
/*   printf("No match - failed bond %i\n, returning %i\n",i,comparebond(B1.bond_x[i],B1.bond_y[i],((B1.bonddir>>i)&1),B2.bond_x[i],B2.bond_y[i],((B2.bonddir>>i)&1))); */
/*   displaybgb(B1); */
/*   displaybgb(B2); */

  return comparebond(B1.bond_x[i],B1.bond_y[i],((B1.bonddir>>i)&1),B2.bond_x[i],B2.bond_y[i],((B2.bonddir>>i)&1));
 }


void rotate(bondgraphbig *B){
  /* rotate to the lowest rotation (defined by comparebgb) */
  /* check symmetries of the graph to get lattice constant */
  int i,rot;
  bondgraphbig Blowest,Brot;
  
  Blowest=(*B);

/*   printf("before rot\n"); */
/*   displaybgb(*B); */

  for(rot=0;rot<7;rot++){
    /* copy Nbond and latconst, etc. */
    Brot=(*B);
  
/*     printf("before rot %i\n",rot); */
/*     displaybgb(Brot); */
  
    /* seven (not the identity) symmetries of the 2d square */
    switch(rot){
      /* 0: x,y -> -x,y */
      /* bonds to not change their type */
      /* horizontal bonds are now associated with 1 site to the left */
    case 0 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_x[i] = -(*B).bond_x[i];

	if(((Brot.bonddir>>i)&1)==0)	  
	  Brot.bond_x[i] -= 1;
      }
      break;

      /* 1: x,y -> x,-y */
      /* bonds to not change their type */
      /* vertical bonds are now associated with 1 site down */
    case 1 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_y[i] = -(*B).bond_y[i];

	if(((Brot.bonddir>>i)&1)==1)	  
	  Brot.bond_y[i] -= 1;
      }
      break;

      /* 2: x,y -> -x,-y */
      /* bonds to not change their type */
      /* horizontal bonds are now associated with 1 site to the left */
      /* vertical bonds are now associated with 1 site down */
    case 2 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_x[i] = -(*B).bond_x[i];
	Brot.bond_y[i] = -(*B).bond_y[i];

	if(((Brot.bonddir>>i)&1)==0)	  
	  Brot.bond_x[i] -= 1;
	else
	  Brot.bond_y[i] -= 1;
      }
      break;

      /* 3: x,y -> y,x */
      /* bonds change type */
    case 3 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_x[i] = (*B).bond_y[i];
	Brot.bond_y[i] = (*B).bond_x[i];

	/* swap ith direction */
	Brot.bonddir=Brot.bonddir^(1<<i);
      }
      break;
      /* 4: x,y -> -y,x */
      /* bonds change type */
      /* new vertical bonds are now associated with 1 site down */
    case 4 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_x[i] = (*B).bond_y[i];
	Brot.bond_y[i] = -(*B).bond_x[i];

	/* swap ith direction */
	Brot.bonddir=Brot.bonddir^(1<<i);

	if(((Brot.bonddir>>i)&1)==1)	  
	  Brot.bond_y[i] -= 1;
      }
      break;

      /* 5: x,y -> y,-x */
      /* bonds change type */
      /* new horizontal bonds are now associated with 1 site to the left */
    case 5 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_x[i] = -(*B).bond_y[i];
	Brot.bond_y[i] = (*B).bond_x[i];

	/* swap ith direction */
	Brot.bonddir=Brot.bonddir^(1<<i);

	if(((Brot.bonddir>>i)&1)==0)	  
	  Brot.bond_x[i] -= 1;
      }
      break;


      /* 6: x,y -> -y,-x */
      /* bonds change type */
      /* new horizontal bonds are now associated with 1 site to the left */
      /* new vertical bonds are now associated with 1 site down */
    case 6 :
      for(i=0;i<(*B).Nbond;i++){
	Brot.bond_x[i] = -(*B).bond_y[i];
	Brot.bond_y[i] = -(*B).bond_x[i];

	/* swap ith direction */
	Brot.bonddir=Brot.bonddir^(1<<i);

	if(((Brot.bonddir>>i)&1)==0)	  
	  Brot.bond_x[i] -= 1;
	else
	  Brot.bond_y[i] -= 1;
      }
      break;
      
    default :	break;
    }

/*     printf("after rot %i\n",rot); */
/*     displaybgb(Brot); */

    order(&Brot);

/*     printf("after order\n"); */
/*     displaybgb(Brot); */

    translate(&Brot);

/*     printf("after translate, rot %i\n",rot); */
/*     displaybgb(Brot); */

    if(comparebgb(Blowest,Brot)>0){
/*       printf("new lowest rotation\n"); */
      Blowest=Brot;
    }
  }
  /* put the lowest rotation into B */
  (*B)=Blowest;
}
#define MAXBOND 16

typedef struct{
  int length;
  int ishair;
  int site[MAXBOND];
} bridgetype;

typedef struct{
  int topo;
  bondgraphbig embed;
  int Nbond;
  int Nsite;
  int Nnode;
  int Nbridge;
  int latconst;
  bridgetype bridge[2*MAXBOND]; 
  int topolabel[MAXBOND+1];
  int embedlabel[MAXBOND+1];
  int site_x[MAXBOND+1];
  int site_y[MAXBOND+1];
  int left[MAXBOND];
  int right[MAXBOND];
  int SA[MAXBOND][MAXBOND+1];
} topo;

typedef struct{
  int parenttopo;
  int Nbond;
  int topo;
  int Nsite;
  int topositemap[MAXBOND+1];
} subtopobig;


typedef struct{
  int parenttopo;
  int topo;
  char x_Nbond_y_Nsite;
  char topositemap[MAXBOND+1];
} subtoposmall;

int **int_2d(int lx, int ly){
  /* int_2d allocates memory for a 2d array of ints */
  int **dx;
  int x;
  dx = (int **)malloc(sizeof(int *)*lx);
  for (x = 0; x < lx; x++) {
    dx[x] = (int *)malloc(sizeof(int)*ly);
  }
  return dx;
}

void freeint_2d(int **dx, int lx){
  int x;
  for (x = 0; x < lx; x++) {
    free(dx[x]);
  }
  free(dx);
}

subtoposmall subtopobigtosmall(subtopobig Sbig){
  subtoposmall Ssmall;
  int i;

  Ssmall.parenttopo = Sbig.parenttopo;
  Ssmall.topo = Sbig.topo;
  Ssmall.x_Nbond_y_Nsite = makebond(Sbig.Nbond,Sbig.Nsite);
  
  for(i=0;i<Sbig.Nsite;i++){
    Ssmall.topositemap[i]= (char)(Sbig.topositemap[i]);
  }
  return Ssmall;
}


subtopobig subtoposmalltobig(subtoposmall Ssmall){
  subtopobig Sbig;
  int i;

  Sbig.parenttopo = Ssmall.parenttopo;
  Sbig.topo = Ssmall.topo;
  Sbig.Nbond = bondx(Ssmall.x_Nbond_y_Nsite);
  Sbig.Nsite = bondy(Ssmall.x_Nbond_y_Nsite);
  
  for(i=0;i<Sbig.Nsite;i++){
    Sbig.topositemap[i]= (int)Ssmall.topositemap[i];
  }
  
  return Sbig;
}

void displaysubtopobig(subtopobig S){
  int i;
  printf("%i %i:%i ",S.parenttopo,S.Nbond,S.topo);
  for(i=0;i<S.Nsite;i++){
     printf("%i ",S.topositemap[i]);
  }
  printf("\n");
}

int new(int cand, int *site, int k){
  int i;
  for(i=0;i<k;i++){
    if(cand==site[i])
      return 0;			/* false */
  }
  return 1;			/* true */
}

int addbridge(int l,int old){
  int i;
  for(i=0;i<4;i++){
    if(l>(int)(old/pow(100,i))%100)
      return 100*(old-old%(int)pow(100,i)) + l*pow(100,i) + old%(int)pow(100,i);
  }
  printf("Error: addbridge(%i,%i) failed\n",l,old);
  return 0;
}

int getbridgefromSA(int big, int n){
  return (big/(int)pow(100,n))%100;
}

int labelbridge(topo *T,int l, int start, int end,int sitevar){
  int i=0;
  int j;
  
  while(i<(*T).Nbridge){
    
    if(l==(*T).bridge[i].length){
      if(((*T).bridge[i].site[0]==start)&&((*T).bridge[i].site[l]==end)){
	for(j=1;j<l;j++){
	  (*T).embedlabel[sitevar]=(*T).bridge[i].site[j];
	  sitevar++;
	}
	(*T).bridge[i].site[0]=-1; /* effectively remove bridge from list */
	return sitevar;
      }
    }
    
    i++;
  }
  return sitevar;
}

int labelhair(topo *T,int l, int start,int sitevar){
  int i=0;
  int j;

  while(i<(*T).Nbridge){
    if((*T).bridge[i].ishair){
      if(l==(*T).bridge[i].length){
	if((*T).bridge[i].site[0]==start){
	  for(j=1;j<=l;j++){
	    (*T).embedlabel[sitevar]=(*T).bridge[i].site[j];
	    sitevar++;
	  }
	  (*T).bridge[i].site[0]=-1; /* effectively remove bridge from list */
	}
      }
    }
    i++;
  }
  return sitevar;
}

int swap(int *plist,int j,int k){
  int temp;
  temp = plist[j];
  plist[j] = plist[k];
  plist[k] = temp;
  return 0;
}


int nextplist(int *plist,int Nnode){
  int j=Nnode-1;
  int k=Nnode-2;
  int r,s;
 
  if(Nnode<2)
    return 0;

  /* point to location of first that can swap */
  while(plist[k]>plist[k+1]){
    k--;
    if(k<0)
      return 0;
  }

  /* swap and copy the rest */
  while(plist[k]>plist[j]){
    j--;
  }

  swap(plist,j,k);
  r=Nnode-1;
  s=k+1;
  
  while(r>s){
    swap(plist,r,s);
    r--;
    s++;
  }
  return 1;
}


int issameSA(topo T1,topo T2){
  /* first check nodes */
  int i,j;
  if(T1.Nnode!=T2.Nnode)
    return 0;
  
  if(T1.SA[0][0]!=T2.SA[0][0])
    return 0;

  for(i=0;i<T1.Nnode;i++){
    for(j=0;j<T1.Nnode+1;j++){
      if(T1.SA[i][j]!=T2.SA[i][j])
	return 0;
    }
  }
  return 1;
}

int displaySA(int **SA,int Nnode){
  int j,k;
  for(k=0;k<Nnode;k++){
    for(j=0;j<Nnode+1;j++){
      printf("%7i ",SA[k][j]);      
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}

/* int displaysubtopo(subtopo S){ */
/*   int j,k; */

/*   printf("Parent Topology=%i\n",S.parent); */
/*   printf("Topology=%i\n",S.topo); */
/*   printf("Nsite=%i\n",S.Nsite); */
/*   printf("Nnode=%i\n",S.Nnode); */
   
/*   printf("Mapping to sites of parent:\n"); */
/*   for(k=0;k<S.Nsite;k++){ */
/*     printf("parentsite[%i]=%i\n",k,S.parentsite[k]); */
/*   }  */

 
/*   for(k=0;k<MAXBOND+1;k++){ */
/*     printf("invparentsite[%i]=%i\n",k,S.invparentsite[k]); */
/*   }  */

/*   printf("Star Adjacency Matrix:\n"); */
/*   if(S.Nnode==0){ */
/*     if(S.SA[0][0]<0) */
/*       printf("line, length %i\n",-S.SA[0][0]); */
/*     else */
/*       printf("loop, length %i\n",S.SA[0][0]); */
/*   } */
/*   else{ */
/*     for(k=0;k<S.Nnode;k++){ */
/*       for(j=0;j<S.Nnode+1;j++){ */
/* 	printf("%7i ",S.SA[k][j]);       */
/*       } */
/*       printf("\n"); */
/*     } */
/*     printf("\n"); */
/*   } */
/*   return 0; */
/* }    */


int displaytopo(topo T){
  int j,k;

  printf("Topo %i :",T.topo);
  printf("Nbond=%i,",T.Nbond);
  printf("Nsite=%i,",T.Nsite);
  printf("Nnode=%i,",T.Nnode);
  printf("Nbridge=%i,",T.Nbridge);
  printf("latconst=%i\n",T.latconst);
  printf("Site: (xcoord,ycoord)\n");
  for(j=0;j<T.Nsite;j++){
    printf("%i: (%i,%i) \n",j,T.site_x[j],T.site_y[j]);
  }

  printf("Bond: left site,right site\n");
  for(j=0;j<T.Nbond;j++){
    printf("%i: %2i,%2i \n",j,T.left[j],T.right[j]);
  }

  for(k=0;k<T.Nbridge;k++){
    if(T.bridge[k].site[0]!=-1){
      printf("Bridge %i,length %i:\n",k,T.bridge[k].length);
      for(j=0;j<=T.bridge[k].length;j++)
	printf("%i ",T.bridge[k].site[j]);
      printf("\n");
    }
  }
  
  printf("mapping of sites to lexicographic ordering\n");
  for(k=0;k<T.Nsite;k++){
    printf("topolabel[%i]=%i\n",k,T.topolabel[k]);
  }
/*   for(k=0;k<T.Nsite;k++){ */
/*     printf("embedlabel[%i]=%i\n",k,T.embedlabel[k]); */
/*   } */
  printf("Star Adjacency Matrix:\n");
  if(T.Nnode==0){
    if(T.SA[0][0]<0)
      printf("line, length %i\n",-T.SA[0][0]);
    else
      printf("loop, length %i\n",T.SA[0][0]);
  }
  else{
    for(k=0;k<T.Nnode;k++){
      for(j=0;j<T.Nnode+1;j++){
	printf("%7i ",T.SA[k][j]);      
      }
      printf("\n");
    }
    printf("\n");
  }

  displaybgb(T.embed);

  return 0;
}   


int ordernodes(int SA[MAXBOND][MAXBOND+1],int Nnode,int *sitemap){
  int sitemapnew[MAXBOND],sitemapold[MAXBOND];
  int i,j,k,l,done;
  int plist[MAXBOND];
  int SAnew[MAXBOND][MAXBOND+1],SAold[MAXBOND][MAXBOND+1];
   
  /* copy original into SAold */

  for(k=0;k<Nnode;k++){
    for(l=0;l<Nnode+1;l++){
      SAold[k][l]=SA[k][l];
    }
    sitemapold[k]=sitemap[k];
  }
  
  /* initialize plist to identity */
  for(i=0;i<Nnode;i++){
    plist[i]=i;
  }
  
  while(nextplist(plist,Nnode)){
    /* permute the nodes */
    for(k=0;k<Nnode;k++){
      for(l=0;l<Nnode;l++){
	SAnew[plist[k]][plist[l]]=SAold[k][l];
      }
      SAnew[plist[k]][Nnode]=SAold[k][Nnode];
      sitemapnew[plist[k]]=sitemapold[k];
    }

    /* if new is smaller than SA, copy SAnew into SA */
    done=0;
    for(i=0;i<Nnode;i++){
      for(j=0;j<Nnode+1;j++){
	if(done==0){
	  if(SA[i][j]!=SAnew[i][j]){
	    if(SA[i][j]>SAnew[i][j]){	    
	      /* copy SAnew into SA */
	      for(k=0;k<Nnode;k++){
		for(l=0;l<Nnode+1;l++){
		  SA[k][l]=SAnew[k][l];
		}
		sitemap[k]=sitemapnew[k];
	      }
	    }
	    /* stop checking when we're done */
	    done=1;
	  }
	}
      }
    }
  }
  return 0;
}

  
topo topology(bondgraphbig B){
  topo T;
  int SAtemp[MAXBOND][MAXBOND+1];
  int i,j,k,l,n;
  int newbridge;
  int isloop=1;
  int bondvar,sitevar;
  int valence[MAXBOND+1];
  int node[MAXBOND+1];
  int site[MAXBOND+1];
  int sitec=0;
  int sitebonds[MAXBOND+1][5];

  /* copy embedding into topo */
  T.Nbond = B.Nbond;
  T.latconst = B.latconst;
  T.embed = B;

  /* init valence */
  for(i=0;i<=B.Nbond;i++){
    valence[i]=0;
  }

  /* label the sites */
  /* build left and right */
  /* and fill in valence */
  /* and sitebonds */
  for(i=0;i<B.Nbond;i++){
    /* left site of bond */
    T.site_x[sitec]=B.bond_x[i];
    T.site_y[sitec]=B.bond_y[i]; 
    
    /* check that the site isn't already labeled */
    j=0;
    while((j<sitec)&&((T.site_x[j]!=T.site_x[sitec])||(T.site_y[j]!=T.site_y[sitec]))){
      j++;
    }
    if(j==sitec){
      /* new site */
      T.left[i]=j;
      valence[j]=1;
      sitebonds[j][0]=1;
      sitebonds[j][sitebonds[j][0]]=i;
      sitec++;
    }
    else{
      /* site already exits */
      T.left[i]=j;
      valence[j]++;
      sitebonds[j][0]++;
      sitebonds[j][sitebonds[j][0]]=i;
    }

    /* right site of bond */
    if(((B.bonddir>>i)&1)==0){	  
      T.site_x[sitec] = B.bond_x[i]+1;
      T.site_y[sitec] = B.bond_y[i]; 
    }      
    else{
      T.site_x[sitec]=B.bond_x[i];
      T.site_y[sitec]=B.bond_y[i]+1; 
    }
    
    /* check that the site isn't already labeled */
    j=0;
    while((j<sitec)&&((T.site_x[j]!=T.site_x[sitec])||(T.site_y[j]!=T.site_y[sitec]))){
      j++;
    }
    if(j==sitec){
      /* new site */
      T.right[i]=j;
      valence[j]=1;
      sitebonds[j][0]=1;
      sitebonds[j][sitebonds[j][0]]=i;
      sitec++;
    }
    else{
      /* site already exits */
      T.right[i]=j;
      valence[j]++;
      sitebonds[j][0]++;
      sitebonds[j][sitebonds[j][0]]=i;
    }
  }


  /* set Nsite */
  T.Nsite=sitec;
    
        
  /* count number of nodes */
  k=0;
  for(j=0;j<T.Nsite;j++){
    if(valence[j]>2){
      site[k]=j;
      T.embedlabel[k]=j;
      node[j]=k;
      k++;
    }
    if(valence[j]!=2)
      isloop=0;
  }
    
  /* set Nnode */
  T.Nnode=k;
 
  /* init SA */
  for(j=0;j<T.Nnode;j++){
    for(k=0;k<T.Nnode+1;k++){
      SAtemp[j][k]=0;
    }
  }
    
  /* Calculate Star Adjacency matrix */
       
  /* check for special cases */
  /* Nnode = 0 */
  /* if there are no free ends, SA[0][0] has +length*/
  /* if there are, SA[0][0] has -length */  
  
  n=0;			/* n is the bridge index */
  if(T.Nnode==0){
    if(isloop)
      SAtemp[0][0]=T.Nbond;
    else
      SAtemp[0][0]=-T.Nbond;
  }
  else{
      
    /* set up SA */
    for(j=0;j<T.Nnode;j++){
      /* j is the count of the node */
      for(k=0;k<sitebonds[site[j]][0];k++){
	/* k is the count of the bonds connecting to site[j] */
	bondvar = sitebonds[site[j]][k+1];
	  
	T.bridge[n].ishair = 0; /* initialize ishair to false */
	T.bridge[n].length = 1;
	T.bridge[n].site[0] = site[j];
	  

	if(T.left[bondvar]==site[j])
	  sitevar = T.right[bondvar];
	else
	  sitevar = T.left[bondvar];

	T.bridge[n].site[1] = sitevar;
	  
	while(valence[sitevar]==2){
	  if(sitebonds[sitevar][1]==bondvar)
	    bondvar = sitebonds[sitevar][2];
	  else
	    bondvar = sitebonds[sitevar][1];
	      
	   
	  if(T.left[bondvar]==sitevar)
	    sitevar = T.right[bondvar];
	  else
	    sitevar = T.left[bondvar];
	  T.bridge[n].length++;
	  T.bridge[n].site[T.bridge[n].length] = sitevar;
	    
	}
	
	if(valence[sitevar]==1){
	  /* bridge to infinity */
	  SAtemp[j][T.Nnode]= addbridge(T.bridge[n].length,SAtemp[j][T.Nnode]);
	  T.bridge[n].ishair = 1;
	  n++;
	}
	else if(sitevar==site[j]){
	  /* bridge to same node (avoid double counting) */
	  i=0;
	  newbridge=1;
	  while(i<n){
    	    if(T.bridge[i].length==T.bridge[n].length){
	      if((T.bridge[i].site[0]==site[j])&&(T.bridge[i].site[T.bridge[i].length]==site[j])){
		if(T.bridge[i].site[T.bridge[i].length-1]==T.bridge[n].site[1]){
		  /* already on the list */
		  newbridge=0;
		}
	      }
	    }
	    i++;
	  }
	  if(newbridge){
	    SAtemp[j][node[sitevar]]= addbridge(T.bridge[n].length,SAtemp[j][node[sitevar]]);
	    n++;
	  }
	}
	else{
	  /* bridge to another node */
	  SAtemp[j][node[sitevar]]= addbridge(T.bridge[n].length,SAtemp[j][node[sitevar]]);
	  n++;
	}
      }
    }
  }
    
  T.Nbridge = n;
  
      
  ordernodes(SAtemp,T.Nnode,T.embedlabel);

  /* copy SAtemp into T*/
  T.SA[0][0]=SAtemp[0][0];
  for(j=0;j<T.Nnode;j++){
    for(k=0;k<T.Nnode+1;k++){
      T.SA[j][k]=SAtemp[j][k];
    }
  }



  /* generate mapping of sites(T.embedlabel) from SA */
  if(T.Nnode==0){
    if(isloop){
      /* crawl through bonds keep 0 -> 0 */
      T.embedlabel[0]=0;
      sitevar=0;
      bondvar = sitebonds[sitevar][1];
      for(j=1;j<T.Nsite;j++){
	
	if(sitebonds[sitevar][1]==bondvar)
	  bondvar = sitebonds[sitevar][2];
	else
	  bondvar = sitebonds[sitevar][1];

	if(T.left[bondvar]==sitevar)
	  sitevar = T.right[bondvar];
	else
	  sitevar = T.left[bondvar];  
	
	
	T.embedlabel[j]=sitevar;
      }
    }
      
    
    else{
      /* crawl through bonds ,  site 0 has valence = 1 */
      
      sitevar=0;
      while(valence[sitevar]!=1)
	sitevar++;

      T.embedlabel[0]=sitevar;
      bondvar = sitebonds[sitevar][1];

      if(T.left[bondvar]==sitevar)
	sitevar = T.right[bondvar];
      else
	sitevar = T.left[bondvar];  
	     
      T.embedlabel[1]=sitevar;

      for(j=2;j<T.Nsite;j++){
	
	if(sitebonds[sitevar][1]==bondvar)
	  bondvar = sitebonds[sitevar][2];
	else
	  bondvar = sitebonds[sitevar][1];

	if(T.left[bondvar]==sitevar)
	  sitevar = T.right[bondvar];
	else
	  sitevar = T.left[bondvar];  
	
	
	T.embedlabel[j]=sitevar;
      }
    }
  }

  else{
	
    
    /* nodes already assigned, so start at Nnode */
    sitevar=T.Nnode;
    /* go through bridges */
    for(j=0;j<T.Nnode;j++){
      for(k=j;k<T.Nnode;k++){
	n=0;
	l=getbridgefromSA(T.SA[j][k],n);
	while(l>1){
	  /* label all sites along the bridges */
	  sitevar=labelbridge(&T,l,T.embedlabel[j],T.embedlabel[k],sitevar);
	  n++;
	  l=getbridgefromSA(T.SA[j][k],n);
	}	
      }
    }
  
    /* go through hairs */ 
    for(j=0;j<T.Nnode;j++){
      k=T.Nnode;
      n=0;
      l=getbridgefromSA(T.SA[j][k],n);
      while((l>=1)&&(sitevar<T.Nsite)){
	/* label all sites along the hairs */
	sitevar=labelhair(&T,l,T.embedlabel[j],sitevar);
	n++;
	l=getbridgefromSA(T.SA[j][k],n);
      }	
    }
  } 

  /* create topolabel[] */
  for(i=0;i<T.Nsite;i++){
    T.topolabel[T.embedlabel[i]]=i;
  }

  return T;
}
