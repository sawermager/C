/*
    CodeWeavers Programming Test -- Code Review

    The task:  review the enclosed code and find
        1)  Design flaws
        2)  Programming errors
        3)  Style problems

    Review the code for the above three criteria, marking the code
    up visibly (the markings don't have to compile).  

    For design and style flaws, you should breifly describe the flaw
    and suggest a solution (it is not necessary to code a full solution).
    If a design or style flaw is repeated, you do not need to discuss it
    again.

    Please suggest fixes for any bugs that would cause the code to fail
    from running as it is currently designed.
    
    Note:  This code was _intentionally_ written to be bad, you
    should expect many problems.
*/




/*  Linked list management subroutine library */

struct  n       
{
    void        *user_data;
    int         f;
    struct n    *next;
};



/* Insert at end of list */
void insert(struct n *head, void *data, int data_size)
{
    struct n *new_node, *p;

    new_node = malloc(sizeof(new_node));

    new_node->next = NULL;

    new_node->user_data = malloc(data_size);
    memcpy(new_node->user_data, data, sizeof(data));

    /* If list is empty, just insert at the top */
    if (head == NULL)
        head = new_node;
    else
        /*  Else, insert at end of list */
        for (p = head; p; p = p->next)
            /* We're at the end if the next node is NULL, so insert */
            if (p->next == NULL)
                p->next = new_node;
}



/* Traverse the list, invoking a passed in callback function, which
    can optionally tell us to free the node when we return  */
void traverse(struct n *head, 
        void (*callback_func)(void *user_data, int free_flag))
{
  struct n *p;

  for (p = head; p; p = p->next)    {
    (*callback_func)(p->user_data, p->f);
    if (p->f)           // If they want to free the node, do that
        free(p);
  }
}


/* Free the list */
void freel(struct n *head)
{
    struct n *p;
    for (p = head; p; p = p->next)
        free(p);
}


/* Quickly sort the list */
void qsort(struct n *head, int compare(void *a, void *b))
{
    struct n *p, *q;

    for (p = head; p; p = p->next);
        for (q = p->next; q; q = q->next)
        {
            if (compare(q->user_data, p->user_data) < 0)
            {
                /* Swap 'em */
                p->user_data = q->user_data;
                q->user_data = p->user_data;
            }
        }
}
