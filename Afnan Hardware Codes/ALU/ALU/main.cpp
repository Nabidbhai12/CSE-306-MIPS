/*
 * ALU.cpp
 *
 * Created: 23/02/2023 11:22:40 am
 * Author : hp
 */ 

#include <avr/io.h>
#define F_CPU 1000000 // Clock Frequency
#include <util/delay.h>


int main(void)
{
	// Some pins of PORTC can not be used for I/O directly.
	// turn of JTAG to use them
	// o write a 1 to the JTD bit twice consecutively to turn it off
	MCUCSR = (1<<JTD);  MCUCSR = (1<<JTD);

	DDRA = 0xF0;				// A[3:0] srcreg2/dstreg from mux
	DDRB = 0xF0;				// input from register srcreg
	DDRD = 0b00000111;				// D[7] - Zero Flag | D[6:3] - Output
	DDRC = 0xF8;				//aluop c[2:0] from control

	uint8_t control_bits;
	uint8_t in1, in2, out;

	while (1)
	{
		control_bits = PINC & 0x07;

		in1 = PINB & 0x0f;//srcreg1
		in2 = (PINB & 0x0f);//srcreg2/dstreg
		out = 0;
		//in1=0b0001;
		//in2=0b0001;
		

		if(control_bits == 0) {
			out = in1 + in2;
			} else if(control_bits == 1){
			out = in1 - in2;
			} else if(control_bits == 2) {
			out = in1 & in2;
			} else if(control_bits == 3) {
			out = in1 | in2;
			} else if(control_bits == 4) {
			out = ~(in1 | in2);
			} else if(control_bits == 5) {
			out = in1 << in2;
			} else if(control_bits == 6) {
			out = in1 >> in2;
		}

		out = out & 0x0f;
		if(!out) {
			out = out | 0x80;
		}
		//change this
		PORTD = out<<3;
	}
}

