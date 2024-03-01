This archive contains *basic* implementations of the following algorithm:

Dynamic Huffman coding II - Algorithm Vitter (vitc.c and vitd.c);
(vitz.c, an improved version which transmits at most log2(n) bits for 
a "new" symbol, where n is the size of the current set of "unseen" 
symbols; vitc.c simply emits 8 bits for a new symbol/byte.
Vittz.c and vittzd.c use simple encode_symbol() and 
decode_symbol() functions respectively.)

Notes:

For personal, academic, and research purposes only. Freely distributable.

-- Gerald R. Tamayo, BSIE
   Philippines
