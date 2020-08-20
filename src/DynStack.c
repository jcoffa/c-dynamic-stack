#include "DynStack.h"


DynStack *dynstackNew(void (*deleteFunc)(void *), char *(*printFunc)(void *)) {
	if (deleteFunc == NULL || printFunc == NULL) {
		return NULL;
	}

	DynStack *toReturn = malloc(sizeof(DynStack));

	// Can't assume malloc works every time, no matter how unlikely
	if (toReturn == NULL) {
		return NULL;
	}

	toReturn->top = NULL;
	toReturn->size = 0;
	toReturn->deleteData = deleteFunc;
	toReturn->printData = printFunc;

	return toReturn;
}


DynFrame *dynstackFrameNew(void *data) {
	DynFrame *toReturn = malloc(sizeof(DynFrame));

	// Can't assume malloc works every time, no matter how unlikely
	if (toReturn == NULL) {
		return NULL;
	}

	toReturn->data = data;
	toReturn->next = NULL;

	return toReturn;
}


void dynstackClear(DynStack *stack) {
	if (stack == NULL) {
		return;
	}

	while (!dynstackIsEmpty(stack)) {
		// stackPop already removes and frees the stack DynFrame struct,
		// so we only have to delete the frame's stored data
		stack->deleteData(dynstackPop(stack));
	}
}


void dynstackFree(DynStack *stack) {
	if (stack == NULL) {
		return;
	}

	dynstackClear(stack);
	free(stack);
}


bool dynstackPush(DynStack *stack, void *data) {
	if (stack == NULL) {
		return false;
	}

	DynFrame *toPush = dynstackFrameNew(data);

	// Can't assume malloc works every time, no matter how unlikely
	if (toPush == NULL) {
		return false;
	}

	toPush->next = stack->top;
	stack->top = toPush;
	(stack->size)++;
	return true;
}


void *dynstackPeek(const DynStack *stack) {
	if (stack == NULL || stack->top == NULL) {
		return NULL;
	}

	return stack->top->data;
}


void *dynstackPop(DynStack *stack) {
	if (stack == NULL || stack->top == NULL) {
		return NULL;
	}

	// Save the top frame and its data
	DynFrame *top = stack->top;
	void *toReturn = top->data;

	// Move the stack pointer
	stack->top = stack->top->next;
	(stack->size)--;

	// Free the removed frame and return its data
	free(top);
	return toReturn;
}


unsigned int dynstackGetSize(const DynStack *stack) {
	if (stack == NULL) {
		return 0;
	}
	return stack->size;
}


bool dynstackIsEmpty(const DynStack *stack) {
	return dynstackGetSize(stack) == 0;
}


char *dynstackTopToString(const DynStack *stack) {
	if (stack == NULL) {
		return NULL;
	}

	char *toReturn;
	if (dynstackIsEmpty(stack)) {
		toReturn = malloc(sizeof(char));
		toReturn[0] = '\0';
	} else {
		toReturn = stack->printData(stack->top->data);
	}

	return toReturn;
}


void dynstackPrintTop(const DynStack *stack) {
	if (stack == NULL) {
		return;
	}

	char *toPrint = dynstackTopToString(stack);
	printf("%s\n", toPrint);
	free(toPrint);
}


char *dynstackToString(const DynStack *stack) {
	if (stack == NULL) {
		return NULL;
	}

	// Start with the top of the stack
	char *toReturn = dynstackTopToString(stack);
	size_t length = strlen(toReturn);

	// Prepare to iterate over the rest of the stack beyond the top
	DynFrame *cur;
	if (dynstackIsEmpty(stack)) {
		// If the stack is empty then `cur = stack->top->next`
		// will error out or segfault since `stack->top` is NULL,
		// so this guard is necessary
		cur = NULL;
	} else {
		cur = stack->top->next;
	}

	while (cur != NULL) {
		char *frameStr = stack->printData(cur->data);
		length += strlen(frameStr) + 1;	// +1 for newline
		toReturn = realloc(toReturn, length + 1);	// +1 for null terminator
		strcat(toReturn, "\n");
		strcat(toReturn, frameStr);
		free(frameStr);

		cur = cur->next;
	}

	return toReturn;
}


void dynstackPrint(const DynStack *stack) {
	if (stack == NULL) {
		return;
	}

	char *toPrint = dynstackToString(stack);
	printf("%s\n", toPrint);
	free(toPrint);
}


void dynstackMap(DynStack *stack, void (*func)(void *)) {
	if (stack == NULL || dynstackIsEmpty(stack)) {
		return;
	}

	DynFrame *cur = stack->top;
	while (cur != NULL) {
		func(cur->data);
		cur = cur->next;
	}
}

