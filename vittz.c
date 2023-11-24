/*
	---- A DYNAMIC Huffman Coding Implementation (Algorithm V) ----

	Filename:   VITTZ.C	(Encoder)
	Decoder:    VITTZD.C
	Written by: Gerald Tamayo
	Date:       2008/2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtbitio.c"
#include "hufmodel.c"
#include "adhvitth.c"

typedef struct {
	char algorithm[4];
	unsigned long file_size;
} file_stamp;

/* create an input buffer for faster I/O */
#define IN_BUFSIZE 16384
unsigned char input_buf[ IN_BUFSIZE ];
unsigned int nread = 0, in_i = 0;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long in_file_len, out_file_len;
	FILE *in;
	huff_model huff;
	int c = EOF, i;
	file_stamp fstamp;

	if ( argc != 3 ) {
		fprintf(stderr, "\n-[ A Base-256 Adaptive Huffman (Vitter) Implementation ]-\n");
		fprintf(stderr, "\n Usage: vittz infile outfile");
		copyright();
		return 0;
	}

	if ( (in = fopen( argv[1], "rb" )) == NULL ) {
		fprintf(stderr, "\nError opening input file.");
		copyright();
		return 0;
	}
	if ( (pOUT = fopen( argv[2], "wb" )) == NULL ) {
		fprintf(stderr, "\nError opening output file.");
		copyright();
		return 0;
	}
	init_put_buffer();
	
	fprintf(stderr, "\n--[ A DYNAMIC Huffman (Vitter) Implementation ]--\n");
	
	fprintf(stderr, "\nName of input file : %s", argv[1] );
	fprintf(stderr, "\nName of output file: %s", argv[2] );
	
	/* get length of input file. */
	fseek( in, 0, SEEK_END );
	in_file_len = ftell( in );
	if ( in_file_len == 0 ) goto no_compression;
	
	/* ===== The Main Huffman Implementation ======= */
	
	fprintf(stderr, "\n\nHuffman compressing...");
	
	/*	This is a DYNAMIC algorithm, so no need to read and store stats. */
	
	/* encode FILE STAMP. */
	rewind( pOUT );
	strcpy( fstamp.algorithm, "VTR" );
	fstamp.file_size = in_file_len;
	fwrite( &fstamp, sizeof( file_stamp ), 1, pOUT );
	
	/* ---- start huffman encoding the symbols/bytes. ---- */
	rewind( in );
	
	/* get first byte. */
	c = fgetc( in );
	
	/* output first byte. */
	fputc( (unsigned char) c, pOUT );
	
	/* ---- initialize the dynamic Huffman tree model ---- */
	init_huff_model( DYNAMIC_HUFFMAN, &huff, 256, 8 );
	init_first_nodeVITTER( &huff, c );
	
	while( 1 ) {
		/* load the input buffer. */
		nread = fread( input_buf, 1, IN_BUFSIZE, in );
		if ( nread == 0 ) break;
		in_i = 0;

		/* get bytes from the buffer and compress them. */
		while( in_i < nread ){
			c = (unsigned char) *(input_buf+in_i);
			++in_i;
			
			/* encode the byte c. */
			encode_symbolVITTER( &huff, c );
		}
	}
	flush_put_buffer();
	fprintf( stderr, "done.");
	
	/* get outfile's size and get compression ratio. */
	out_file_len = ftell( pOUT );
	
	fprintf(stderr, "\n\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes", out_file_len );
	fprintf(stderr, "\nCompression ratio:         %15.2f %%\n",
		( ((float) in_file_len - (float) out_file_len) / (float) in_file_len	)
		* (float) 100 );
	
	free_huff_model( DYNAMIC_HUFFMAN, &huff );
	
	no_compression:
	
	free_put_buffer();
	if ( in ) fclose( in );
	if ( pOUT ) fclose( pOUT );
	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald Tamayo, 2005/2023\n");
}
