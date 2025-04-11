typedef struct list_node {
  bondgraphbig data;
  unsigned int key;
  struct list_node *next;
} bgbnode;

/* BGBHASHSIZE is the size of the array for the hash list*/
#define BGBHASHSIZE 49979693 	

void bgbinsert(bgbnode **before, bondgraphbig data_in, unsigned int key){
  /* inserts a node after 'before' and points to the new node */
  bgbnode *tmp,*after;
   
  if ((tmp = malloc(sizeof(bgbnode))) == NULL) {
    printf("out of memory.  bgbinsert\n");
    exit(0);
  }
  

  if((*before)!=NULL){
    after = (*before)->next;
    (*before)->next = tmp;
    tmp->next = after;
  }
  else{
    tmp->next = NULL;
  }

  tmp->data = data_in;
  tmp->key = key;

  (*before) = tmp;
}

void bgbremove(bgbnode *head) {
  /* removes the node after *head */
  bgbnode *temp;
  if (head->next != NULL){
    temp = head->next;
    head->next = temp->next;
    free(temp);
  }
}

void bgbfree(bgbnode *head) {
  if(head!=NULL){
    while (head->next != NULL) {
      bgbremove(head);
    } 
    free(head);
  }
}


unsigned int bgbkeygen(bondgraphbig B){
  int i;
  unsigned int key=0;
  
  
  for(i=1;i<=B.Nbond;i++)
    key +=  (1<<(3*i))*((B.bond_x[B.Nbond-i])+100*(B.bond_y[B.Nbond-i]+B.bonddir[B.Nbond-i]));

  /*   printf("0 - %i\n",key);  */ 
  key = key%BGBHASHSIZE;
  /*   printf("2 - %i\n",key); */
  return key;
}
