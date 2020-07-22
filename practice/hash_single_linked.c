#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

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

Table *Ptr_entry_table = NULL;
int current_size;
int max_size = 10;

void display() {
   int i, j; 
   Node *Ptr_tmp_table = NULL;
   Node *Ptr_tmp_list = NULL;

   for (i=0; i<max_size; ++i) {
      Ptr_tmp_table = (Ptr_entry_table+i)->Ptr_head_entry;
      if (!Ptr_tmp_table) {
         printf("No hashed values in table entry %d\n", i);
      }
      else {
         printf("Entry %d: ", i);
         Ptr_tmp_list = Ptr_tmp_table;
         for (j=0; Ptr_tmp_list; ++j) {
            printf("key=%d,value=%d ", Ptr_tmp_list->key, Ptr_tmp_list->value);
            if (!Ptr_tmp_list->next) break;
            else Ptr_tmp_list++;
         }
      Ptr_tmp_table++; 
      }
   }
} /* end display() */

void init_table() {
   int i;
   for (i=0; i<max_size; ++i) {
      (Ptr_entry_table+i)->Ptr_head_entry = NULL;
      (Ptr_entry_table+i)->Ptr_tail_entry = NULL;
   }
}

int hash(int key) {
   return (key%max_size);
}

bool insert(int key, int value) {
   bool retval = true;
   int hashed_entry = hash(key);
   Node *Ptr_tmp = (Ptr_entry_table+hashed_entry)->Ptr_head_entry;
   if ((Ptr_tmp = (Node *) malloc (sizeof(Node))) 
      == NULL) {
      retval = false;
   }
   Ptr_tmp->key = key;
   Ptr_tmp->value = value;
   Ptr_tmp->next = NULL;
   return retval;   
} /* end insert() */

int main () {

   int choice = 0;
   int key, value;
   if ((Ptr_entry_table = (Table *) malloc (sizeof(Table) * max_size)) == NULL) {
      assert(Ptr_entry_table == NULL);
   }
  
   /* Get user input for list */
   do {
      printf("\nMENU:\n");
      printf("===============\n");
      printf("Enter 1: Display List Entries\n");
      printf("Enter 2: Insert key/value pair\n");
      printf("Enter 3: Delete key/value pair\n");
      printf("Enter 4: Find key/value pair\n");
      printf("Enter 99: Exit\n");
      scanf("%d", &choice);
      switch (choice) {
         case 1:
            display();
            break;
         case 2:
            printf("Enter key: ");    
            scanf("%d", &key);
            printf("Enter value: ");    
            scanf("%d", &value);
            if (insert(key, value) == NULL) {
               printf("Failed to add entry.\n");
               exit(0);
            }
            break;
         case 3:
            break;
         case 4:
            break;
         case 99:
            break;
         default:
            printf("Invalid entry...\n");
       }
   } while (choice != 99);
}
