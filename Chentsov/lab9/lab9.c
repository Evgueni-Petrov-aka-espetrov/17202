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
        int S = 0, F = 0;
        while((a[i] <= '9') && (a[i] >= '0')){
            S *= 10;
            S += a[i] - '0';
            ++ i;
        }
        if((S < 1) || (S > N))
            printf("bad vertex");
        else{
            ++ i;
            while((a[i] <= '9') && (a[i] >= '0')){
                F *= 10;
                F += a[i] - '0';
                ++ i;
            }
            if((F < 1) || (F > N))
                printf("bad vertex");
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
                        if(c != d)
                            f[(c < d) ? ((2 * N - c) * (c - 1) / 2 + d - c - 1) : ((2 * N - d) * (d - 1) / 2 + c - d - 1)] = e;
                    }
                    if(i == M){
                        int *g = malloc(sizeof(int) * N), *h = malloc(sizeof(int) * N);
                        unsigned *l = malloc(sizeof(unsigned) * N);
                        for(i = 0;i < N;++ i){
                            g[i] = i + 1;
                            h[i] = 0;
                            l[i] = f[(S <= i) ? ((2 * N - S) * (S - 1) / 2 + i - S) : ((2 * N - 1 - i) * i / 2 + S - i - 2)];
                        }
                        g[S - 1] = 0;
                        h[S - 1] = 1;
                        l[S - 1] = 0;
                        int m;
                        for(i = 1;i < N;++ i){
                            m = -1;
                            for(j = 0;j < N;++ j)
                                if((!(h[j])) && ((m == -1) || (l[j] < l[m])))
                                    m = j;
                            if(l[m] == UINT_MAX){
                                break;
                            }
                            h[m] = 1;
                            if(g[m] == m + 1)
                                g[m] = S;
                            for(j = 0;j < N;++ j)
                                if((!(h[j])) && (f[(m < j) ? ((2 * N - 1 - m) * m / 2 + j - m - 1) : ((2 * N - 1 - j) * j / 2 + m - j - 1)] != UINT_MAX)){
                                    if((l[m] <= INT_MAX - f[(m < j) ? ((2 * N - 1 - m) * m / 2 + j - m - 1) : ((2 * N - 1 - j) * j / 2 + m - j - 1)]) && (l[m] + f[(m < j) ? ((2 * N - 1 - m) * m / 2 + j - m - 1) : ((2 * N - 1 - j) * j / 2 + m - j - 1)] < l[j])){
                                        l[j] = l[m] + f[(m < j) ? ((2 * N - 1 - m) * m / 2 + j - m - 1) : ((2 * N - 1 - j) * j / 2 + m - j - 1)];
                                        g[j] = m + 1;
                                    }
                                    else{
                                        if((l[j] == UINT_MAX) && (l[m] <= (unsigned)INT_MAX + 1)){
                                            l[j] = (unsigned)INT_MAX + 1;
                                            g[j] = m + 1;
                                        }
                                        else
                                            if(((l[j] == (unsigned)INT_MAX + 1) && (l[m] <= (unsigned)INT_MAX + 1)) || ((l[j] == UINT_MAX) && (l[m] == (unsigned)INT_MAX + 2)))
                                                l[j] = (unsigned)INT_MAX + 2;
                                    }
                                }
                        }
                        for(i = 0;i < N;++ i){
                            if(h[i]){
                                if(l[i] > INT_MAX)
                                    printf("INT_MAX+ ");
                                else
                                    printf("%u ",l[i]);
                            }
                            else
                                printf("oo ");
                        }
                        printf("\n");
                        free(f);
                        if(h[F - 1]){
                            if(l[F - 1] == (unsigned)INT_MAX + 2)
                                printf("overflow");
                            else{
                                while(g[F - 1] != 0){
                                    printf("%i ",F);
                                    F = g[F - 1];
                                }
                                printf("%i ",F);
                            }
                        }
                        else
                            printf("no path");
                        free(g);
                        free(h);
                        free(l);
                    }
                }
            }
        }
    }
    fclose(b);
    return 0;
}
