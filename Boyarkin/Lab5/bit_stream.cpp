#include "bit_stream.h"

bit_stream* create_stream(FILE *f) {
	bit_stream *stream = (bit_stream*)malloc(sizeof(bit_stream));
	stream->f = f;
	stream->count = 0;

	return stream;
}

int get_bit(bit_stream *stream) {

	if (stream->count == 0) {
		stream->data = getc(stream->f);
		stream->count = 8;
	}
	--stream->count;

	int result = stream->data & 1;
	stream->data >>= 1;
	return result;
}

void put_bit(bit_stream *stream, char bit) {
	if (bit == '1') {
		stream->data |= (1 << stream->count);
	}
	else if (bit == '0') {
		stream->data &= ~(1 << stream->count);
	}
	else { return; }

	++stream->count;
	if (stream->count == 8) {
		putc(stream->data, stream->f);
		stream->count = 0;
	}
}

void put_bits(bit_stream *stream, const char *sequence) {
	for (const char *c = sequence; *c != '\0'; ++c) {
		put_bit(stream, *c);
	}
}

void print_remainder(bit_stream *stream) {
	if (stream->count > 0) {
		putc(stream->data, stream->f);
		stream->count = 0;
	}
}

void free_stream(bit_stream *stream) {
	free(stream);
}
