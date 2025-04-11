#define MAXBOND 36
#define COORD 6

typedef struct{
  int length;
  int ishair;
  int site[MAXSITE];
} bridgetype;

typedef struct{
  int topo;
  bondgraphbig embed;
  int Nbond;
  int Nsite;
  int Nnode;
  int Nbridge;
  int latconst;
  bridgetype bridge[MAXBOND]; 
  int topolabel[MAXSITE];
  int embedlabel[MAXSITE];
  int site_x[MAXSITE];
  int site_y[MAXSITE];
  int left[MAXBOND];
  int right[MAXBOND];
  int SA[MAXSITE][MAXSITE+1];
} topo;

typedef struct{
  int topo;
  int latconst;
  int Nnode;
  int SA[MAXSITE][MAXSITE+1];
} topoSA;

typedef struct{
  int topo;
  bondgraphbig embed;
  int topolabel[MAXSITE];
  int left[MAXBOND];
  int right[MAXBOND];
  int Nnode;
  int SA[MAXSITE][MAXSITE+1];
} toposub;

typedef struct{
  int parenttopo;
  int Nbond;
  int topo;
  int Nsite;
  int topositemap[MAXSITE];
} subtopo;

topoSA maketopoSA(topo T){
  int i,j;
  topoSA TSA;

  TSA.topo=T.topo;
  TSA.latconst=T.latconst;
  TSA.Nnode=T.Nnode;
  for(i=0;i<MAXSITE;i++){
    for(j=0;j<MAXSITE+1;j++){
      TSA.SA[i][j]=T.SA[i][j];
    }
  }
  return TSA;
}

toposub maketoposub(topo T){
  int i,j;
  toposub Tsub;

  Tsub.topo = T.topo;
  Tsub.embed = T.embed;
  for(i=0;i<MAXSITE;i++){
    Tsub.topolabel[i]=T.topolabel[i];
  }
  
  for(i=0;i<MAXBOND;i++){
    Tsub.left[i] = T.left[i];
    Tsub.right[i] = T.right[i];
  }
  Tsub.Nnode=T.Nnode;
  for(i=0;i<MAXSITE;i++){
    for(j=0;j<MAXSITE+1;j++){
      Tsub.SA[i][j]=T.SA[i][j];
    }
  }
  
  return Tsub;
}

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

void writebondgraphbigtofile(bondgraphbig B,FILE *fp){
  int i;
  char x;
  
  x = (char)(B.Nbond);
  fwrite(&x,sizeof(char),1,fp);

  x = (char)(B.latconst);
  fwrite(&x,sizeof(char),1,fp);
  
  for(i=0;i<B.Nbond;i++){
    x = (char)(B.bonddir[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<B.Nbond;i++){
    x = (char)(B.bond_x[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<B.Nbond;i++){
    x = (char)(B.bond_y[i]);
    fwrite(&x,sizeof(char),1,fp);
  }  
}

int readbondgraphbigfromfile(bondgraphbig *B,FILE *fp){
  int i;
  char x;
  
  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*B).Nbond=(int)x;
  
  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*B).latconst=(int)x;
  
  for(i=0;i<(*B).Nbond;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*B).bonddir[i]=(int)x;
  }
  for(i=0;i<(*B).Nbond;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*B).bond_x[i]=(int)x;
  }
  for(i=0;i<(*B).Nbond;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*B).bond_y[i]=(int)x;
  } 
  return 1; 
}

void writebridgetofile(bridgetype b,FILE *fp){
  int i;
  char x;

  x = (char)(b.length);
  fwrite(&x,sizeof(char),1,fp);
  x = (char)(b.ishair);
  fwrite(&x,sizeof(char),1,fp);
  for(i=0;i<b.length+1;i++){
    x = (char)(b.site[i]);
    fwrite(&x,sizeof(char),1,fp);
  }  
}

int readbridgefromfile(bridgetype *b,FILE *fp){
  int i;
  char x;

  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*b).length=(int)x;
  if(fread(&x,sizeof(char),1,fp)==0) return 0;
  (*b).ishair=(int)x;
  for(i=0;i<(*b).length+1;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*b).site[i]=(int)x;
  }
  return 1;  
}

