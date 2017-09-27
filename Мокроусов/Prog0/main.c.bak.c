#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define true 1
#define false 0
#define ALPHABET_SIZE 256
#define bool int

FILE *in, *out;
char stoptable[ALPHABET_SIZE];

char pattern[16];
char buf[1000];

int pat_len, buf_len, cur_pos;

bool cmp_elems(int pos_buf, int pos_pattern)
{
    fprintf(out, " %d", pos_buf+1);
    return (buf[pos_buf]==pattern[pos_pattern]);
}
int readln(FILE *f, char *str_array, bool do_stop_on_ln)
{
    int res = 0;
    while (true)
    {
        int ch = fgetc(f);
        if ((ch==-1)) break;
        if ((ch=='\n') && (do_stop_on_ln)) break;
        *str_array = ((char) ch);
        str_array++;
        res++;
    }
    return res;
}
long long hash_chr(unsigned char ch, int pos)
{
    return (ch % 3)*pow(3, pos);
}
long long hash_substr(int from, int to, char *pattern)
{
    long long hash = 0;
    int i;
    for (i=from;i<to;i++)
    {
        hash+=hash_chr(pattern[i], i);
    };
    return hash;
}

bool cmp_substr(int from)
{
    int i;
    for (i=0;i<pat_len;i++)
        if (!cmp_elems(i+from, i)) return false;
    return true;
}
int main()
{
    int i;
    in = fopen("in.txt", "r");
    out = fopen("out.txt", "w");
    pat_len = readln(in, pattern, true);
    buf_len = readln(in, buf, false);
    long long hash_pattern = hash_substr(0, pat_len, pattern);
    fprintf(out, "%d", hash_pattern);
    if (buf_len<pat_len) goto fin;
    long long temp_hash = hash_substr(0, pat_len, buf);

    if (temp_hash==hash_pattern)
            cmp_substr(0);
    for (i=pat_len;i<buf_len;i++)
    {
        temp_hash -= hash_chr(buf[i-pat_len], 0);
        temp_hash /= 3;
        temp_hash += hash_chr(buf[i], pat_len-1);
        if (temp_hash==hash_pattern)
            cmp_substr(i-pat_len+1);


    };
    fin:
    fclose(in);
    fclose(out);
    return 0;
}
