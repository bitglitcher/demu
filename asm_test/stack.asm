/*Push data into stack*/

//Test.... I think

/*
Basically push some value to the stack
And then pop this value to some register
*/
		
main(0x0000):


	load ax, 0xffff;
	
	load bx, 0xaaaa;
	
	load cx, 0xf0f0;

	push ax, lo;
	push ax, hi;
	
	push bx, lo;
	push bx, hi;
	
	push cx, lo;
	push cx, hi;
	
	halt;
