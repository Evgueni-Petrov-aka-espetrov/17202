#pragma once
#include <stdio.h>
#include <stdlib.h>

struct bit_stream {
	FILE *f;
	int data;
	int count;
};

bit_stream* create_stream(FILE *f);
int get_bit(bit_stream *stream);
void put_bit(bit_stream *stream, char bit);
void put_bits(bit_stream *stream, const char *sequence);
void print_remainder(bit_stream *stream);
void free_stream(bit_stream *stream);
