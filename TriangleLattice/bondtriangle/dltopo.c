typedef struct list_node {
  topo data;
  unsigned int key;
  struct list_node *next;
} toponode;

/* TOPOHASHSIZE is the size of the array for the hash list*/
#define TOPOHASHSIZE 979693

void topoinsert(toponode **before, topo data_in, unsigned int key){
  /* inserts a node after 'before' and points to the new node */
  toponode *tmp,*after;
   
  if ((tmp = malloc(sizeof(toponode))) == NULL) {
    printf("out of memory.  topoinsert\n");
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

void toporemove(toponode *head) {
  /* removes the node after *head */
  toponode *temp;
  if (head->next != NULL){
    temp = head->next;
    head->next = temp->next;
    free(temp);
  }
}

void topofree(toponode *head) {
  if(head!=NULL){
    while (head->next != NULL) {
      toporemove(head);
    } 
    free(head);
  }
}


unsigned int topokeygen(topo T){
  int i,j;
  unsigned int key=T.SA[0][0];
  
  
  for(i=1;i<=T.Nnode;i++){
    for(j=1;j<=T.Nnode+1;j++){
      key +=  (1<<(3*i+j))*(T.SA[i-1][j-1]);
    }
  }
  
  key = key%TOPOHASHSIZE;
  return key;
}
