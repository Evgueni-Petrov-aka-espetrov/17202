#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct
{
    int was_used;
    long long distance;
    int prev_node;
    int has_only_way;
} TVertex;
void error(FILE *in, FILE *out, char *msg, void *malloc1, void *malloc2)
{
    if (malloc1) free(malloc1);
    if (malloc2) free(malloc2);
    fprintf(out, msg);
    fclose(in);
    fclose(out);
    exit(0);
}

void read_connectivity_map(FILE *in, FILE *out, long long *connectivity_map, int n, int m)
{
    int i;
    for (i=0;i<n*n;i++)
        connectivity_map[i] = -1;
    for (i=0;i<m;i++)
    {
        int v1, v2;
        long long len;
        int temp = fscanf(in, "%d %d %I64d\n", &v1, &v2, &len);
        if (temp==EOF) error(in, out, "bad number of lines", NULL, NULL);
        if (len>INT_MAX || len < 0) error(in, out, "bad length", NULL, NULL);
        if (v1<1 || v1>n) error(in, out, "bad vertex", NULL, NULL);
        if (v2<1 || v2>n) error(in, out, "bad vertex", NULL, NULL);
        v1--;
        v2--;
        connectivity_map[v1*n+v2] = len;
        connectivity_map[v2*n+v1] = len;
    }
}

int get_least_node(TVertex *nodes, int node_count)
{
    int i, min_dist_index = -1;
    long long min_dist = 1;
    int was_set_min_dist = 0;
    for (i=0;i<node_count;i++)
        if ((nodes[i].was_used==0) && (nodes[i].distance!=-1) && ((!was_set_min_dist) || (nodes[i].distance<=min_dist)))
        {
            min_dist_index = i;
            min_dist = nodes[i].distance;
            was_set_min_dist = 1;
        };
    return was_set_min_dist ? min_dist_index : -1;
}

void print_distances(FILE *out, TVertex *nodes, int node_count)
{
    int i;
    for (i=0;i<node_count;i++)
    {
        long long temp = nodes[i].distance;
        if (temp==-1)
            fprintf(out, "oo ");
        else if (temp>INT_MAX)
            fprintf(out, "INT_MAX+ ");
        else
            fprintf(out, "%I64d ", temp);
    }
    fputc('\n', out);
}

void find_shortest_path(FILE *out, TVertex *nodes, int finish_node, int start_node)
{
    int buf[5001];
    int i, pos = 0;
    if (nodes[finish_node].distance==-1)
    {
        fprintf(out, "no path");
        return;
    }
    while (finish_node!=start_node)
    {
        buf[pos++] = finish_node+1;
        if ((nodes[finish_node].distance>INT_MAX) && (!(nodes[finish_node].has_only_way)))
        {
            fprintf(out, "overflow");
            return;
        }
        finish_node = nodes[finish_node].prev_node;
    };
    for (i=0;i<pos;i++)
        fprintf(out, "%d ", buf[i]);
    fprintf(out, "%d ", start_node+1);

}
int main()
{
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    int n, s, f, i, m;
    int temp = fscanf(in, "%d\n", &n);
    if (temp==EOF)
        error(in, out, "bad number of lines", NULL, NULL);
    if (n<0 || n>5000)
        error(in, out, "bad number of vertices", NULL, NULL);
    temp = fscanf(in, "%d %d\n", &s, &f);
    if (temp==EOF)
        error(in, out, "bad number of lines", NULL, NULL);
    if (s<1 || s>n)
        error(in, out, "bad vertex", NULL, NULL);
    if (f<1 || f>n)
        error(in, out, "bad vertex", NULL, NULL);
    s--;
    f--;
    temp = fscanf(in, "%d\n", &m);
    if (temp==EOF)
        error(in, out, "bad number of lines", NULL, NULL);
    if (m<0 || m>n*(n-1)/2)
        error(in, out, "bad number of edges", NULL, NULL);
    long long *connectivity_map = malloc(sizeof(long long)*n*n);
    read_connectivity_map(in, out, connectivity_map, n, m);
    if (n==0)
        error(in, out, "", NULL, NULL);
    TVertex *nodes = (TVertex*) malloc(sizeof(TVertex)*n);
    for (i=0;i<n;i++)
    {
        nodes[i].distance = nodes[i].prev_node = -1;
        nodes[i].was_used = 0;
    }
    nodes[s].distance = 0;
    while (1)
    {
        int cur_least_node = get_least_node(nodes, n);
        if (cur_least_node==-1) break;
        for (i=0;i<n;i++)
             if (connectivity_map[cur_least_node*n+i]!=-1)
            {
                if ((nodes[i].distance==-1) || (nodes[cur_least_node].distance+connectivity_map[cur_least_node*n+i]<nodes[i].distance))
                {
                    nodes[i].distance = nodes[cur_least_node].distance+connectivity_map[cur_least_node*n+i];
                    nodes[i].prev_node = cur_least_node;
                    nodes[i].has_only_way = 1;
                    continue;
                }
                if (nodes[cur_least_node].distance+connectivity_map[cur_least_node*n+i]==nodes[i].distance)
                {
                    nodes[i].has_only_way = 0;
                    continue;
                }
            }
        nodes[cur_least_node].was_used = 1;
    }
    print_distances(out, nodes, n);
    find_shortest_path(out, nodes, f, s);
    error(in, out, "", connectivity_map, nodes);
    return 0;
}
