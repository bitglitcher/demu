/*Bootloader Lv 0*/

data(0x90):
	alloc Hello_msg, "Architecture D16i\n";
	alloc Device_id, "\tDefault Output Device 0x\n";
	alloc Default_Memory_address, "\tDefault Memory Address 0x\n";
	alloc hex_digits, "0123456789ABCDEF";
halt_sec(0x33):
	halt;

//Takes ax as argument
hex_ascii(0x3e):
	//Push LPR to stack for nested calls
	push lpr, hi;
	push lpr, lo;
	
	
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
	
	return;

main(0x0000):
	load ax, lo, Hello_msg;
	load bx, lo, Hello_msg.lenght;
	
	mov mma, ax;
	mov mmc, bx;
	dma dev;
	
	load ax, 0x0000;
	
	
	device_print:
	load ax, lo, Device_id;
	load bx, lo, Device_id.lenght;
	
	mov mma, ax;
	mov mmc, bx;
	dma dev;	
	
	memory_print:
	load ax, lo, Default_Memory_address;
	load bx, lo, Default_Memory_address.lenght;
	
	load cx, lo, 0x1;
	sub bx, bx, cx;
	
	mov mma, ax;
	mov mmc, bx;
	dma dev;
	
	load ax, lo, 0xa;
	

	hex_print:
	
	load ax, lo, 0xa;
	call hex_ascii, fx;
	
	halt;
	//jmp halt_sec;
	//halt;