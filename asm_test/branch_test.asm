/*Branch Test 12/26/2019*/
halt_section(0xee):
	halt;

main(0x0000):
	load ax, lo, 0x01;
	load bx, lo, 0x01;
	load ex, lo, 0xee;
	load dx, lo, 0x0a;
	loop:
	add cx, ax, bx;
	mov ax, cx;
	jmp ex, ax eq dx;
	jmp loop;
	halt; //For protection on the emulator
