/*
 * Register.cpp
 *
 * Created: 23/02/2023 11:34:36 am
 * Author : hp
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define WRITE_REG_	0x01
#define CLK_		0x02
#define RESET_		0x04
#define OUTPUT_		0x08

uint8_t REGISTOR[8] = {0};

int main(void)
{
	// Some pins of PORTC can not be used for I/O directly.
	// turn of JTAG to use them
	// o write a 1 to the JTD bit twice consecutively to turn it off
	MCUCSR = (1<<JTD);  MCUCSR = (1<<JTD);
	//8 bit input from instruction in pinb
	//c[7:4] is input to alu(srcreg1)
	//c[3:0] is input to 
	
	DDRB = 0x00;	// B[3:0] - SrcReg1, B[7:4] - SrcReg2
	DDRD = 0x00;	// D[3:0] - DstReg, D[7:4] - Write Data
	
	DDRC = 0xFF;	// C[3:0] - $2, C[7:4] - $1//$1 to alu, $2 to data mem and mux
	DDRA = 0xFE;	// A[0] - WriteReg, A[1] - CLK, A[2] - RESET, A[7:4] shows the written data
	
	uint8_t src, dst_data, last_control = 0, current_control;
	
	while (1)
	{
		src = PINB;
		dst_data = PIND;//first 4 bits contains data, last 4 bits contain address of dstreg
		current_control = PINA;
		
		PORTC = (REGISTOR[src&0x7]<<4) | (REGISTOR[(src>>4)&0x7]);//output to alu, data mem and mux
		
		if (current_control&RESET_) {
			for (uint8_t i=0; i<8; i++) REGISTOR[i] = 0;
			_delay_ms(100);
			continue;
		}
		
		if ((current_control^last_control) & CLK_) {		//on edge
			if ((last_control&CLK_) && (last_control&WRITE_REG_)) {	// neg edge with WRITE INSTRUCTION
				REGISTOR[dst_data&0x7] = (dst_data>>4);
				PORTA = dst_data & 0xf0;//shows the written data to dstreg
			}
			_delay_ms(100);
		}
		
		
		last_control = current_control;
	}
}

