#include <stdio.h>
#include <string.h>

void make_stops(int *stops, int len_templ, const unsigned char *templ){
    int i;
    for(i = 0; i < 256; stops[i++] = -1);
    for(i = 0; i < len_templ - 1; stops[templ[i]] = i, i++);
}

void make_suffix(int *suffix, int len_templ, const unsigned char *templ){
    int i;
    for(i = 1; i <= len_templ; ++i){
        suffix[i - 1] = shift(len_templ,i,templ);
    }
}

int shift(int len_templ, int i, const unsigned char *templ){
    int j, a, state;

    for(j = 1;j <= len_templ; ++j){
        state = 1;

        for(a = len_templ - 1; a>= len_templ - i; --a)
            if(a - j >= 0)
                if(templ[a] != templ[a - j])
                    state = 0;
        if(len_templ - i - j - 1 >= 0)
            if(templ[len_templ - i - 1] == templ[len_templ - i - j - 1])
                state = 0;

        if(state>0){
            return j;
        }
    }
    return 0;
}


void search(FILE *in, int len_templ, const unsigned char *templ, const int *stops,const int *suffix){
    int len_string = 0, j = 0, length = 0, i;
    unsigned char string[256];
    string[0] = '\0';

    while(!feof(in)){
        fgets(string + len_string,256,in);
        len_string = strlen(string);
        i = len_templ - 1;

        while((i + j < len_string) && (len_string > 0)){
            if(i >= 0) {
                while(templ[i] == string[i + j]){
                    printf("%i ", length + i + j + 1);
                    --i;
                }
            }
            if(i == len_templ - 1){
                printf("%i ",length + i + j + 1);
                j += i - stops[string[i + j]];
            }
            else {
                if(i > -1){
                    printf("%i ",length + i + j + 1);
                    j += suffix[len_templ - 1 - i];
                    i = len_templ - 1;
                }
                else{
                    i = len_templ - 1;
                    j += suffix[len_templ - 1];
                }
            }
        }
        for(i = 0;i < len_templ; ++i)
            string[i] = string[len_string - len_templ + i];

        length += len_string - len_templ;
        j -= len_string - len_templ;
        len_string = len_templ;
    }
}
int main(){
    FILE *in = fopen("in.txt","r");

    unsigned char templ[16];

    fgets(templ,18,in);
    int len_templ = strlen(templ) - 1;

    int stops[256];

    make_stops(stops,len_templ,templ);

    int suffix[16];

    make_suffix(suffix, len_templ,templ);
    search(in,len_templ,templ,stops,suffix);
    fclose(in);

    return 0;
}
