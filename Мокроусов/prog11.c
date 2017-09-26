#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PATTERN_SIZE 16

typedef enum {false, true} bool;

unsigned int pow3s[MAX_PATTERN_SIZE];



bool cmp_elems(FILE *out, int pos_buf, int pos_pattern, int pat_len, unsigned char *buf, unsigned char *pattern)
{
    fprintf(out, " %d", pos_buf+1);
    return (buf[pos_buf % pat_len]==pattern[pos_pattern]);
}
int readln(FILE *f, unsigned char *str_array)
{
    int res = 0;
    while (true)
    {
        int ch = fgetc(f);
        if ((ch==-1) || (ch=='\n')) break;
        *str_array = ((char) ch);
        str_array++;
        res++;
    }
    return res;
}
long hash_chr(unsigned char ch, int pos)
{
    return (ch % 3)*pow3s[pos];
}
long hash_substr(int from, int to, unsigned char *string)
{
    long hash = 0;
    int i;
    for (i=from;i<to;i++)
    {
        hash+=hash_chr(string[i], i);
    };
    return hash;
}

bool cmp_substr(FILE *out, int from, int pat_len, unsigned char *buf, unsigned char *pattern)
{
    int i;
    for (i=0;i<pat_len;i++)
        if (!cmp_elems(out, i+from, i, pat_len, buf, pattern )) return false;
    return true;
}
int main()
{
    int i;
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");

    unsigned char pattern[MAX_PATTERN_SIZE];
    unsigned char buf[MAX_PATTERN_SIZE];
    int pat_len, cur_pos;

    pat_len = readln(in, pattern);
    cur_pos = 0;
    int tmp_ch;
    pow3s[0] = 1;
    for (i=1;i<16;i++)
    {
        pow3s[i] = pow3s[i-1]*3;
    }

    while ((cur_pos<pat_len) &&((tmp_ch = fgetc(in))!=EOF))
    {
        buf[cur_pos++] = tmp_ch;
    }
    long hash_pattern = hash_substr(0, pat_len, pattern);
    fprintf(out, "%ld", hash_pattern);
    if (cur_pos<pat_len) goto fin;
    long temp_hash = hash_substr(0, pat_len, buf);

    if (temp_hash==hash_pattern)
            cmp_substr(out, 0, pat_len, buf, pattern);
    int curpos_mod_patlen = 0;
    for (cur_pos=pat_len;(tmp_ch = fgetc(in))!=EOF;cur_pos++)
    {
        temp_hash -= buf[curpos_mod_patlen] % 3;
        temp_hash /= 3;
        buf[curpos_mod_patlen] = tmp_ch;
        temp_hash += (buf[curpos_mod_patlen] % 3)*pow3s[pat_len-1];
        if (temp_hash==hash_pattern)
            cmp_substr(out, cur_pos-pat_len+1, pat_len, buf, pattern);
        if (++curpos_mod_patlen==pat_len) curpos_mod_patlen = 0;
    };
    fin:
    fclose(in);
    fclose(out);
    return 0;
}
