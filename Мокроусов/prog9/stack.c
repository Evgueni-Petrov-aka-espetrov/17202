#include "stack.h"

void stack_push(TStack *stack, int item)
{
    stack->items[stack->pos++] = item;
}
int stack_pop(TStack *stack)
{
    if (!stack->pos) return -1;
    return stack->items[--stack->pos];
}
