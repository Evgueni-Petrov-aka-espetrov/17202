#include <stdio.h>
#include <stdlib.h>
struct vertex{
    int number, number_of_incoming_edges;
    struct edge *edges;
    struct vertex *next;
};
struct edge{
    struct vertex *ending;
    struct edge *next;
};
int main(){
    FILE *a;
    a = fopen("in.txt","r");
    char m[14];
    m[0] = '\0';
    fgets(m,14,a);
    if((m[0] < '0') || (m[0] > '9'))
        printf("bad number of lines");
    else{
        int i = 0, N = 0;
        while((m[i] >= '0') && (m[i] <= '9')){
            N *= 10;
            N += m[i] - '0';
            ++ i;
        }
        if((N < 0) || (N > 1000))
            printf("bad number of vertices");
        else{
            m[0] = '\0';
            fgets(m,14,a);
            if((m[0] < '0') || (m[0] > '9'))
                printf("bad number of lines");
            else{
                i = 0;
                int M = 0;
                while((m[i] >= '0') && (m[i] <= '9')){
                    M *= 10;
                    M += m[i] - '0';
                    ++ i;
                }
                if((M < 0) || (M > N * (N - 1) / 2))
                    printf("bad number of edges");
                else{
                    int j, b, c;
                    struct vertex *d, *e, *f = NULL;
                    struct edge *g;
                    for(i = 0;i < M;++ i){
                        m[0] = '\0';
                        fgets(m,14,a);
                        if(m[0] == '-'){
                            printf("bad vertex");
                            break;
                        }
                        if((m[0] < '0') || (m[0] > '9')){
                            printf("bad number of lines");
                            break;
                        }
                        j = 0;
                        b = 0;
                        c = 0;
                        while((m[j] >= '0') && (m[j] <= '9')){
                            b *= 10;
                            b += m[j] - '0';
                            ++ j;
                        }
                        ++ j;
                        if(m[j] == '-'){
                            printf("bad vertex");
                            break;
                        }
                        while((m[j] >= '0') && (m[j] <= '9')){
                            c *= 10;
                            c += m[j] - '0';
                            ++ j;
                        }
                        if((b < 0) || (b > N) || (c < 0) || (c > N)){
                            printf("bad vertex");
                            break;
                        }
                        d = f;
                        while((d) && (d->number != b))
                            d = d->next;
                        if(!(d)){
                            d = malloc(sizeof(struct vertex));
                            d->number = b;
                            d->number_of_incoming_edges = 0;
                            d->edges = NULL;
                            d->next = f;
                            f = d;
                        }
                        e = f;
                        while((e) && (e->number != c))
                            e = e->next;
                        if(!(e)){
                            e = malloc(sizeof(struct vertex));
                            e->number = c;
                            e->number_of_incoming_edges = 0;
                            e->edges = NULL;
                            e->next = f;
                            f = e;
                        }
                        g = malloc(sizeof(struct edge));
                        g->ending = e;
                        g->next = d->edges;
                        d->edges = g;
                        ++ e->number_of_incoming_edges;
                    }
                    if(i == M){
                        d = f;
                        int n = 0;
                        while(d){
                            d = d->next;
                            ++ n;
                        }
                        int *h = malloc(sizeof(int) * N);
                        int *p = malloc(sizeof(int) * N);
                        for(i = 0;i < N;++ i)
                            p[i] = 1;
                        for(i = 0;i < n;++ i){
                            d = f;
                            while((d) && (d->number_of_incoming_edges > 0)){
                                e = d;
                                d = d->next;
                            }
                            if(!(d)){
                                printf("impossible to sort");
                                break;
                            }
                            h[i] = d->number;
                            p[d->number - 1] = 0;
                            if(d == f)
                                f = d->next;
                            else
                                e->next = d->next;
                            g = d->edges;
                            free(d);
                            struct edge *l;
                            while(g){
                                -- g->ending->number_of_incoming_edges;
                                l = g;
                                if(g)
                                    g = g->next;
                                free(l);
                            }
                        }
                        int k = 0;
                        for(j = n;j < N;++ j){
                            while(p[k] == 0)
                                ++ k;
                            h[j] = k + 1;
                            p[k] = 0;
                            ++ k;
                        }
                        if(i == n)
                            for(i = 0;i < N;++ i)
                                printf("%i ",h[i]);
                    }
                }
            }
        }
    }
    fclose(a);
    return 0;
}
