
/* Element type */
typedef struct node {
   int key;
   int value;
   struct node *next;
} Node;

/* Pointer to head and tail of each hash entry list*/
typedef struct table {
   Node *Ptr_head_entry;
   Node *Ptr_tail_entry;
} Table;

void display(void);
void init_table(void);
int hash(int);
bool insert(int, int);
bool find(Node *, int);
Node * get_element(Node *, int);

