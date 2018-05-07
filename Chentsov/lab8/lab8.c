#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
struct edge{
    short beginning, ending;
    int length;
    struct edge *next;
};
int main_element(int q, int *r){
    if(q == r[q - 1])
        return q;
    r[q - 1] = main_element(r[q - 1],r);
    return r[q - 1];
}
int main(){
    char a[22];
    a[0] = '\0';
    FILE *b;
    b = fopen("in.txt","r");
    fgets(a,22,b);
    int i = 0, N = 0;
    while((a[i] <= '9') && (a[i] >= '0')){
        N *= 10;
        N += a[i] - '0';
        ++ i;
    }
    if((N < 0) || (N > 5000))
        printf("bad number of vertices");
    else{
        a[0] = '\0';
        fgets(a,22,b);
        i = 0;
        int M = 0;
        while((a[i] <= '9') && (a[i] >= '0')){
            M *= 10;
            M += a[i] - '0';
            ++ i;
        }
        if((M < 0) || (M > N * (N - 1) / 2))
            printf("bad number of edges");
        else{
            int j, c, d;
            unsigned e;
            struct edge **f;
            struct edge *g, *h = NULL;
            for(i = 0;i < M;++ i){
                if(feof(b)){
                    printf("bad number of lines");
                    break;
                }
                a[0] = '\0';
                fgets(a,22,b);
                if(a[0] == '\0'){
                    printf("bad number of lines");
                    break;
                }
                j = 0;
                c = 0;
                while((a[j] <= '9') && (a[j] >= '0')){
                    c *= 10;
                    c += a[j] - '0';
                    ++ j;
                }
                if((c < 1) || (c > N)){
                    printf("bad vertex");
                    break;
                }
                ++ j;
                d = 0;
                while((a[j] <= '9') && (a[j] >= '0')){
                    d *= 10;
                    d += a[j] - '0';
                    ++ j;
                }
                if((d < 1) || (d > N)){
                    printf("bad vertex");
                    break;
                }
                ++ j;
                e = 0;
                if(a[j] == '-'){
                    printf("bad length");
                    break;
                }
                while((a[j] <= '9') && (a[j] >= '0') && (e <= INT_MAX)){
                    e *= 10;
                    e += a[j] - '0';
                    ++ j;
                }
                if((e < 0) || (e > INT_MAX)){
                    printf("bad length");
                    break;
                }
                f = &h;
                while((*f) && ((*f)->length < e))
                    f = &((*f)->next);
                g = malloc(sizeof(struct edge));
                g->beginning = c;
                g->ending = d;
                g->length = e;
                g->next = *f;
                *f = g;
            }
            if(i == M){
                int *l = malloc(sizeof(int) * N);
                int *m = malloc(sizeof(int) * N);
                for(i = 0;i < N;++ i){
                    l[i] = i + 1;
                    m[i] = 0;
                }
                g = h;
                int *n = malloc(sizeof(int) * (N - 1));
                int *p = malloc(sizeof(int) * (N - 1));
                int s, t;
                for(i = 0;i < N - 1;++ i){
                    while((g) && (main_element(g->beginning,l) == main_element(g->ending,l)))
                        g = g->next;
                    if(!(g))
                        break;
                    n[i] = g->beginning;
                    p[i] = g->ending;
                    s = main_element(g->beginning,l);
                    t = main_element(g->ending,l);
                    if(m[s] < m[t])
                        l[s - 1] = t;
                    else{
                        l[t - 1] = s;
                        if(m[s] == m[t])
                            ++ m[s];
                    }
                    g = g->next;
                }
                while(h){
                    g = h;
                    h = h->next;
                    free(g);
                }
                if(i + 1 == N){
                    for(i = 0;i < N - 1;++ i)
                        printf("%i %i\n",n[i],p[i]);
                }
                else
                    printf("no spanning tree");
            }
        }
    }
    fclose(b);
    return 0;
}
