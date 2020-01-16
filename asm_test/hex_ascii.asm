/*Bootloader Lv 0*/

data(0x90):
	alloc hex_digits, "0123456789ABCDEF";

	

//ax points to the values
//It only prints the first 4 bits

hex_ascii(0x0000):
	load ax, 0xfff1;
	//Copy ax to dx, to have a copy for later
	mov dx, ax;
	//With Logical Add it will eliminate 0xf + values, so 0xaef9 will left only 0x0009
	load bx, lo, 0x0f;
	and cx, ax, bx;

	//load the hex_digits addr
	load fx, lo, hex_digits;
	
	
	//So cx holds the index and fx the address
	//load cx, lo, 0xa;
	add gx, fx, cx;
	mov fx, gx;
	
	//Now fx points to the corresponding letter
	load gx, lo, 0x00; //To print one letter
	
	//Print to terminal device
	
	mov mma, fx;
	mov mmc, gx;

	dma dev;
	halt;
	//jmp halt_sec;
	//halt;