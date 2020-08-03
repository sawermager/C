#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

Table *Ptr_entry_table = NULL;
int current_size;
int max_size = 10;

void display() {
   int i, j; 
   Node *Ptr_tmp_list = NULL;

   printf("Total number of elements in table: %d\n", current_size);
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

bool find (Node *Ptr_tmp, int key) {
   int i;
   while (Ptr_tmp) {
      if (Ptr_tmp->key == key) {
         return true;
      }
      Ptr_tmp = Ptr_tmp->next;
   }
   return false;
}

Node *get_element(Node *Ptr_tmp, int key) {
   while (Ptr_tmp) {
      if (Ptr_tmp->key == key) 
         return Ptr_tmp;
      Ptr_tmp = Ptr_tmp->next; 
   return (Node *)NULL;
   }
}

bool insert(int key, int value) {
   bool retval = true;
   Node *Ptr_tmp;
   Node *Ptr_ele_tmp;
   int hashed_entry = hash(key);

   if ((Ptr_tmp = (Node *) malloc (sizeof(Node))) == NULL) {
      perror("malloc failed");     
      retval = false;
   }
   Ptr_tmp->key = key;
   Ptr_tmp->value = value;
   Ptr_tmp->next = NULL;

   /* empty table entry*/
   if (!((Ptr_entry_table+hashed_entry)->Ptr_head_entry)) {
      (Ptr_entry_table+hashed_entry)->Ptr_head_entry = Ptr_tmp;
      (Ptr_entry_table+hashed_entry)->Ptr_tail_entry = Ptr_tmp;
      current_size++;
   }
   else {

      /* Check to see if key/value pair already exists. */
      /* If it does, then overwrite existing value for that key. */
      if (find((Ptr_entry_table+hashed_entry)->Ptr_head_entry, key)) {

         /* get duplicate element and replace it's value */
         printf("Found pre-existing entry for key: %d. Overwriting value.\n",
            key);

         /* Can free allocated space since replacing element instead of
            adding one to end of list. */
         free(Ptr_tmp);

         /* Get element with dup key and replace value with new value */
         Ptr_ele_tmp = get_element((Ptr_entry_table+hashed_entry)->Ptr_head_entry, key); 
         Ptr_ele_tmp->value = value;
      }
      else {
         (Ptr_entry_table+hashed_entry)->Ptr_tail_entry->next = Ptr_tmp;
         (Ptr_entry_table+hashed_entry)->Ptr_tail_entry = Ptr_tmp;
         current_size++;
      }
   }   
   /* If hash table gets 75% full, double size of table */
   if ((float)current_size/(float) max_size > .75) {
      printf ("Table > 75%% full. Rehashing new table...\n");
      rehash();
   } 
   return retval;   
} /* end insert() */

/*
  Description: Once table capacity reaches 75%, double size of 
  table and copy original table into first have of new table.
*/
void rehash(void) {
   int i;
   Table *Ptr_new_hash = (Table *) malloc (sizeof(Table) * max_size * 2);
   Table *Ptr_tmp;
   assert(Ptr_new_hash != NULL);
    
   Ptr_tmp = Ptr_entry_table;
   for (i=0; i<max_size; ++i) {
      (Ptr_new_hash+i)->Ptr_head_entry = (Ptr_entry_table+i)->Ptr_head_entry;
      (Ptr_new_hash+i)->Ptr_tail_entry = (Ptr_entry_table+i)->Ptr_tail_entry;

   }
   for (i=max_size; i<(max_size * 2); ++i) {
      (Ptr_new_hash+i)->Ptr_head_entry = NULL;
      (Ptr_new_hash+i)->Ptr_tail_entry = NULL;
   }
   Ptr_entry_table = Ptr_new_hash;
   max_size *=2;
}

void delete(int key) {
   int hash_value = hash(key);
   Node *Ptr_tmp1 = NULL;
   Node *Ptr_tmp2 = NULL;
   Ptr_tmp1 = (Ptr_entry_table+hash_value)->Ptr_head_entry;

   /* check for matching key at head of 1 element list */
   if (Ptr_tmp1->key == key) {
      if (Ptr_tmp1->next == NULL) {
         (Ptr_entry_table+hash_value)->Ptr_head_entry = NULL;
         (Ptr_entry_table+hash_value)->Ptr_tail_entry = NULL;
         free(Ptr_tmp1);
      }
      else {
         (Ptr_entry_table+hash_value)->Ptr_head_entry = Ptr_tmp1->next;
         free(Ptr_tmp1);
      }
   }
   else {

      /* check for matching key in multi-node list */
      while (Ptr_tmp1->next->key != key) {
         Ptr_tmp1 = Ptr_tmp1->next;
      }
      if (Ptr_tmp1->next == ((Ptr_entry_table+hash_value)->Ptr_tail_entry)) {
         (Ptr_entry_table+hash_value)->Ptr_tail_entry = Ptr_tmp1;
         free(Ptr_tmp1->next);
         Ptr_tmp1->next = NULL;
      }
      else {
         Ptr_tmp1->next = Ptr_tmp1->next->next;
         free(Ptr_tmp1);
      }
   }
   return;
}

int main () {

   int choice = 0;
   int key, value, hashed_entry;
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
      printf("Enter 3: Delete key\n");
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
            printf("Enter key to delete: ");
            scanf("%d", &key);
            hashed_entry = hash(key);
            if (!find((Ptr_entry_table+hashed_entry)->Ptr_head_entry, key)) {
               printf("No valid entry for this key found\n");
               break;
            }
            else {
               delete(key);
            }
            current_size--;
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
