/* Fill in your Name and GNumber in the following two comment fields
 * Name: Venkata Sasank Mudigonda
 * GNumber: 01205757
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

#define MAX_LOAD 2.0


void destroy_symbol_llist(Symbol** head);
Symbol* find_var(Symbol* head, char* var);
void insert_new_var(Symbol** head, char* var, int val);

/* (IMPLEMENT THIS FUNCTION)
 * In this function, you will create a new Symtab struct.
 * Follow the Directions in the Project Documentation for this Function
 * Return the pointer to the new symtab.
 * On any memory errors, return NULL
 */
Symtab *hash_initialize() {
  // allocating new space for symtab
  Symtab *symtab = malloc(sizeof(Symtab));
  if(symtab == NULL){
    return NULL;
  }
  symtab->size = 0;
  symtab->capacity = HASH_TABLE_INITIAL;

  // initializing symbols array
  Symbol **symbols_arr = malloc(symtab->capacity*sizeof(Symbol*));
  int i;
  for(i = 0; i < symtab->capacity; i ++){
    symbols_arr[i] = NULL;
  }
  symtab->table = symbols_arr;

  return symtab;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy your Symbol Table.
 * Follow the Directions in the Project Documentation for this Function
 * Return on any memory errors.
 */
void hash_destroy(Symtab *symtab) {

  if(symtab == NULL){
    return;
  }
  int i;
  // destroying linked lists associated with hash indices
  for(i = 0; i < symtab->capacity; i++){
    // destry each linked list
    destroy_symbol_llist(symtab->table);
  }
  // freeing up symtab after freeing its linked list
  free(symtab->table);
  return;
}

void destroy_symbol_llist(Symbol** head){
  // return when linked list is empty
  if((*head) == NULL){
    return;
  }

  Symbol *walker = (*head);
  while(walker != NULL){
    Symbol* next = walker->next;
    symbol_free(walker);
    walker = next;
  }
  // setting table head to NULL
  (*head) = NULL;
  return;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the capacity of the table inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_capacity(Symtab *symtab) {
  if(symtab == NULL){
    return -1;
  }
  return symtab->capacity;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of used indexes in the table (size) inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_size(Symtab *symtab) {
  if(symtab == NULL){
    return -1;
  }
  return symtab->size;
}

/* (IMPLEMENT THIS FUNCTION)
 * Adds a new Symbol to the symtab via Hashing.
 * Follow the Directions in the Project Documentation for this Function
 * If symtab is NULL, there are any malloc errors, or if any rehash fails, return -1;
 * Otherwise, return 0;
 */
int hash_put(Symtab *symtab, char *var, int val) {
  if(symtab == NULL){
    return -1;
  }
  // get the index of the var
  int capacity = hash_get_capacity(symtab);
  int index = hash_code(var)%capacity;
  Symbol* node = find_var(symtab->table[index], var);
  if(node == NULL){
    /*
      * symbol not found, has to be inserted
      * check if hash table reached max load
      * if yes, reshash, else insert the var at the end
    */
    float load = ((float)hash_get_size(symtab))/((float)hash_get_capacity(symtab));
    if(load >= MAX_LOAD){
      /*
        * rehash with double capacity
        * find new index
        * insert the new var at new index
        * increment the size
      */
      capacity = capacity*2;
      hash_rehash(symtab, capacity);
      index = hash_code(var)%capacity;
      insert_new_var(&(symtab->table[index]), var, val);
      symtab->size += 1;
      return 0;
    }
    else{
      /*
        * Insert new var
        * Increment size
      */
      insert_new_var(&(symtab->table[index]), var, val);
      symtab->size += 1;
      return 0;
    }
  }
  else{
    /*
      * symbol found, modify value
    */
    node->val = val;
    return 0;
  }
  return -1;
}

Symbol* find_var(Symbol* head, char* var){
  Symbol* walker = head;
  while(walker != NULL){
    if(strcmp(walker->variable, var) == 0){
      // found the var in the hash
      return walker;
    }
    walker = walker->next;
  }
  // not found in the linked list
  return NULL;
}

void insert_new_var(Symbol** head, char* var, int val){
  // insert at last
  Symbol* walker = (*head);
  if(walker == NULL){
    (*head) = symbol_create(var, val);
    return;
  }

  while(walker->next != NULL){

    walker = walker->next;
  }
  walker->next = symbol_create(var, val);
}

/* (IMPLEMENT THIS FUNCTION)
 * Gets the Symbol for a variable in the Hash Table.
 * Follow the Directions in the Project Documentation for this Function
 * On any NULL symtab or memory errors, return NULL
 */
Symbol *hash_get(Symtab *symtab, char *var) {
  if(symtab == NULL){
    return NULL;
  }
  int capacity = hash_get_capacity(symtab);
  int index = hash_code(var)%capacity;
  Symbol* node = find_var(symtab->table[index], var);
  if(node == NULL){
    // var not found
    return NULL;
  }
  // var found, copy symbol
  return symbol_copy(node);
}

/* (IMPLEMENT THIS FUNCTION)
 * Doubles the size of the Array in symtab and rehashes.
 * Follow the Directions in the Project Documentation for this Function
 * If there were any memory errors, set symtab->array to NULL
 * If symtab is NULL, return immediately.
 */
void hash_rehash(Symtab *symtab, int new_capacity) {
  /*
    * create new table with new capacity
    * reshash the existing data
  */

  Symtab *new_symtab = malloc(sizeof(Symtab));
  if(new_symtab == NULL){
    printf("Failed to attain memory for new hash table");
    return;
  }
  new_symtab->size = 0;
  new_symtab->capacity = new_capacity;

  // initializing symbols array
  Symbol **symbols_arr = malloc(sizeof(Symbol *)*new_symtab->capacity);
  int j;
  for(j = 0; j < new_symtab->capacity; j++){
    symbols_arr[j] = NULL;
  }
  new_symtab->table = symbols_arr;
  // new symtab is created with new capacity

  // performing reshashing
  int i;
  for (i = 0; i < hash_get_capacity(symtab); i++){
    // get each linked list
    Symbol *walker = symtab->table[i];
    // walk through and insert every element into new hash table
    while(walker != NULL){
      Symbol* sym_copy = symbol_copy(walker);
      hash_put(new_symtab, sym_copy->variable, sym_copy->val);
      walker = walker->next;
    }
  }
  // destroy old symtable
  hash_destroy(symtab);
  
  // point symtab to new hash table
  symtab->size = new_symtab->size;
  symtab->capacity = new_symtab->capacity;
  symtab->table = new_symtab->table;
  return;
}

/* Implemented for you.
 * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab) {
  if(symtab == NULL) {
    return;
  }
  printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

  int i = 0;
  Symbol *walker = NULL;

  /* Iterate every index, looking for symbols to print */
  for(i = 0; i < symtab->capacity; i++) {
    walker = symtab->table[i];
    /* For each found linked list, print every symbol therein */
    while(walker != NULL) {
      printf("| %10s: %d \n", walker->variable, walker->val);
      walker = walker->next;
    }
  }
  return;
}

/* This function is written for you.
 * This computes the hash function for a String
 */
long hash_code(char *var) {
  long code = 0;
  int i;
  int size = strlen(var);

  for(i = 0; i < size; i++) {
    code = (code + var[i]);
    if(size == 1 || i < (size - 1)) {
      code *= 128;
    }
  }

  return code;
}
