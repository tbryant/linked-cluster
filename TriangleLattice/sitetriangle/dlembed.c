typedef struct list_node {
  graph data;
  unsigned int key;
  struct list_node *next;
} dlnode;

/* HASHSIZE is the size of the array for the hash list*/
#define HASHSIZE 57099299


void dlinsert(dlnode **before, graph data_in){
  /* inserts a node after 'before' and points to the new node */
  dlnode *tmp,*after;
   
  if ((tmp = malloc(sizeof(dlnode))) == NULL) {
    printf("out of memory.  dlinsert\n");
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
  (*before) = tmp;
}

void dlremove(dlnode *head) {
  /* removes the node after *head */
  dlnode *temp;
  if (head->next != NULL){
    temp = head->next;
    head->next = temp->next;
    free(temp);
  }
}

void dlfree(dlnode *head) {
  while (head->next != NULL) {
    dlremove(head);
  } 
  free(head);
}


unsigned int keygen(graph M){
  int i;
  unsigned int key=0;
  
  
  for(i=1;i<=G_Nsite;i++)
    key +=  (1<<(3*i))*((x(M.site[G_Nsite-i])-20)+100*(y(M.site[G_Nsite-i])-20));

  /*   printf("0 - %i\n",key);  */ 
  key = key%HASHSIZE;
  /*   printf("2 - %i\n",key); */
  return key;
}
