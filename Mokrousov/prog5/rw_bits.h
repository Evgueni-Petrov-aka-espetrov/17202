#ifndef RW_BITS_H_INCLUDED
#define RW_BITS_H_INCLUDED

#include <stdio.h>
typedef struct {
    FILE *fil;
    unsigned char buf_byte;
    unsigned char last_byte_pos;
} BitFile;


int file_read_bit(BitFile *this);
void file_write_bit(BitFile *this, int bit_val);
void file_write_byte(BitFile *this, int bait);
int file_read_byte(BitFile *this);
void file_flush_write(BitFile *this);
void init_file_read_writer(BitFile *this, FILE *fil);



#endif // RW_BITS_H_INCLUDED