void writetopotofile(topo T,FILE *fp){
  int i,j;
  char x;

  fwrite(&(T.topo),sizeof(int),1,fp);
  writebondgraphbigtofile(T.embed,fp);  
  fwrite(&(T.Nbond),sizeof(int),1,fp);
  fwrite(&(T.Nsite),sizeof(int),1,fp);
  fwrite(&(T.Nnode),sizeof(int),1,fp);
  fwrite(&(T.Nbridge),sizeof(int),1,fp);
  fwrite(&(T.latconst),sizeof(int),1,fp);
  for(i=0;i<T.Nbridge;i++){
    writebridgetofile(T.bridge[i],fp);  
  }
  for(i=0;i<T.Nsite;i++){
    x = (char)(T.topolabel[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<T.Nsite;i++){
    x = (char)(T.embedlabel[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<T.Nsite;i++){
    x = (char)(T.site_x[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<T.Nsite;i++){
    x = (char)(T.site_y[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<T.Nbond;i++){
    x = (char)(T.left[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  for(i=0;i<T.Nbond;i++){
    x = (char)(T.right[i]);
    fwrite(&x,sizeof(char),1,fp);
  }
  if(T.Nnode==0){
    fwrite(&(T.SA[0][0]),sizeof(int),1,fp);
  }
  else{    
    for(i=0;i<T.Nnode;i++){
      for(j=0;j<T.Nnode+1;j++){
	fwrite(&(T.SA[i][j]),sizeof(int),1,fp);
      }
    }
  }
}

int readtopofromfile(topo *T,FILE *fp){
  int i,j;
  char x;

  if(fread(&((*T).topo),sizeof(int),1,fp)==0) return 0;
  if(readbondgraphbigfromfile(&((*T).embed),fp)==0) return 0;  
  if(fread(&((*T).Nbond),sizeof(int),1,fp)==0) return 0;
  if(fread(&((*T).Nsite),sizeof(int),1,fp)==0) return 0;
  if(fread(&((*T).Nnode),sizeof(int),1,fp)==0) return 0;
  if(fread(&((*T).Nbridge),sizeof(int),1,fp)==0) return 0;
  if(fread(&((*T).latconst),sizeof(int),1,fp)==0) return 0;
  for(i=0;i<(*T).Nbridge;i++){
    if(readbridgefromfile(&((*T).bridge[i]),fp)==0) return 0;  
  }
  for(i=0;i<(*T).Nsite;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*T).topolabel[i]=(int)x;
  }
  for(i=0;i<(*T).Nsite;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*T).embedlabel[i]=(int)x;
  }
  for(i=0;i<(*T).Nsite;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*T).site_x[i]=(int)x;
  }
  for(i=0;i<(*T).Nsite;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*T).site_y[i]=(int)x;
  }
  for(i=0;i<(*T).Nbond;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*T).left[i]=(int)x;
  }
  for(i=0;i<(*T).Nbond;i++){
    if(fread(&x,sizeof(char),1,fp)==0) return 0;
    (*T).right[i]=(int)x;
  }
  if((*T).Nnode==0){
      if(fread(&((*T).SA[0][0]),sizeof(int),1,fp)==0) return 0;
  }
  else{
    for(i=0;i<(*T).Nnode;i++){
      for(j=0;j<(*T).Nnode+1;j++){
	if(fread(&((*T).SA[i][j]),sizeof(int),1,fp)==0) return 0;
      }
    }
  }
  return 1;
}

void writesubtopotofile(subtopo S,FILE *fp){
  int i;
  char x;

  /* int */
  fwrite(&(S.parenttopo),sizeof(int),1,fp);

  /* char */
  x = (char)S.Nbond;
  fwrite(&x,sizeof(char),1,fp);
  
  /* char */
  x = (char)S.Nsite;
  fwrite(&x,sizeof(char),1,fp);

  /* int */
  fwrite(&(S.topo),sizeof(int),1,fp);
  
  /* Nsite * char */
  for(i=0;i<=S.Nsite;i++){
    x = (char)S.topositemap[i];
    fwrite(&x,sizeof(char),1,fp);
  }
}


int readsubtopofromfile(subtopo *S,FILE *fp){
  int i;
  char x;

  /* int */
  if(fread(&((*S).parenttopo),sizeof(int),1,fp)==0){
    return 0;
  }
  
  /* char */
  if(fread(&x,sizeof(char),1,fp)==0){
    return 0;
  }
  (*S).Nbond = (int)x;
  
  /* char */
  if(fread(&x,sizeof(char),1,fp)==0){
    return 0;
  }
  (*S).Nsite = (int)x;

  /* int */
  if(fread(&((*S).topo),sizeof(int),1,fp)==0){
    return 0;
  }
  
  
  /* Nsite * char */
  for(i=0;i<=(*S).Nsite;i++){
    if(fread(&x,sizeof(char),1,fp)==0){
      return 0;
    }
    (*S).topositemap[i] = (int)x;
  }
  
  return 1;
}


void displaysubtopo(subtopo S){
  int i;
  printf("%i (%i %i:%i) ",S.parenttopo,S.Nbond,S.Nsite,S.topo);
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
  for(i=0;i<6;i++){
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
  
/*   displaytopo(*T); */
/*   printf("labelbridge(T,l=%i,start=%i,end=%i,sitevar=%i)\n",l,start,end,sitevar); */

  while(i<(*T).Nbridge){
    
/*     printf("i=%i length=%i (*T).bridge[i].site[0]=%i,(*T).bridge[i].site[l]=%i\n",i,(*T).bridge[i].length,(*T).bridge[i].site[0],(*T).bridge[i].site[l]); */
    if(l==(*T).bridge[i].length){
      if((((*T).bridge[i].site[0]==start)&&((*T).bridge[i].site[l]==end))||(((*T).bridge[i].site[0]==end)&&((*T).bridge[i].site[l]==start))){
	for(j=1;j<l;j++){
	  (*T).embedlabel[sitevar]=(*T).bridge[i].site[j];
	  sitevar++;
	}
/* 	printf("returns inside %i\n",sitevar); */
	return sitevar;
      }
    }
    
    i++;
  }
/*   printf("returns outside %i\n",sitevar); */
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
/* 	  (*T).bridge[i].site[0]=-1; /\* effectively remove bridge from list *\/ */
	/* actually remove bridge from list */
/* 	for(k=i;k<(*T).Nbridge;k++){ */
/* 	  (*T).bridge[k]=(*T).bridge[k+1]; */
/* 	} */
/* 	(*T).Nbridge--; */
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


int issameSA(topoSA T1,topoSA T2){
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

int issameSAsub(toposub T1,toposub T2){
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

 
/*   for(k=0;k<MAXSITE;k++){ */
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
    printf("topolabel[%i]=%i embedlabel[%i]=%i\n",k,T.topolabel[k],k,T.embedlabel[k]);
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


int ordernodes(int SA[MAXSITE][MAXSITE+1],int Nnode,int *sitemap){
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
  int SAtemp[MAXSITE][MAXSITE+1];
  int i,j,k,l,n;
  int newbridge;
  int isloop=1;
  int bondvar,sitevar;
  int valence[MAXSITE];
  int node[MAXSITE];
  int site[MAXSITE];
  int sitec=0;
  int sitebonds[MAXSITE][COORD+1];

  /* copy embedding into topo */
  T.Nbond = B.Nbond;
  T.latconst = B.latconst;
  T.embed = B;

  /* init T */
  T.topo=0;
  T.Nsite=0;
  T.Nnode=0;
  T.Nbridge=0;
  for(i=0;i<MAXBOND;i++){
    T.bridge[i].length=0;
    T.bridge[i].ishair=0;
    for(j=0;j<MAXSITE;j++){
      T.bridge[i].site[j]=0;}
  }
  for(i=0;i<MAXSITE;i++){
    T.topolabel[i]=0;
    T.embedlabel[i]=0;
    T.site_x[i]=0;
    T.site_y[i]=0;
  }
  for(i=0;i<MAXBOND;i++){
    T.left[i]=0;
    T.right[i]=0;
  }
  for(i=0;i<MAXSITE;i++){
    for(j=0;j<MAXSITE+1;j++){
      T.SA[i][j]=0;
    }
  }

  /* init valence */
  for(i=0;i<=MAXSITE;i++){
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
    if(B.bonddir[i]==0){
      /* horizontal */
      T.site_x[sitec] = B.bond_x[i]+1;
      T.site_y[sitec] = B.bond_y[i]; 
    }          
    if(B.bonddir[i]==1){
      /* vertical */
      T.site_x[sitec]=B.bond_x[i];
      T.site_y[sitec]=B.bond_y[i]+1; 
    }
    if(B.bonddir[i]==2){
      /* diagonal /  */
      T.site_x[sitec]=B.bond_x[i]+1;
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
	  if(T.bridge[n].length>=MAXBOND){
	    printf("ERROR length > MAXBOND");
	    displaybgb(T.embed);
	    exit(0);
	  }
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
	  /* check to not store the same bridge in reverse */
	  i=0;
	  newbridge=1;
	  while(i<n){
    	    if(T.bridge[i].length==T.bridge[n].length){
	      l=0;
	      while((l<T.bridge[n].length)&&(T.bridge[i].site[T.bridge[i].length-l]==T.bridge[n].site[l]))
		l++;
	      if(l==T.bridge[n].length)
		newbridge=0;			     
	    }
	    i++;
	  }
	  if(newbridge){
	    SAtemp[j][node[sitevar]]= addbridge(T.bridge[n].length,SAtemp[j][node[sitevar]]);
	    /* keep SAtemp symmetric */
	    SAtemp[node[sitevar]][j] = SAtemp[j][node[sitevar]];
	    n++;
	  }
	}
      }
    }
  }
    
  T.Nbridge = n;
  
  if(T.Nbridge>MAXBOND){
    printf("WARNING T.Nbridge > MAXBOND, MAXBOND = %i, n = %i \n",MAXBOND,n);
  }
      
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
      /* is a line */
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
