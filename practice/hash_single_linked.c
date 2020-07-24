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
   Node *Ptr_tmp_list = NULL;

   for (i=0; i<max_size; ++i) {
      Ptr_tmp_list = (Ptr_entry_table+i)->Ptr_head_entry;
      if (!Ptr_tmp_list) {
         printf("Entry %d: No hashed values\n", i);
      }
      else {
         printf("Entry %d: ", i);
         for (j=0; Ptr_tmp_list; ++j) {
            printf("key=%d,value=%d", Ptr_tmp_list->key, Ptr_tmp_list->value);
            if (!Ptr_tmp_list->next) break;
            else {
               printf(", ");
               Ptr_tmp_list = Ptr_tmp_list->next;
            }
         }
         printf("\n");
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
   Node *Ptr_tmp;
   Node *Ptr_tmp_traverse;
   int hashed_entry = hash(key);
   if ((Ptr_tmp = (Node *) malloc (sizeof(Node))) 
      == NULL) {
      retval = false;
   }
   Ptr_tmp->key = key;
   Ptr_tmp->value = value;
   Ptr_tmp->next = NULL;
   if ((Ptr_entry_table+hashed_entry)->Ptr_head_entry == NULL) {
      (Ptr_entry_table+hashed_entry)->Ptr_head_entry = Ptr_tmp;
   }
   else {
      Ptr_tmp_traverse = (Ptr_entry_table+hashed_entry)->Ptr_head_entry;
      while(Ptr_tmp_traverse->next) {
         Ptr_tmp_traverse = Ptr_tmp_traverse->next;
      }
      Ptr_tmp_traverse->next = Ptr_tmp;
   }
   return retval;   
} /* end insert() */

int main () {

   int choice = 0;
   int key, value;
   int retval;
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
            if ((retval = insert(key, value) == false)) {
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
