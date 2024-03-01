/*
---- A DYNAMIC Huffman (Vitter) Coding Implementation ----

Filename:     VITTZD.C	(Decoder)
Written by:   Gerald R. Tamayo, 2008/2023/2024

This is the decompressor for VITTZ.C,
	an *Adaptive* Huffman (Vitter) Algorithm implementation.
*/
#include <stdio.h>
#include <stdlib.h>
#include "gtbitio.c"
#include "hufmodel.c"
#include "adhvitth.c"

typedef struct {
	char algorithm[4];
	unsigned long file_size;
} file_stamp;

void copyright( void );

int main( int argc, char *argv[] )
{
	unsigned long in_file_len = 0, out_file_len = 0;
	huff_model h;
	int hc, i;
	file_stamp fstamp;
	
	if ( argc != 3 ) {
		fprintf(stderr, "\n Usage: vittzd infile outfile");
		copyright();
		return 0;
	}
	if ( (gIN = fopen( argv[1], "rb" )) == NULL ) {
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
	
	fprintf(stderr, "\n---[ A DYNAMIC Huffman (Vitter) Implementation ]---\n");
	fprintf(stderr, "\nName of input file : %s", argv[1] );
	fprintf(stderr, "\nName of output file: %s", argv[2] );
	
	/* get file length. */
	fseek( gIN, 0, SEEK_END );
	in_file_len = ftell( gIN );
	if ( in_file_len == 0 ) goto no_decompression;
	
	/* ===== The Main Huffman Implementation ======= */
	
	fprintf(stderr, "\n\nHuffman decompressing...");
	
	/* This is a DYNAMIC algorithm, so no need to read stats. */
	
	/* start the decoding process. */
	rewind( gIN );
	
	/* read first the file stamp/header. */
	fread( &fstamp, sizeof(file_stamp), 1, gIN );
	
	/* get FIRST symbol. */
	hc = fgetc( gIN );
	out_file_len = fstamp.file_size - 1;
	
	/* output first symbol as a raw byte. */
	pfputc( (unsigned char) hc );
	
	/* ---- initialize the dynamic Huffman tree model ---- */
	init_huff_model( DYNAMIC_HUFFMAN, &h, 256, 8 );
	init_first_nodeVITTER( &h, hc );
	
	/* now get the bit stream. */
	init_get_buffer();
	
	while ( out_file_len-- ) {
		hc = decode_symbolVITTER( &h );
		
		/* output the decoded byte. */
		pfputc( (unsigned char) hc );
	}
	flush_put_buffer();
	fprintf(stderr, "done.");
	
	/* get outfile's size. */
	out_file_len = ftell( pOUT );
	
	fprintf(stderr, "\n\nLength of input file     = %15lu bytes", in_file_len );
	fprintf(stderr, "\nLength of output file    = %15lu bytes\n", out_file_len );
	
	free_get_buffer();
	free_put_buffer();
	free_huff_model( DYNAMIC_HUFFMAN, &h );
	
	no_decompression:
	
	if ( gIN ) fclose( gIN );
	if ( pOUT ) fclose( pOUT );
	return 0;
}

void copyright( void )
{
	fprintf(stderr, "\n\n Written by: Gerald R. Tamayo, 2008-2024\n");
}
