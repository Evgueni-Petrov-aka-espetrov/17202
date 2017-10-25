#include <stdio.h>
#include <stdlib.h>

#define MAX_PERMUT_LEN 10
#define DIGIT_COUNT 10

typedef enum {false, true} bool;
int readln(FILE *f, char *str_array)
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

bool check_permut(char *permut, int permut_len)
{
    bool was_value[DIGIT_COUNT];
    int i;
    for (i=0;i<DIGIT_COUNT;i++)
        was_value[i] = false;
    for (i=0;i<permut_len;i++)
    {
        if ((permut[i]<'0') || (permut[i]>'9')) return false;
        if (was_value[permut[i]-'0'])
            return false;
        else was_value[permut[i]-'0']=true;
    }
    return true;
}

void swap(char *permut, int i, int j)
{
  int s = permut[i];
  permut[i] = permut[j];
  permut[j] = s;
}
bool gen_next_permut(char *permut, int permut_len)
{
  int j = permut_len - 2;
  while (j != -1 && permut[j] >= permut[j + 1]) j--;
  if (j == -1)
    return false;
  int k = permut_len - 1;
  while (permut[j] >=permut[k]) k--;
  swap(permut, j, k);
  int l = j + 1, r = permut_len - 1;
  while (l<r)
    swap(permut, l++, r--);
  return true;
};

void print_permut(FILE *out, const char *permut, int permut_len)
{
  while (permut_len--)
      fputc(*permut++, out);
  fputc('\n', out);
}
int main()
{
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    char permut[MAX_PERMUT_LEN];
    int permut_len = readln(in, permut);
    if (!check_permut(permut, permut_len))
    {
        fputs("bad input", out);
        return 0;
    }
    int n, i;
    fscanf(in, "%d\n", &n);
    for (i=0;i<n;i++)
    {
        if (!gen_next_permut(permut, permut_len)) break;
        print_permut(out, permut, permut_len);
    }
    fclose(in);
    fclose(out);
    return 0;
}
