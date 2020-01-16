/*This code will test the Store instruction in LSINS*/

/*
0x00 0000000000 0000000000 
0x02 0000000000 0000000000
0x04 0000000000 0000000000
0x06 0000000000 0000000000

0x08 0000000000 0000000000
0x0a 0000000000 0000000000 <= Where we want to store halt;

*/
main(0x0000):
	load ax, lo, 0x1c;
	load ax, hi, 0x00;
	load cx, lo, 0x0c; //Address pointer
	load dx, lo, 0x0d;
	store ax, hi, cx;
	store ax, lo, dx;
	noop; //Will get overwritten
	
halt_save(0xaa):
	halt; //Just for if the store halt, doesnt work