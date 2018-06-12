#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define BITS_IN_BYTE 8
#define MAX_USED_SIMBOLS_AMOUNT 256
#define FINAL_BITS_NUMBER 3
#define IN "in.txt"
struct graph{
    unsigned char simbol;
    int multiplicity;
    struct graph *next, *left, *right;
};
struct graph *graph_ctor(unsigned char simbol, int multiplicity, struct graph *next, struct graph *left, struct graph *right){
    struct graph *new_vertex = malloc(sizeof(struct graph));
    new_vertex->simbol = simbol;
    new_vertex->multiplicity = multiplicity;
    new_vertex->next = next;
    new_vertex->left = left;
    new_vertex->right = right;
    return new_vertex;
}
unsigned char *output_buffer_update(unsigned char *k, const unsigned char *buffer, FILE *out, char s){
    if(k < buffer)
        return k + 1;
    k -= BITS_IN_BYTE - 1;
    int i;
    unsigned char simbol = 0;
    for(i = 0;i < BITS_IN_BYTE;++ i)
        simbol += k[i] * powf(2,BITS_IN_BYTE - 1 - i);
    fwrite(&simbol,sizeof(unsigned char),1,out);
    return k;
}
void tree_coding(const struct graph *root, unsigned char **k, unsigned char *buffer, FILE *out){
    if(root->left){
        **k = 1;
        *k = output_buffer_update(*k,buffer,out,'d');
        tree_coding(root->left,k,buffer,out);
        tree_coding(root->right,k,buffer,out);
        return;
    }
    **k = 0;
    *k = output_buffer_update(*k,buffer,out,'e');
    int i;
    unsigned char data = root->simbol;
    for(i = BITS_IN_BYTE - 1;i >= 0;-- i){
        **k = data / powf(2,i);
        *k = output_buffer_update(*k,buffer,out,'f');
        data %= (int)powf(2,i);
    }
}
void tree_update(struct graph *root, struct graph **codes){
    if(root->left){
        tree_update(root->left,codes);
        tree_update(root->right,codes);
        root->left->simbol = 0;
        root->right->simbol = 1;
        root->left->next = root;
        root->right->next = root;
        return;
    }
    codes[root->simbol] = root;
}
void simbol_coding(const struct graph *code, unsigned char **k, unsigned char *buffer, FILE *out){
    if(code->next->next){
        simbol_coding(code->next,k,buffer,out);
    }
    **k = code->simbol;
    *k = output_buffer_update(*k,buffer,out,'g');
}
struct tree{
    unsigned char simbol;
    struct tree *left, *right;
};
int input_buffer_update(unsigned char **k, unsigned char *buffer, FILE *in){
    if((*k) > buffer + BITS_IN_BYTE + 1){
        (*k) -= BITS_IN_BYTE;
        int i;
        for(i = BITS_IN_BYTE + 1;i > 1;-- i){
            buffer[i] = buffer[0] % 2;
            buffer[0] /= 2;
        }
        buffer[0] = buffer[1];
        fread(buffer + 1,sizeof(unsigned char),1,in);
        return 1;
    }
    return 0;
}
struct tree *tree_decoding(FILE *in, unsigned char **k, unsigned char *buffer){
    input_buffer_update(k,buffer,in);
    struct tree *new_vertex = malloc(sizeof(struct tree));
    if(!(**k)){
        ++ *k;
        int i;
        new_vertex->simbol = 0;
        for(i = BITS_IN_BYTE - 1;i >= 0;-- i){
            input_buffer_update(k,buffer,in);
            new_vertex->simbol += **k * powf(2,i);
            ++ *k;
        }
        new_vertex->left = NULL;
        new_vertex->right = NULL;
        return new_vertex;
    }
    ++ *k;
    new_vertex->left = tree_decoding(in,k,buffer);
    new_vertex->right = tree_decoding(in,k,buffer);
    return new_vertex;
}
int main(){
    FILE *in, *out;
    in = fopen(IN,"r");
    unsigned char simbol;
    fscanf(in,"%c",&simbol);
    if(simbol == 'c'){
        int i, bar_chart[MAX_USED_SIMBOLS_AMOUNT];
        for(i = 0;i < MAX_USED_SIMBOLS_AMOUNT;++ i)
            bar_chart[i] = 0;
        int length = 0;
        fscanf(in,"%c%c",&simbol,&simbol);
        FILE *help = fopen("in1.txt","w");
        while(!(feof(in))){
            ++ bar_chart[simbol];
            fprintf(help,"%c",simbol);
            fscanf(in,"%c",&simbol);
            ++ length;
        }
        fclose(help);
        fclose(in);
        if(length > 0){
            struct graph *j, *root;
            i = 0;
            while(bar_chart[i] == 0)
                ++ i;
            root = graph_ctor(i,bar_chart[i],NULL,NULL,NULL);
            int used_simbols_amount = 1;
            for(++ i;i < MAX_USED_SIMBOLS_AMOUNT;++ i)
                if(bar_chart[i] > 0){
                    j = root;
                    while((j->next) && (bar_chart[i] > j->next->multiplicity))
                        j = j->next;
                    j->next = graph_ctor(i,bar_chart[i],j->next,NULL,NULL);
                    ++ used_simbols_amount;
                }
            unsigned char buffer[BITS_IN_BYTE];
            for(i = 0;i < BITS_IN_BYTE;++ i)
                buffer[i] = 0;
            unsigned char *k = buffer;
            out = fopen("out.txt","wb");
            if(used_simbols_amount == 1){
                *k = simbol % 2;
                ++ k;
                simbol /= 2;
                fwrite(&simbol,sizeof(unsigned char),1,out);
                while(length){
                    *k = length % 2;
                    k = output_buffer_update(k,buffer + BITS_IN_BYTE - 1,out,'h');
                    length /= 2;
                }
                while(k > buffer){
                    *k = 0;
                    k = output_buffer_update(k,buffer + BITS_IN_BYTE - 1,out,'i');
                }
            }
            else{
                while(root->next){
                    j = root;
                    while((j->next) && (root->multiplicity + root->next->multiplicity > j->next->multiplicity))
                        j = j->next;
                    j->next = graph_ctor(0,root->multiplicity + root->next->multiplicity,j->next,root,root->next);
                    root = root->next->next;
                }
                tree_coding(root,&k,buffer + BITS_IN_BYTE - 1,out);
                struct graph *codes[MAX_USED_SIMBOLS_AMOUNT];
                for(i = 0;i < MAX_USED_SIMBOLS_AMOUNT;++ i)
                    codes[i] = NULL;
                tree_update(root,codes);
                in = fopen(IN,"r");
                fscanf(in,"%c%c%c",&simbol,&simbol,&simbol);
                while(!(feof(in))){
                    simbol_coding(codes[simbol],&k,buffer + BITS_IN_BYTE - 1,out);
                    fscanf(in,"%c",&simbol);
                }
                if(k - buffer > BITS_IN_BYTE - FINAL_BITS_NUMBER){
                    simbol = 0;
                    for(i = 0;i < BITS_IN_BYTE;++ i)
                        simbol += buffer[i] * powf(2,BITS_IN_BYTE - 1 - i);
                    fwrite(&simbol,sizeof(unsigned char),1,out);
                }
                simbol = 0;
                for(i = 0;i < BITS_IN_BYTE - FINAL_BITS_NUMBER;++ i)
                    simbol += buffer[i] * powf(2,BITS_IN_BYTE - 1 - i);
                simbol += (buffer - k + 2 * BITS_IN_BYTE - FINAL_BITS_NUMBER) % BITS_IN_BYTE;
                fwrite(&simbol,sizeof(unsigned char),1,out);
            }
            fclose(out);
        }
    }
    else{
        fclose(in);
        in = fopen(IN,"rb");
        unsigned char buffer[BITS_IN_BYTE + 2];
        fread(buffer,sizeof(unsigned char),1,in);
        fread(buffer,sizeof(unsigned char),1,in);
        fread(buffer,sizeof(unsigned char),1,in);
        fread(buffer,sizeof(unsigned char),1,in);
        out = fopen("out.txt","w");
        int eof;
        if(!(feof(in))){
            fread(buffer + 1,sizeof(unsigned char),1,in);
            unsigned char *k = buffer + BITS_IN_BYTE + 2;
            if(buffer[0] < MAX_USED_SIMBOLS_AMOUNT / 2){
                simbol = 2 * (buffer[0] % (MAX_USED_SIMBOLS_AMOUNT / 2)) + buffer[1] / (MAX_USED_SIMBOLS_AMOUNT / 2);
                input_buffer_update(&k,buffer,in);
                k = buffer + BITS_IN_BYTE + 2;
                input_buffer_update(&k,buffer,in);
                k = buffer + 3;
                int i = 0, length = 0;
                while(!(feof(in))){
                    input_buffer_update(&k,buffer,in);
                    length += *k * powf(2,i);
                    ++ k;
                    ++ i;
                }
                while(k < buffer + 10){
                    input_buffer_update(&k,buffer,in);
                    length += *k * powf(2,i);
                    ++ k;
                    ++ i;
                }
                for(i = 0;i < length;++ i)
                    fprintf(out,"%c",simbol);
            }
            else{
                struct tree *j, *root = tree_decoding(in,&k,buffer);
                while(1){
                    j = root;
                    while(j->left){
                        if(input_buffer_update(&k,buffer,in))
                            eof = (feof(in) != 0);
                        j = *k ? j->right : j->left;
                        ++ k;
                    }
                    fprintf(out,"%c",j->simbol);
                    if((eof) && (((buffer + BITS_IN_BYTE - FINAL_BITS_NUMBER + 2 + 2 * BITS_IN_BYTE - buffer[0] % BITS_IN_BYTE - k) % BITS_IN_BYTE == 0) && (buffer[0] % BITS_IN_BYTE + (BITS_IN_BYTE - FINAL_BITS_NUMBER + 1) * eof > BITS_IN_BYTE - FINAL_BITS_NUMBER)))
                        break;
                }
            }
        }
        fclose(out);
    }
    fclose(in);
    return 0;
}
