#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

typedef struct {
    unsigned char node_char;
    int node_weight;
    short childs[2];
} tree_node;

typedef struct {
    unsigned char *data;
    int left, right;
} queue;

typedef struct {
    int count_digits;
    unsigned char data[256];
} bitseq;

#endif // MAIN_H_INCLUDED
