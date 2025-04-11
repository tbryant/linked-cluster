typedef struct{
  char Nbond;
  char x_ylatconst;
  /* bond dir stores the orientation for the bond */
  /* two bits per, 0 for horizontal, 1 for vertical, 2 for lower left to upper right */
  char bonddir[3*MAXSITE];
  /* bond stores the site coordinate for the left or lower site of the bond */
  char bond[3*MAXSITE];
} bondgraph;

typedef struct{
  int Nbond;
  int latconst;
  /* bond dir stores the orientation for the bond */
  /* 0 for horizontal, 1 for vertical, 2 for lower left to upper right */
  int bonddir[3*MAXSITE];
  /* bond stores the site coordinate for the left or lower site of the bond */
  int bond_x[3*MAXSITE];
  int bond_y[3*MAXSITE];
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

void displaybondgraph(bondgraph B);
int displaybgb(bondgraphbig B);

int readbgbfromfile(bondgraphbig *B,int Nbond,FILE *fp){
  int i,temp;
  char x;

  (*B).Nbond = Nbond;

  /* read latconst */
  if(fread(&x,sizeof(char),1,fp)){
    (*B).latconst = (int) x;
    
    for(i=0;i<(*B).Nbond;i++){
      if((i%4)==0){
	/* read a char */
	fread(&x,sizeof(char),1,fp);
	temp = (int)x;
      }
      (*B).bonddir[i]=(temp>>(2*(i%4)))&0x3;
    }
    
    for(i=0;i<(*B).Nbond;i++){
      fread(&x,sizeof(char),1,fp);
      (*B).bond_x[i]=bondx(x);
      (*B).bond_y[i]=bondy(x);
    }
    
    /* make the rest empty */
    for(i=(*B).Nbond;i<3*MAXSITE;i++){
      (*B).bonddir[i]=0;
      (*B).bond_x[i]=0;   
      (*B).bond_y[i]=0;
    }
    return 1;
  }
  /* if it was the end of the file */
  return 0;
}

void writebgbtofile(bondgraphbig B, FILE *outfp){
  int i,n;
  int temp;
  char x;
  
  n=B.Nbond;
  
  /* write latconst as a char */
  x = (char)B.latconst;
  fwrite(&x,sizeof(char),1,outfp);
  
  temp=0;
  for(i=0;i<n;i++){
    /* put bonddir into 2 bits of temp */
    temp += (B.bonddir[i]&0x3)<<(2*(i%4));
    if((i%4)==3){
      /* write a char and reset temp */
      x = (char)temp;
      temp=0;
      fwrite(&x,sizeof(char),1,outfp);
    }
  }
  if((i%4)!=0){
    /* write last char */
    x = (char)temp;
    fwrite(&x,sizeof(char),1,outfp);
  }
    
  for(i=0;i<n;i++){
    /* put bondx and bondy into one char */
    x = makebond(B.bond_x[i],B.bond_y[i]);
    fwrite(&x,sizeof(char),1,outfp);
  }
}



bondgraphbig smalltobgb(bondgraph B){
  bondgraphbig Bbig;
  int i;

  Bbig.Nbond = (int)(B.Nbond&0xff);
  Bbig.latconst = bondy(B.x_ylatconst);
  for(i=0;i<Bbig.Nbond;i++){
    Bbig.bonddir[i]=(int)B.bonddir[i];
    Bbig.bond_x[i]=bondx(B.bond[i]);   
    Bbig.bond_y[i]=bondy(B.bond[i]);
  }
  for(i=Bbig.Nbond;i<3*MAXSITE;i++){
    Bbig.bonddir[i]=0;
    Bbig.bond_x[i]=0;   
    Bbig.bond_y[i]=0;
  }

  return Bbig;
}

bondgraph bondgraphbigtosmall(bondgraphbig Bbig){
  bondgraph B;
  int i;
  
  B.Nbond=(char)Bbig.Nbond;
  B.x_ylatconst=makebond(0,Bbig.latconst);

/*   printf("compressing\n"); */
/*   displaybgb(Bbig); */
  /* put bond dir into chars */
  for(i=0;i<Bbig.Nbond;i++){
    B.bonddir[i]=(char)(Bbig.bonddir[i]&0xff);
  }

  for(i=0;i<Bbig.Nbond;i++){
    B.bond[i]=makebond(Bbig.bond_x[i],Bbig.bond_y[i]);
  }

/*   displaybondgraph(B); */
  return B;
}

#define BGBSHOWSIZE 20

int displaybgb(bondgraphbig B){
  int j,k,l;
  int Nbond,xc,yc,bdir;
  int show[BGBSHOWSIZE][BGBSHOWSIZE];

  Nbond = B.Nbond;
  printf("Nbond: %i\n",Nbond);
  printf("Lattice Constant: %i \n",B.latconst);
  
  printf("Bonds:\n");
  for(k=0;k<BGBSHOWSIZE;k++){
    for(l=0;l<BGBSHOWSIZE;l++){
      show[k][l]=-1;
    }
  }
  for(j=0;j<Nbond;j++){
    
    xc = B.bond_x[j];
    yc = B.bond_y[j];
    bdir = B.bonddir[j];
    
    printf("%i:(%i,%i) dir %i\n",j,xc,yc,bdir);
    
    if(bdir==0)
      show[(2*xc+1)%BGBSHOWSIZE][(2*yc)%BGBSHOWSIZE]='-';
    if(bdir==1)
      show[(2*xc)%BGBSHOWSIZE][(2*yc+1)%BGBSHOWSIZE]='|';
    if(bdir==2)
      show[(2*xc+1)%BGBSHOWSIZE][(2*yc+1)%BGBSHOWSIZE]='/';
    if(bdir==3)
      show[(2*xc-1)%BGBSHOWSIZE][(2*yc+1)%BGBSHOWSIZE]='\\';
  }
  for(k=BGBSHOWSIZE-1;k>=0;k--){
    for(j=0;j<BGBSHOWSIZE;j++){
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
  int newbonddir[2*MAXSITE];
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
    lowdir=4;
    
    for(i=0;i<(*B).Nbond;i++){
      if(used[i]==0){
	/* compare bonds */
	if(comparebond(low_x,low_y,lowdir,(*B).bond_x[i],(*B).bond_y[i],(*B).bonddir[i])==1){
	  /* new low bond */
	  low_x=(*B).bond_x[i];
	  low_y=(*B).bond_y[i];
	  lowdir=(*B).bonddir[i];
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
    newbonddir[loopcount]=(*B).bonddir[lowi];
    new_x[loopcount]=(*B).bond_x[lowi];
    new_y[loopcount]=(*B).bond_y[lowi];
    loopcount++;
  }
  /* put new into B */
  for(i=0;i<(*B).Nbond;i++){
    (*B).bonddir[i] = newbonddir[i];
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
  while((i<B1.Nbond)&&(comparebond(B1.bond_x[i],B1.bond_y[i],B1.bonddir[i],B2.bond_x[i],B2.bond_y[i],B2.bonddir[i])==0)){
    i++;
    /* if they match */
    if(i==B1.Nbond){
/*       printf("MATCH!\n"); */
/*       displaybgb(B1); */
/*       displaybgb(B2); */
      return 0;  
    }
  }

/*   displaybgb(B1); */
/*   displaybgb(B2); */

  return comparebond(B1.bond_x[i],B1.bond_y[i],B1.bonddir[i],B2.bond_x[i],B2.bond_y[i],B2.bonddir[i]);
 }


void rotate(bondgraphbig *B){
  /* rotate to the lowest rotation (defined by comparebgb) */
  /* check symmetries of the graph to get lattice constant */
  int i,rot;
  bondgraphbig Blowest,Brot,Brotnew[6];
  int coord1[2*MAXSITE][2],coord2[2*MAXSITE][2];
  int newcoord1[2*MAXSITE][2],newcoord2[2*MAXSITE][2];
  
  Blowest=(*B);
  Brotnew[0]=(*B);

/*   printf("before rot\n"); */
/*   displaybgb(*B); */

  /* make coords for both sides of the bond */
  for(i=0;i<(*B).Nbond;i++){
    coord1[i][0]=(*B).bond_x[i];
    coord1[i][1]=(*B).bond_y[i];
    if((*B).bonddir[i]==0){
      /* horizontal bond */
      coord2[i][0]=coord1[i][0]+1;
      coord2[i][1]=coord1[i][1];
    }
    if((*B).bonddir[i]==1){
      /* vertical bond */
      coord2[i][0]=coord1[i][0];
      coord2[i][1]=coord1[i][1]+1;
      
    }
    if((*B).bonddir[i]==2){
      /* diagonal / bond */
      coord2[i][0]=coord1[i][0]+1;
      coord2[i][1]=coord1[i][1]+1;
    }
  }
  
  for(rot=0;rot<5;rot++){
    /* copy Nbond and latconst, etc. */
    Brot=(*B);
    
    /*     printf("before rot %i\n",rot); */
    /*     displaybgb(Brot); */
    
    /* five (not the identity) symmetries of the 2d triangle */
    switch(rot){
      /* 0: x,y -> -x,-y */
    case 0 :
      for(i=0;i<(*B).Nbond;i++){
	newcoord1[i][0] = -coord1[i][0];
	newcoord1[i][1] = -coord1[i][1];

	newcoord2[i][0] = -coord2[i][0];
	newcoord2[i][1] = -coord2[i][1];
      }	
      break;

      /* 1: x,y -> -y,x-y */
    case 1 :
      for(i=0;i<(*B).Nbond;i++){
	newcoord1[i][0] = -coord1[i][1];
	newcoord1[i][1] = coord1[i][0]-coord1[i][1];

	newcoord2[i][0] = -coord2[i][1];
	newcoord2[i][1] = coord2[i][0]-coord2[i][1];
      }	
      break;

      /* 2: x,y ->  y,y-x */
    case 2 :
      for(i=0;i<(*B).Nbond;i++){
	newcoord1[i][0] = coord1[i][1];
	newcoord1[i][1] = -coord1[i][0]+coord1[i][1];

	newcoord2[i][0] = coord2[i][1];
	newcoord2[i][1] = -coord2[i][0]+coord2[i][1];
      }	
      break;

      /* 3: x,y -> x-y,x */
    case 3 :
      for(i=0;i<(*B).Nbond;i++){
	newcoord1[i][0] = coord1[i][0]-coord1[i][1];
	newcoord1[i][1] = coord1[i][0];

	newcoord2[i][0] = coord2[i][0]-coord2[i][1];
	newcoord2[i][1] = coord2[i][0];
      }	
      break;

      /* 4: x,y -> y-x,-x */
    case 4 :
      for(i=0;i<(*B).Nbond;i++){
	newcoord1[i][0] = coord1[i][1]-coord1[i][0];
	newcoord1[i][1] = -coord1[i][0];

	newcoord2[i][0] = coord2[i][1]-coord2[i][0];
	newcoord2[i][1] = -coord2[i][0];
      }	
      break;
	
    default :	break;
    }
    /* make brot out of newcoord */
    
    for(i=0;i<(*B).Nbond;i++){
      /* find the lower coord */
      /* and the bonddir */

      if(comparebond(newcoord1[i][0],newcoord1[i][1],0,newcoord2[i][0],newcoord2[i][1],0)<0){
	Brot.bond_x[i]=newcoord1[i][0];
	Brot.bond_y[i]=newcoord1[i][1];
	if((newcoord2[i][0]==newcoord1[i][0]+1)&&(newcoord2[i][1]==newcoord1[i][1])){
	  /* horizontal bond */
	  Brot.bonddir[i]=0;
	}
	if((newcoord2[i][0]==newcoord1[i][0])&&(newcoord2[i][1]==newcoord1[i][1]+1)){
	  /* vertical bond */
	  Brot.bonddir[i]=1;
	}
	if((newcoord2[i][0]==newcoord1[i][0]+1)&&(newcoord2[i][1]==newcoord1[i][1]+1)){
	  /* diagonal / bond */
	  Brot.bonddir[i]=2;
	}
      }
      else{
	Brot.bond_x[i]=newcoord2[i][0];
	Brot.bond_y[i]=newcoord2[i][1];
	if((newcoord1[i][0]==newcoord2[i][0]+1)&&(newcoord1[i][1]==newcoord2[i][1])){
	  /* horizontal bond */
	  Brot.bonddir[i]=0;
	}
	if((newcoord1[i][0]==newcoord2[i][0])&&(newcoord1[i][1]==newcoord2[i][1]+1)){
	  /* vertical bond */
	  Brot.bonddir[i]=1;
	}
	if((newcoord1[i][0]==newcoord2[i][0]+1)&&(newcoord1[i][1]==newcoord2[i][1]+1)){
	  /* diagonal / bond */
	  Brot.bonddir[i]=2;
	}
      }
    }	
    
    
    
    order(&Brot);
    
    /*     printf("after order\n"); */
    /*     displaybgb(Brot); */
    
    translate(&Brot);
    
    
/*     printf("after rot %i\n",rot); */
/*     displaybgb(Brot); */

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
