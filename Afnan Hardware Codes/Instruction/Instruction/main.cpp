/*
 * Instruction.cpp
 *
 * Created: 23/02/2023 10:55:52 am
 * Author : hp
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

uint16_t INSTRUCTIONS[1<<8] = {
	0x206f, 0x2013, 0x702e, 0xe127, 0x6123, 0xb724, 0x5213, 0xf221, 0x8321, 0xa1c0, 0x1331, 0xd224, 0x5610, 0x7661, 0x5620, 0x7661, 0xa1a0, 0x4727, 0x9421, 0x3115, 0x2661, 0xd620, 0x0241, 0x2661, 0xd610, 0xa1c0, 0x8271, 0xa110
};//0010000001101111, 00100000 00010011

int main(void)
{
	// Some pins of PORTC can not be used for I/O directly.
	// turn of JTAG to use them
	// o write a 1 to the JTD bit twice consecutively to turn it off
	MCUCSR = (1<<JTD);  MCUCSR = (1<<JTD);
	//input in b, a[0] -[3]opcode, a[4]-[7]=srcreg1, c[0]-[3] srcreg2/dstreg, c[[4]-[7] dstreg/shamt
	
	DDRB = 0x00;	// input pc in pinb
	
	// Instruction : D[7:0]C[7:0]
	DDRA = 0xFF;	// upper 8-bits of instruction a[7:4] is srcreg1, a[3:0] is opcode
	DDRC = 0xFF;	// lower 8-bits of instruction c[7:4] is srcreg2/dstreg, c[3:0] is dstreg/shamt
	
	uint8_t pc = -1;
	uint16_t instruction;
	/* Replace with your application code */
	while (1)
	{
		if (PINB != pc) {
			pc = PINB;
			instruction = INSTRUCTIONS[pc];
			PORTC = (uint8_t) instruction;//lower 8
			PORTA = (uint8_t) (instruction >> 8);
			_delay_ms(50);
		}
		//pc=1;
		//instruction=INSTRUCTIONS[pc];
		//PORTC = (uint8_t) instruction;//lower
		//PORTA = (uint8_t) (instruction >> 8);
	}
}
