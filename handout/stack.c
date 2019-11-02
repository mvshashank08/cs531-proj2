/* Fill in your Name and GNumber in the following two comment fields
 * Name: Venkata Sasank Mudigonda
 * GNumber: 01205757
 */
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

/* (IMPLEMENT THIS FUNCTION)
 * Create a new Stack_head struct on the Heap and return a pointer to it.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return NULL
 */
Stack_head *stack_initialize() {

  // initializing the dummy node
  Stack_head *dummy = malloc(sizeof(Stack_head));

  // if there is no error on allocating memory from heap
  if(dummy != NULL){
    dummy->count = 0;
    dummy->top = NULL;
    return dummy;
  }

  // returning NULL in case there's an error from malloc
  return NULL;
}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy a stack.
 * Follow the Directions in the Project Documentation for this Function
 * If head is NULL, you can just return.
 */
void stack_destroy(Stack_head *head) {
  
  if(head->top == NULL){
    // when empty stack is provided
    return;
  }
  else{
    Node *walker = head->top;
    while(walker != NULL){
      // walking through each node and freeing it
      Node *next = walker->next;
      token_free(walker->tok);
      free(walker);
      walker = next;
    }
    head->top = NULL; //setting top to NULL
    // freeing up head and setting it to null
    free(head);
    head = NULL;
  }
  return;
}

/* (IMPLEMENT THIS FUNCTION)
 * Push a new Token on to the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return -1.
 * If there are no errors, return 0.
 */
int stack_push(Stack_head *stack, Token *tok) {
  // creating a new node
  Node *new_node = node_create(tok);
  if(new_node == NULL){
    return -1;
  }
  // adding the node onto the stack
  new_node->next = stack->top;
  stack->top = new_node;
  stack->count += 1;
  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Pop a Token off of the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * If the stack was empty, return NULL.
 */
Token *stack_pop(Stack_head *stack) {
  if(stack == NULL || stack->top == NULL){
    return NULL;
  }

  // popping stack and decrementing count
  Node *node_to_pop = stack->top;
  stack->top = stack->top->next;
  stack->count -= 1;

  // extracting token and freeing node
  Token *tok = node_to_pop->tok;
  node_free(node_to_pop);
  return tok;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the token in the stack node on the top of the stack
 * Follow the Directions in the Project Documentation for this Function
 * If the stack is NULL, return NULL.
 * If the stack is empty, return NULL.
 */
Token *stack_peek(Stack_head *stack) {
  if(stack == NULL || stack->top == NULL){
    return NULL;
  }
  return stack->top->tok;
}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of nodes in the stack.
 * Follow the Directions in the Project Documentation for this Function
 * If stack is NULL, return -1.
 * Return 0 if the stack is empty.
 */
int stack_is_empty(Stack_head *stack) {
  /* Implement this function */
  if(stack == NULL){
    return -1;
  }
  if(stack->top == NULL){
    return 1;
  }
  return 0;
}

/* These two functions are written for you.
 * It recurses the stack and prints out the tokens in reverse order
 * eg. top->2->4->1->8 will print at Stack: 8 1 4 2
 * eg. stack_push(5) will then print Stack: 8 1 4 2 5
 */

/* This is implemented for you.
 * Recursive print. (Local function)
 * Base Case: node == NULL, return
 * Recursive Case: call print_node(node->next, print_data), then print node.
 */
static void print_node(Node *node) {
  if(node == NULL) {
    return;
  }
  token_print(node->tok);
  print_node(node->next);
  return;
}

/* This is implemented for you.
 * Setup function for the recursive calls.  Starts printing with stack->top
 */
void stack_print(Stack_head *stack) {
  if(stack == NULL) {
    return;
  }
  printf("|-----Program Stack\n");
  printf("| ");
  print_node(stack->top);
  printf("\n");
  return;
}
