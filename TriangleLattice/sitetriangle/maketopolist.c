typedef struct list_node {
  topo data;
  struct list_node *next;
  struct list_node *prev;
} dlnode;


void dlinsert(dlnode **before, topo data_in){
  /* inserts a node after 'before' and points to the new node */
  dlnode *tmp,*after;
   
  if ((tmp = malloc(sizeof(dlnode))) == NULL) {
    exit(0);
  }
  

  if((*before)!=NULL){
    after = (*before)->next;
    if(after!=NULL)
      after->prev = tmp;
    (*before)->next = tmp;
    tmp->next = after;
  }
  else{
    tmp->next = NULL;
  }

  tmp->prev = (*before); 
  tmp->data = data_in;
  (*before) = tmp;
}

void dlremove(dlnode **head) {
  /* removes the node *head, points to next */
  dlnode *temp;
  if ((*head) != NULL){
    if ((*head)->next!=NULL){
      ((*head)->next)->prev = (*head)->prev;
    }
    if ((*head)->prev!=NULL){
      ((*head)->prev)->next = (*head)->next;
    }      
    temp = (*head)->next;
    free((*head));
    *head = temp;
  }
}

void dlfree(dlnode **head) {
  while (*head != NULL) {
    dlremove(head);
  } 
}

