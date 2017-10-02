#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {none, processing, complete} TVertexState;
typedef struct {
    unsigned short queque_start;
    unsigned short queque_end;
    unsigned short connections_to[1000];
    TVertexState state;
} TVertex;

typedef struct {
    unsigned short pos;
    unsigned short item[1000];
} TStack;

void stack_push(TStack *stack, int val)
{
    stack->item[stack->pos++] = val;
}
int stack_pop(TStack *stack)
{
    if (!stack->pos) return -1;
    return stack->item[--stack->pos];
}

void vertex_queque_push(TVertex *vert, int val)
{
    vert->connections_to[vert->queque_end++] = val;
}
int vertex_queque_pop(TVertex *vert)
{
    if (vert->queque_start==vert->queque_end) return -1;
    return vert->connections_to[vert->queque_start++];
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
    if (vert_arr[vert_id].state==processing) error(in, out, vert_arr, "impossible to sort");
    vert_arr[vert_id].state=processing;
    int i;
    for (i=vert_arr[vert_id].queque_end;i<;i++)
        find_topological_order(in, out, vert_arr, vert_count, vert_arr[vert_id].connections_to[i], stack);
    stack_push(stack, vert_id);
    vert_arr[vert_id].state = complete;
}
int main()
{
    unsigned int n, m, i;
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");
    fscanf(in, "%d\n", &n);
    TVertex *verts = malloc(sizeof(TVertex)*n);
    if (verts==NULL) return -1;
    memset(verts, 0, sizeof(TVertex)*n);
    fscanf(in, "%d\n", &m);
    if (n<0 || n>1000) error(in, out, verts, "bad number of vertices");
    if (m<0 || m>(n*(n-1)/2)) error(in, out, verts, "bad number of edges");
    for (i=0;i<m;i++)
    {
        if (feof(in)) error(in, out, verts, "bad number of lines");
        int ai,bi;
        fscanf(in, "%d %d", &ai, &bi);
        if (ai<0 || ai>n) error(in, out, verts, "bad vertex");
        if (bi<0 || bi>n) error(in, out, verts, "bad vertex");
        vertex_queque_push(&verts[ai-1], bi-1);
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
        printf("%d ", item);
    fclose(in);
    fclose(out);
    return 0;
}
