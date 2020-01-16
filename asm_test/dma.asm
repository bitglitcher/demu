/*Assembly Test for the D16i architecture DMA*/

//12/31/2019
//1:42AM

/*
This CODE will use the device 0 
which in the D16i emulator(DEMU) is the default Terminal
Ran succefuly 3:38AM
*/


where(0xaa):
	alloc Hello_msg, "Hello World or Hola Mundo xd hahahahaha\n";
	alloc Tu_mama, "\t\tHahaha Tu mama!\n";
	alloc Tu_otra_mama, "\t\tHahahaha Tu otra mama\n";

halt_sec(0x33):
	halt;

print_the_other_one(0x22):
	load ax, lo, Tu_otra_mama;
	load bx, lo, Tu_otra_mama.lenght;
	
	mov mma, ax;
	mov mmc, bx;
	
	dma dev;
	jmp halt_sec;
	
print(0x55):
	load ax, lo, Tu_mama;
	load bx, lo, Tu_mama.lenght;
	
	mov mma, ax;
	mov mmc, bx;
	
	dma dev;
	jmp print_the_other_one;
	

main(0x0000):
	load ax, lo, Hello_msg;
	load bx, lo, Hello_msg.lenght;
	
	mov mma, ax;
	mov mmc, bx;
	
	dma dev;
	
	jmp print;
	
	halt;