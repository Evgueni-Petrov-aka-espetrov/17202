#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTEX_COUNT 1000
typedef enum {none, processing, complete} TVertexState;

typedef struct {
    unsigned short pos;
    unsigned short *items;
} TStack;

typedef struct {
    TStack vertex_connections;
    TVertexState state;
} TVertex;


void error(FILE *in, FILE *out, TVertex *verts, int vert_count, char *msg)
{
    if (verts!=NULL)
    {
        int i;
        for (i=0;i<vert_count;i++)
            if (verts[i].vertex_connections.items!=NULL) free(verts[i].vertex_connections.items);
        free(verts);
    }
    fprintf(out, msg);
    fclose(in);
    fclose(out);
    exit(0);
}

void stack_push(FILE *in, FILE *out, TVertex *vert_arr, int vert_count, TStack *stack, int val)
{
    if (stack->pos % 20 == 0)
    {
        void *temp = malloc((stack->pos + 20) * sizeof(unsigned short));
        if (temp==NULL)
            error(in, out, vert_arr, vert_count, "");
        memcpy(temp, stack->items, stack->pos*sizeof(unsigned short));
        free(stack->items);
        stack->items = temp;
    }

    stack->items[stack->pos++] = val;
}
int stack_pop(TStack *stack)
{
    if (!stack->pos) return -1;
    return stack->items[--stack->pos];
}

int find_vertex_with_state(TVertex *vert_arr, int vert_count, TVertexState state)
{
    int i;
    for (i=0;i<vert_count;i++)
        if (vert_arr[i].state==state) return i;
    return -1;
}

void find_topological_order(FILE *in, FILE *out, TVertex *vert_arr, int vert_count, int vert_id, TStack *stack)
{
    switch (vert_arr[vert_id].state)
    {
        case none: vert_arr[vert_id].state=processing; break;
        case processing: error(in, out, vert_arr, vert_count, "impossible to sort"); break;
        case complete: return;
    }

    int temp;
    while ((temp=stack_pop(&vert_arr[vert_id].vertex_connections))!=-1)
        find_topological_order(in, out, vert_arr, vert_count, temp, stack);
    stack_push(in, out, vert_arr, vert_count, stack, vert_id);
    vert_arr[vert_id].state = complete;
}
int main()
{
    unsigned int n, m, i;
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    if (fscanf(in, "%d\n", &n)==EOF) error(in, out, NULL, 0, "bad number of lines");
    TVertex *verts = malloc(sizeof(TVertex)*n);
    if (verts==NULL) return -1;
    memset(verts, 0, sizeof(TVertex)*n);
    if (fscanf(in, "%d\n", &m)==EOF)
        error(in, out, verts, 0, "bad number of lines");
    if (n<0 || n>MAX_VERTEX_COUNT) error(in, out, verts, 0, "bad number of vertices");
    if (m<0 || m>(n*(n-1)/2)) error(in, out, verts, 0, "bad number of edges");
    for (i=0;i<m;i++)
    {
        int ai,bi;
        if (fscanf(in, "%d %d", &ai, &bi)==EOF)
            error(in, out, verts, n, "bad number of lines");
        if (ai<0 || ai>n) error(in, out, verts, n, "bad vertex");
        if (bi<0 || bi>n) error(in, out, verts, n, "bad vertex");
        stack_push(in, out, verts, n, &(verts[ai-1].vertex_connections), bi-1);
    }
    TStack stack;
    stack.pos = 0;
	stack.items = NULL;
    while (1)
    {
        int new_start_vert = find_vertex_with_state(verts, n, none);
        if (new_start_vert==-1) break;
        find_topological_order(in, out, verts, n, new_start_vert, &stack);
    }
    int item;
    while ((item = stack_pop(&stack))!=-1)
        fprintf(out, "%d ", item+1);
    fclose(in);
    fclose(out);
    return 0;
}
