#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char node_char;
    int node_weight;
    int childs[2];
} tree_node;

typedef struct {
    char *data;
    int left, right;
} queue;

typedef struct {
    int count_digits;
    char data[256];
} bitseq;
void bubble_sort(tree_node *arr, int len)
{
    int i, j;
    for (i=0;i<len;i++)
        for (j=i+1;j<len;j++)
            if (arr[i].node_weight>arr[j].node_weight)
            {
                tree_node item = arr[i];
                arr[i] = arr[j];
                arr[j] = item;
            }
}
long count_frequencies(FILE *in, tree_node *freq_arr)
{
    int i = 0;
    for (i=0;i<255;i++)
    {
        freq_arr[i].node_char = (char) i;
        freq_arr[i].childs[1] = freq_arr[i].childs[0] = -1;
        freq_arr[i].node_weight = 0;
    }
    int temp;
    long result = 0;
    while ((temp = fgetc(in))!=EOF)
    {
        freq_arr[(char) temp].node_weight++;
        result++;
    }
    return result;
}

int genHaffmanTree(queue *q1, queue *q2, tree_node *haffman_tree)
{
    while ((q1->right-q1->left)+(q2->right-q2->left)>=2)
    {
        int node1 = -1;
        int node2 = -1;
        if (q1->left<q1->right)
            node1 = q1->left++;
        if (q2->left<q2->right)
            node2 = q2->left++;
        if ((node1==-1) || ((q2->left<q2->right) && (haffman_tree[q1->left-1].node_weight>haffman_tree[q2->left].node_weight)))
        {
            if (node1!=-1) q1->left--;
            node1 = q2->left++;
        }
        if ((node2==-1) || ((q1->left<q1->right) && (haffman_tree[q2->left-1].node_weight>haffman_tree[q1->left].node_weight)))
        {
            if (node2!=-1) q2->left--;
            node2 = q1->left++;
        }

        haffman_tree[q2->right].node_weight = haffman_tree[node1].node_weight + haffman_tree[node2].node_weight;
         haffman_tree[q2->right].childs[0] = node1;
        haffman_tree[q2->right].childs[1] = node2;
        q2->right++;
    }
    return q2->left;
}

void go_tree(tree_node *haffman_tree, int node_id, char *temp_buffer, char seq_len, bitseq *bit_sequences)
{
    int i;
    if ((haffman_tree[node_id].childs[0]==-1) && (haffman_tree[node_id].childs[1]==-1))
    {

        char c = haffman_tree[node_id].node_char;
        bit_sequences[c].count_digits = seq_len;
        for (i=0;i<seq_len;i++)
            bit_sequences[c].data[i+1] = temp_buffer[i];
        return;
    }
    for (i=0;i<2;i++)
        if (haffman_tree[node_id].childs[i]!=-1)
        {
            temp_buffer[seq_len] = i;
            go_tree(haffman_tree, haffman_tree[node_id].childs[i], temp_buffer, seq_len+1, bit_sequences);
        }
}

void write_map(FILE *out, tree_node *haffman_tree, int lastNode)
{
    int temp =
    fwrite(&temp, sizeof(int), 1, out);
}

void encode_data(FILE *in, FILE *out, bitseq *bit_sequences, long count_chars)
{
    unsigned char temp = 0;
    unsigned char count_bits = 0;
    int read_byte, i;
    while ((read_byte=fgetc(in))!=EOF)
    {
        char temp_char = (char) read_byte;
        for (i=0;i<bit_sequences[temp_char].count_digits;i++)
        {
            temp <<= 1;
            temp |= bit_sequences[temp_char].data[i];
            count_bits++;
            if (count_bits==8)
            {
                count_bits = 0;
                fputc(temp, out);
            }
        }
    };
    if (count_bits!=0)
    {
        temp <<= (8-count_bits);
        fputc(temp, out);
    };
}
void compress(FILE *in, FILE *out)
{
    long old_ptr = ftell(in);
    tree_node haffman_tree[1024];
    long count_chars = count_frequencies(in, haffman_tree);
    int temp = 0, i, j;
    bubble_sort(haffman_tree, 256);
    while (!haffman_tree[temp].node_weight) temp++;
    queue q1, q2;
    q1.left = temp;
    q1.right = q2.left = q2.right = 256;
    int lastNode = genHaffmanTree(&q1, &q2, haffman_tree);
    printf("%d", lastNode);
    bitseq bit_sequences[256];
    char temp_sequence[256];
    go_tree(haffman_tree, lastNode, temp_sequence, 0, bit_sequences);
    for (i=0;i<256;i++)
    {
        printf("\nChar %c", (char) i);
        for (j=0;j<bit_sequences[i].count_digits;j++)
        {
            printf("%d ", bit_sequences[i].data[j]);
        }
    }
    fseek(in, old_ptr, SEEK_SET);
    write_map(out, haffman_tree, lastNode);
    encode_data(in, out, bit_sequences);
}
void decompress(FILE *in, FILE *out)
{
};
int main()
{
    FILE *in = fopen("in.txt","r");
    FILE *out = fopen("out.txt","w");
    char cmd;
    fscanf(in, "%c\n", &cmd);
    switch (cmd)
    {
        case 'c': compress(in, out); break;
        case 'd': decompress(in, out); break;
    }
    fclose(in);
    fclose(out);
    return 0;
}
