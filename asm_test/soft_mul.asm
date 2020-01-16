/*Soft Multiplication Rutine 12/27/2019*/

/*
ax Reserved for the ALU
bx Reserved for the ALU
cx Reserved for the ALU

dx argument 1
ex argumnet 2

fx halt pointer

example 2 * 64
*/

proto halt_section;


main(0x0000):
	load dx, lo, 0x8;
	load ex, lo, 0x8;
	load gx, lo, 0x01; //To increment hx
	load fx, lo, 0xff;
	load hx, lo, 0x01; //Counter
	
	mov ax, dx;
	mov bx, dx;
	
	loop:
	add cx, ax, bx;
	mov ax, cx;
	
	//increment hx
	add hx, gx, hx;
	jmp fx, hx eq ex;
	jmp loop;
	halt;
	

halt_section(0xff):
	halt;