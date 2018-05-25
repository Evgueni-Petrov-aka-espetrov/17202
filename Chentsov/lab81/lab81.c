#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
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
        if((M <= 0) || (M > N * (N - 1) / 2))
            if(M == 0){
                if(N != 1)
                    printf("no spanning tree");
            }
            else
                printf("bad number of edges");
        else{
            int j, c, d, e;
            unsigned *f = malloc(sizeof(unsigned) * N * (N - 1) / 2);
            for(i = 0;i < N * (N - 1) / 2;++ i)
                f[i] = UINT_MAX;
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
                f[(c < d) ? ((2 * N - c) * (c - 1) / 2 + d - c - 1) : ((2 * N - d) * (d - 1) / 2 + c - d - 1)] = e;
            }
            if(i == M){
                int *g = malloc(sizeof(int) * N), *h = malloc(sizeof(int) * N);
                unsigned *l = malloc(sizeof(unsigned) * N);
                g[0] = 0;
                h[0] = 1;
                l[0] = 0;
                for(i = 1;i < N;++ i){
                    g[i] = i + 1;
                    h[i] = 0;
                    l[i] = f[i - 1];
                }
                unsigned m;
                for(i = 1;i < N;++ i){
                    m = 0;
                    for(j = 1;j < N;++ j)
                        if((!(h[j])) && ((m == 0) || (l[j] < l[m])))
                            m = j;
                    if(l[m] == UINT_MAX){
                        printf("no spanning tree");
                        break;
                    }
                    h[m] = 1;
                    l[m] = 0;
                    if(g[m] == m + 1)
                        g[m] = 1;
                    for(j = 1;j < N;++ j)
                        if((!(h[j])) && (f[(m < j) ? ((2 * N - 1 - m) * m / 2 + j - m - 1) : ((2 * N - 1 - j) * j / 2 + m - j - 1)] < l[j])){
                            l[j] = f[(m < j) ? ((2 * N - 1 - m) * m / 2 + j - m - 1) : ((2 * N - 1 - j) * j / 2 + m - j - 1)];
                            g[j] = m + 1;
                        }
                }
                if(i == N)
                    for(i = 2;i <= N;++ i)
                        printf("%i %i\n",g[i - 1],i);
            }
        }
    }
    fclose(b);
    return 0;
}
