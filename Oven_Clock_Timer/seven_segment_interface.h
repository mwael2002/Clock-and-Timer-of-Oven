/*
 * seven_segment_interface.h
 *
 *  Created on: Jul 5, 2022
 *      Author: mwael
 */

#ifndef SEVEN_SEGMENT_INTERFACE_H_
#define SEVEN_SEGMENT_INTERFACE_H_

#define Anode 1
#define Cathode 0

typedef struct{
	U8 Type;
	U8 no_Port;
	U8 enable_port;
	U8 enable_pin;
	U8 Decoder_pins[4];
}Seven_Segment;

void SS_init(Seven_Segment* SS);
void SS_ON(Seven_Segment* SS);
void SS_OFF(Seven_Segment* SS);
void SS_Set_Number(Seven_Segment* SS ,U8 no);

void SS_Decoder_init(Seven_Segment* SS);
void SS_Decoder_Set_Number(Seven_Segment* SS,U8 no);

#endif /* SEVEN_SEGMENT_INTERFACE_H_ */
