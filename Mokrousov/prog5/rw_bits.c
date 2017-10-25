#include "rw_bits.h"
#include <stdio.h>
#include <stdlib.h>

int file_read_bit(BitFile *this)
{
    if (this->last_byte_pos==8)
    {
        if (!fread(&this->buf_byte, 1, 1, this->fil))
        {
            return -1;
        }
        this->last_byte_pos=0;
    }
    return (this->buf_byte >> (7-this->last_byte_pos++)) & 1;
}

void file_write_bit(BitFile *this, int bit_val)
{
    if (this->last_byte_pos==8) this->last_byte_pos = 0;
    this->buf_byte <<=1;
    this->buf_byte |= (bit_val & 1);
    if ((++this->last_byte_pos)==8)
    {
        fwrite(&this->buf_byte, 1, 1, this->fil);
        this->last_byte_pos = 0;
        this->buf_byte = 0;
    }
}

void file_write_byte(BitFile *this, int bait)
{
    int i;
    for (i=7;i>=0;i--)
        file_write_bit(this, (bait >> i) & 1);
}

int file_read_byte(BitFile *this)
{
    int i, res;
    res = 0;
    for (i=7;i>=0;i--)
    {
        int temp = file_read_bit(this);
        if (temp==-1) return -1;
        res |= (temp) << i;
    }
    return res;
}
void file_flush_write(BitFile *this)
{
    if (this->last_byte_pos!=0)
    {
        this->buf_byte <<= 8-this->last_byte_pos;
        fwrite(&this->buf_byte, 1, 1, this->fil);
    }
}
void init_file_read_writer(BitFile *this, FILE *fil)
{
    this->fil = fil;
    this->buf_byte = 0;
    this->last_byte_pos = 8;

}
