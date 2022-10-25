/*
 * seven_segment_program.c
 *
 *  Created on: Jul 5, 2022
 *      Author: mwael
 */
#include "STD_Types.h"
#include "bit_calc.h"
#include "DIO_interface.h"
#include "seven_segment_interface.h"

	U8 SS_arr[10]={0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10010000};

void SS_init(Seven_Segment* SS){
	DIO_set_pin_direction(SS->enable_port,SS->enable_pin,OUTPUT);
	DIO_set_port_direction(SS->no_Port,Port_8_pins_OUTPUT);
}
void SS_ON(Seven_Segment* SS){
		if(SS->Type==Anode){
		DIO_set_pin_value(SS->enable_port,SS->enable_pin,HIGH);
		}
	else if(SS->Type==Cathode){
			DIO_set_pin_value(SS->enable_port,SS->enable_pin,LOW);
	}
}
void SS_OFF(Seven_Segment* SS){
		if(SS->Type==Anode){
		DIO_set_pin_direction(SS->enable_port,SS->enable_pin,LOW);
		}
	else if(SS->Type==Cathode){
			DIO_set_pin_value(SS->enable_port,SS->enable_pin,HIGH);
	}
}
void SS_Set_Number(Seven_Segment* SS,U8 no){
	if(SS->Type==Anode){
	DIO_set_port_value(SS->no_Port,SS_arr[no]);
    }
else if(SS->Type==Cathode){
	DIO_set_port_value(SS->no_Port,(~SS_arr[no]));
    }
}



void SS_Decoder_init(Seven_Segment* SS){
	DIO_set_pin_direction(SS->enable_port,SS->enable_pin,OUTPUT);
for(U8 decoder_pin_count=0;decoder_pin_count<4;decoder_pin_count++){
	DIO_set_pin_direction(SS->no_Port,SS->Decoder_pins[decoder_pin_count],OUTPUT);
}
}

void SS_Decoder_Set_Number(Seven_Segment* SS,U8 no){

	for(U8 decoder_pin_count=0;decoder_pin_count<4;decoder_pin_count++){
		DIO_set_pin_value(SS->no_Port,SS->Decoder_pins[decoder_pin_count],get_bit(no,decoder_pin_count));
	}

}
