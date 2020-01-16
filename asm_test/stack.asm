/*Push data into stack*/

//Test.... I think

/*
Basically push some value to the stack
And then pop this value to some register
*/
main(0x0000):
	load ax, lo, 0xff;
	mov bx, sp;
	store ax, lo, bx;
	load ax, lo, bx;
	halt;