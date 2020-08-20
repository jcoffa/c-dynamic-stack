#ifndef DYNSTACK_H
#define DYNSTACK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************
 * STRUCTURES *
 **************/

/*
 * This stack implementation is a "Linked List Stack" (often abbreviated to "Linked Stack"),
 * which uses a singly-linked list to store a collection of elements.
 *
 * This approach, in contrast to the "Array Stack", is typically more memory efficient
 * for large stacks (because memory is allocated per-element instead of having lots of
 * empty space in an array) but slightly less runtime efficient on average due to the
 * expensiveness of allocating memory every time a new item is pushed.
 *
 * This structure represents a node in the linked stack that holds a single piece of
 * abstracted (void *) data.
 */
typedef struct dynamicStackFrame {
	void *data;
	struct dynamicStackFrame *next;
} DynFrame;

/*
 * Metadata top of the stack. 
 * Contains the function pointers for working with the abstracted stack data.
 */
typedef struct dynamicStack {
	DynFrame *top;				// Stack frame at the top of the stack
	unsigned int size;			// Number of stack frames in the stack
	void (*deleteData)(void *);	// Function pointer to free an element in the stack
	char *(*printData)(void *);	// Function pointer to create a string from a stack element
} DynStack;


/*************
 * FUNCTIONS *
 *************/

/*
 * Function to initialize the DynStack metadata head to the appropriate function pointers.
 * Allocates memory to the struct, unless any of the function pointers are NULL. In this
 * case, NULL is returned instead and no memory is allocated.
 *
 * The DynStack provides an interface to a generic collection of data. The two 
 * function pointers allow the struct to print and delete its data.
 * The (void *) arguments are to be casted into their proper data type (i.e. whatever
 * data type the stack will be holding) and do as follows:
 *
 *  char *printFunc(void *toPrint)  : return a string representation of `toPrint`
 *  void deleteFunc(void *toDelete) : free all memory associated with `toDelete`
 *
 * Examples of these functions are provided for string (char *) data in the README.
 */
DynStack *dynstackNew(void (*deleteFunc)(void *), char *(*printFunc)(void *));


/*
 * Allocates memory for a new DynFrame struct and returns a pointer to it.
 */
DynFrame *dynstackFrameNew(void *data);


/*
 * Removes every element from a DynStack without deleting the stack itself.
 */
void dynstackClear(DynStack *stack);


/*
 * Frees all memory associated with a DynStack, including the stack itself.
 */
void dynstackFree(DynStack *stack);


/*
 * Pushes the data to the top of the stack.
 * If the push operation can't be completed for whatever reason,
 * this function returns `false`. Otherwise, it returns `true` for a
 * successful push.
 *
 * `false` may be returned if `stack` is NULL, or if memory can't
 * be allocated to create a new DynStackFrame. Both of these scenarios
 * are incredibly unlikely; assuming proper use of the functions
 * in this library and sufficient memory is available to the system.
 */
bool dynstackPush(DynStack *stack, void *data);


/*
 * Returns the top of the stack without removing it.
 */
void *dynstackPeek(const DynStack *stack);


/*
 * Returns the top of the stack after removing it from the stack.
 */
void *dynstackPop(DynStack *stack);


/*
 * Returns the number of elements in the stack.
 */
unsigned int dynstackGetSize(const DynStack *stack);


/*
 * Returns true if the DynStack contains 0 elements, and false otherwise.
 */
bool dynstackIsEmpty(const DynStack *stack);


/*
 * Returns a string representing the element at the top of the DynStack using the stack's
 * `printData` function pointer to create the string.
 *
 * The string msut be freed by the calling function after use.
 */
char *dynstackTopToString(const DynStack *stack);


/*
 * A convenient alias for printing the string returned by `stackTopToString(stack)`
 * and then freeing the string that was created after printing it.
 * A newline is printed after the stack-element-string is done printing.
 */
void dynstackPrintTop(const DynStack *stack);


/*
 * Returns a string representing the DynStack using the stack's `printData` function pointer
 * to create the string, starting from the top of the stack and working downwards.
 *
 * The string must be freed by the calling function after use.
 */
char *dynstackToString(const DynStack *stack);


/*
 * A convenient alias for printing the string returned by `dynstackToString(stack)`
 * and then freeing the string that was created after printing it.
 * A newline is printed after the stack-string is done printing.
 */
void dynstackPrint(const DynStack *stack);


/*
 * Execute a function `func` on each node in the stack
 * starting from the top and working downwards.
 */
void dynstackMap(DynStack *stack, void (*func)(DynFrame *));

#endif	// DYNSTACK_H

