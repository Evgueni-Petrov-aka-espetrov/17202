//#ifndef STACK_H_INCLUDED
//#define STACK_H_INCLUDED

typedef struct {
    unsigned short pos;
    unsigned short items[5000];
} TStack;


void stack_push(TStack *stack, int item);
int stack_pop(TStack *stack);

//#endif // STACK_H_INCLUDED
