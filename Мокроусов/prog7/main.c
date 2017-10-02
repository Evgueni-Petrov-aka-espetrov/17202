#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {none, processing, complete} TVertexState;

typedef struct {
    unsigned short pos;
    unsigned short item[1000];
} TStack;

typedef struct {
    TStack vertex_connections;
    TVertexState state;
} TVertex;



void stack_push(TStack *stack, int val)
{
    stack->item[stack->pos++] = val;
}
int stack_pop(TStack *stack)
{
    if (!stack->pos) return -1;
    return stack->item[--stack->pos];
}

int find_vertex_with_state(TVertex *vert_arr, int vert_count, TVertexState state)
{
    int i;
    for (i=0;i<vert_count;i++)
        if (vert_arr[i].state==state) return i;
    return -1;
}
void error(FILE *in, FILE *out, void *verts, char *msg)
{
    if (verts!=NULL) free(verts);
    fprintf(out, msg);
    fclose(in);
    fclose(out);
    exit(0);
}
void find_topological_order(FILE *in, FILE *out, TVertex *vert_arr, int vert_count, int vert_id, TStack *stack)
{
    switch (vert_arr[vert_id].state)
    {
        case none: vert_arr[vert_id].state=processing; break;
        case processing: error(in, out, vert_arr, "impossible to sort"); break;
        case complete: return;
    }

    int temp;
    while ((temp=stack_pop(&vert_arr[vert_id].vertex_connections))!=-1)
        find_topological_order(in, out, vert_arr, vert_count, temp, stack);
    stack_push(stack, vert_id);
    vert_arr[vert_id].state = complete;
}
int main()
{
    unsigned int n, m, i;
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    if (fscanf(in, "%d\n", &n)==EOF) error(in, out, NULL, "bad number of lines");
    TVertex *verts = malloc(sizeof(TVertex)*n);
    if (verts==NULL) return -1;
    memset(verts, 0, sizeof(TVertex)*n);
    if (fscanf(in, "%d\n", &m)==EOF)
        error(in, out, verts, "bad number of lines");
    if (n<0 || n>1000) error(in, out, verts, "bad number of vertices");
    if (m<0 || m>(n*(n-1)/2)) error(in, out, verts, "bad number of edges");
    for (i=0;i<m;i++)
    {
        int ai,bi;
        if (fscanf(in, "%d %d", &ai, &bi)==EOF)
            error(in, out, verts, "bad number of lines");
        if (ai<0 || ai>n) error(in, out, verts, "bad vertex");
        if (bi<0 || bi>n) error(in, out, verts, "bad vertex");
        stack_push(&(verts[ai-1].vertex_connections), bi-1);
    }
    TStack stack;
    stack.pos = 0;
    while (1)
    {
        int new_start_vert = find_vertex_with_state(verts, n, none);
        if (new_start_vert==-1) break;
        find_topological_order(in, out, verts, n, new_start_vert, &stack);
    };
    int item;
    while ((item = stack_pop(&stack))!=-1)
        fprintf(out, "%d ", item+1);
    fclose(in);
    fclose(out);
    return 0;
}
