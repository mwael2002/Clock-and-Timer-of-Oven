/*
 * main.c
 *
 *  Created on: Oct 15, 2021
 *      Author: mwael
 */
#include "STD_Types.h"
#include "DIO_interface.h"
#include "Timer_interface.h"
#include "GIE_interface.h"
#include "Buzzer_interface.h"
#include "Keypad_interface.h"
#include "seven_segment_interface.h"
#include "LED_interface.h"

#define Time_increament              4
#define Time_decreament              12
#define switch_mode                  8

#define Clock_mode                   0
#define Oven_timer_mode              1
#define Restart_oven_timer_mode      2

#define No_Blink                     0
#define Blink_hr                     1
#define Blink_min                    2

#define No_Blink_LED                 0
#define Blink_LED                    1


U8 clock_min;
U8 clock_hr;

U16 clock_desired_count;
U16 timer_desired_count;
U16 blink_desired_count;

S8 timer_min;
S8 timer_hr;


U8 Mode=Clock_mode;

U8 SS_blink_hr_flag;
U8 SS_blink_min_flag;
U8 LED_blink_flag;

Seven_Segment min_unit;
Seven_Segment min_tenth;
Seven_Segment hr_unit;
Seven_Segment hr_tenth;

LED Timer_LED;

void App_init();

void clock_sec_change(void);
void set_current_time(void);
void clock_min_change(void);
void clock_hr_change(void);

void set_oven_timer(void);
void timer_sec_change(void);
void timer_min_change(void);
void timer_hr_change(void);

void restart_oven_timer(void);
void Blink_SS_LED(void);

void main(void){

App_init();

while(1){

restart_oven_timer();

}
}

void App_init(){

	Buzzer_init();

    KPD_init();

    Timer_LED.LED_Port=Group_C;
    Timer_LED.LED_Pin=DIO_Pin_6;
    Timer_LED.LED_Type=LED_Source;

    LED_init(&Timer_LED);




	min_unit.no_Port=Group_B;
	min_unit.Decoder_pins[0]=DIO_Pin_0;
	min_unit.Decoder_pins[1]=DIO_Pin_1;
	min_unit.Decoder_pins[2]=DIO_Pin_2;
	min_unit.Decoder_pins[3]=DIO_Pin_3;

	min_unit.enable_port=Group_C;
	min_unit.enable_pin=DIO_Pin_1;

	SS_Decoder_init(&min_unit);
	SS_Decoder_Set_Number(&min_unit,0);


	min_tenth.no_Port=Group_B;
	min_tenth.Type=Cathode;
	min_tenth.Decoder_pins[0]=DIO_Pin_4;
	min_tenth.Decoder_pins[1]=DIO_Pin_5;
	min_tenth.Decoder_pins[2]=DIO_Pin_6;
	min_tenth.Decoder_pins[3]=DIO_Pin_7;

	min_tenth.enable_port=Group_C;
	min_tenth.enable_pin=DIO_Pin_1;

	SS_Decoder_init(&min_tenth);
	SS_ON(&min_tenth);
	SS_Decoder_Set_Number(&min_tenth,0);



	hr_unit.no_Port=Group_D;
	hr_unit.Decoder_pins[0]=DIO_Pin_0;
	hr_unit.Decoder_pins[1]=DIO_Pin_1;
	hr_unit.Decoder_pins[2]=DIO_Pin_2;
	hr_unit.Decoder_pins[3]=DIO_Pin_3;

	hr_unit.enable_port=Group_C;
	hr_unit.enable_pin=DIO_Pin_0;

	SS_Decoder_init(&hr_unit);
	SS_Decoder_Set_Number(&hr_unit,0);


	hr_tenth.no_Port=Group_D;
	hr_tenth.Type=Cathode;
	hr_tenth.Decoder_pins[0]=DIO_Pin_4;
	hr_tenth.Decoder_pins[1]=DIO_Pin_5;
	hr_tenth.Decoder_pins[2]=DIO_Pin_6;
	hr_tenth.Decoder_pins[3]=DIO_Pin_7;

	hr_tenth.enable_port=Group_C;
	hr_tenth.enable_pin=DIO_Pin_0;

	SS_Decoder_init(&hr_tenth);
	SS_ON(&hr_tenth);
	SS_Decoder_Set_Number(&hr_tenth,0);



    blink_desired_count=Overflow_time_calculate(TIMER0_ID);
    Timer0_init();
    Timer_callback(TIMER0_ID,&Blink_SS_LED);
    Global_Int_En_Dis(Global_Int_Enable);


    set_current_time();

    clock_desired_count=Overflow_time_calculate(TIMER2_ID);
    Timer2_init();
    Timer_callback(TIMER2_ID,&clock_sec_change);


    Mode=Oven_timer_mode;

    set_oven_timer();

    if(timer_min==0){

    	LED_OFF(&Timer_LED);

    	if(clock_hr>=10){
           SS_Decoder_Set_Number(&hr_unit,clock_hr%10);
    	   SS_Decoder_Set_Number(&hr_tenth,clock_hr/10);
    	 }
    	else if(clock_hr<10){
    	   SS_Decoder_Set_Number(&hr_unit,clock_hr);
    	   SS_Decoder_Set_Number(&hr_tenth,0);
    	 }

        if(clock_min>=10){
           SS_Decoder_Set_Number(&min_unit,clock_min%10);
    	   SS_Decoder_Set_Number(&min_tenth,clock_min/10);
    	   }
    	   else if(clock_min<10){
    	   SS_Decoder_Set_Number(&min_unit,clock_min);
    	   SS_Decoder_Set_Number(&min_tenth,0);
    	   }
        Mode=Clock_mode;
    }

    timer_desired_count=Overflow_time_calculate(TIMER1_ID);
    Timer1_init();
    Timer_callback(TIMER1_ID,&timer_sec_change);
}


void set_current_time(void){

    U8 Entered_no=0;

    while(1){

    	SS_blink_hr_flag=Blink_hr;

    	do{

    		Entered_no=KPD_status();

    	}while(Entered_no==No_pressed_key);


    	if(Entered_no==Time_increament){
    		if(clock_hr!=23){
    		clock_hr++;
    		}
    		else if(clock_hr==23){
    		    clock_hr=0;
    		}
    	}
    	else if(Entered_no==Time_decreament){
    		if(clock_hr!=0){
    			clock_hr--;
    		}
    		else if(clock_hr==0){
    		    clock_hr=23;
    		}
    	}

    	SS_blink_hr_flag=No_Blink;
        SS_ON(&hr_tenth);

    	if(clock_hr<10){
    			SS_Decoder_Set_Number(&hr_unit,clock_hr);
    			SS_Decoder_Set_Number(&hr_tenth,0);
    		}
    		else if(clock_hr>=10){
    			SS_Decoder_Set_Number(&hr_unit,clock_hr%10);
    			SS_Decoder_Set_Number(&hr_tenth,clock_hr/10);
    			}

    	if(Entered_no==switch_mode){
    		break;
    	}

    }



    while(1){

    	SS_blink_min_flag=Blink_min;

       	do{

       		Entered_no=KPD_status();

       	}while(Entered_no==No_pressed_key);


       	if(Entered_no==Time_increament){
       		if(clock_min!=59){
       		clock_min++;
       		}
    		else if(clock_min==59){
    		    clock_min=0;
    		}
       	}
       	else if(Entered_no==Time_decreament){
       		if(clock_min!=0){
       			clock_min--;
       		}
    		else if(clock_min==0){
    		    clock_min=59;
    		}
       	}

       	SS_blink_min_flag=No_Blink;
        SS_ON(&min_tenth);

       	if(clock_min<10){
       			SS_Decoder_Set_Number(&min_unit,clock_min);
       			SS_Decoder_Set_Number(&min_tenth,0);
       		}
       		else if(clock_min>=10){
       			SS_Decoder_Set_Number(&min_unit,clock_min%10);
       			SS_Decoder_Set_Number(&min_tenth,clock_min/10);
       			}

       	if(Entered_no==switch_mode){
       		break;
       	}

       }

}

void clock_sec_change(void){
	static U16 count=0;
    static U8 sec=0;
	count++;

	if(count==clock_desired_count){
		sec++;

		if(sec==60){
		sec=0;
		clock_min_change();
		}

		Overflow_restart_count(TIMER2_ID);
		count=0;

	}
	}

void clock_min_change(void){

	clock_min++;

	if(clock_min<10){
		if(Mode==Clock_mode){
		SS_Decoder_Set_Number(&min_unit,clock_min);
		SS_Decoder_Set_Number(&min_tenth,0);
		}
	}

	else if(clock_min>=10){
		if(clock_min<60){
		if(Mode==Clock_mode){
		SS_Decoder_Set_Number(&min_unit,clock_min%10);
		SS_Decoder_Set_Number(&min_tenth,clock_min/10);
		}
	   }
	else if(clock_min==60){
		clock_min=0;
		if(Mode==Clock_mode){
		SS_Decoder_Set_Number(&min_unit,0);
		SS_Decoder_Set_Number(&min_tenth,0);
		}
		clock_hr_change();
	   }
	}
}


void clock_hr_change(void){

	clock_hr++;

	if(clock_hr<10){
		if(Mode==Clock_mode){
		SS_Decoder_Set_Number(&hr_unit,clock_hr);
		SS_Decoder_Set_Number(&hr_tenth,0);
		}
	}
	else if(clock_hr>=10){
		if(clock_hr<24){
		if(Mode==Clock_mode){
		SS_Decoder_Set_Number(&hr_unit,clock_hr%10);
		SS_Decoder_Set_Number(&hr_tenth,clock_hr/10);
		}
	   }
	else if(clock_hr==24){
		clock_hr=0;
		if(Mode==Clock_mode){
		SS_Decoder_Set_Number(&hr_unit,0);
		SS_Decoder_Set_Number(&hr_tenth,0);
		}
	   }
	}

}




void set_oven_timer(){

	LED_blink_flag=Blink_LED;

	SS_Decoder_Set_Number(&hr_unit,0);
	SS_Decoder_Set_Number(&hr_tenth,0);

	SS_Decoder_Set_Number(&min_unit,0);
	SS_Decoder_Set_Number(&min_tenth,0);

	U8 Entered_no=0;

	    while(1){

	    	SS_blink_hr_flag=Blink_hr;

	    	do{

	    		Entered_no=KPD_status();

	    	}while(Entered_no==No_pressed_key);

	    	if(Entered_no==Time_increament){
	    		if(timer_hr!=23){
	    		timer_hr++;
	    		}
	    		else if(timer_hr==23){
	    		    timer_hr=0;
	    		}
	    	}
	    	else if(Entered_no==Time_decreament){
	    		if(timer_hr!=0){
	    			timer_hr--;
	    		}
	    		else if(timer_hr==0){
	    		    timer_hr=23;
	    		}
	    	}

	    	SS_blink_hr_flag=No_Blink;
	        SS_ON(&hr_tenth);


	    	if(timer_hr<10){
	    			SS_Decoder_Set_Number(&hr_unit,timer_hr);
	    			SS_Decoder_Set_Number(&hr_tenth,0);
	    		}
	    		else if(timer_hr>=10){
	    			SS_Decoder_Set_Number(&hr_unit,timer_hr%10);
	    			SS_Decoder_Set_Number(&hr_tenth,timer_hr/10);
	    			}

	    	if(Entered_no==switch_mode){
	    		break;
	    	}

	    }


	    while(1){

	    	SS_blink_min_flag=Blink_min;

	       	do{

	       		Entered_no=KPD_status();

	       	}while(Entered_no==No_pressed_key);

	       	if(Entered_no==Time_increament){
	       		if(timer_min!=59){
	       		timer_min++;
	       		}
	       		else if(timer_min==59){
	       			timer_min=0;
	       		}
	       	}
	       	else if(Entered_no==Time_decreament){
	       		if(timer_min!=0){
	       			timer_min--;
	       		}
	       		else if(timer_min==0){
	       			timer_min=59;
	       		}
	       	}

	    	SS_blink_min_flag=No_Blink;
	        SS_ON(&min_tenth);

	       	if(timer_min<10){
	       			SS_Decoder_Set_Number(&min_unit,timer_min);
	       			SS_Decoder_Set_Number(&min_tenth,0);
	       		}
	       		else if(timer_min>=10){
	       			SS_Decoder_Set_Number(&min_unit,timer_min%10);
	       			SS_Decoder_Set_Number(&min_tenth,timer_min/10);
	       			}

	       	if(Entered_no==switch_mode){
	       		LED_blink_flag=No_Blink;
	       		LED_ON(&Timer_LED);
	       		break;
	       	}

	       }

}

void timer_sec_change(void){
	static U16 count=0;
    static U8 sec=60;
    if(Mode==Oven_timer_mode){
	count++;

	if(count==timer_desired_count){
		sec--;

		if(sec==0){
		sec=60;
		timer_min_change();
		}

		Overflow_restart_count(TIMER1_ID);
		count=0;
	}

  }
}

void timer_min_change(void){
	    timer_min--;
	    if(timer_min>=10){
	    SS_Decoder_Set_Number(&min_unit,timer_min%10);
	    SS_Decoder_Set_Number(&min_tenth,timer_min/10);
	    }

		else if(timer_min<10){
			if(timer_min>0){
			SS_Decoder_Set_Number(&min_unit,timer_min);
			SS_Decoder_Set_Number(&min_tenth,0);
			}

			else if((timer_min==-1)||(timer_min==0)){

			if((timer_hr!=0)&&(timer_min==0)){
			SS_Decoder_Set_Number(&min_unit,0);
			SS_Decoder_Set_Number(&min_tenth,0);
			}

			else if((timer_hr!=0)&&(timer_min==-1)){
				timer_min=59;
				SS_Decoder_Set_Number(&min_unit,timer_min%10);
				SS_Decoder_Set_Number(&min_tenth,timer_min/10);
				timer_hr_change();
			   }

			else if((timer_hr==0)&&(timer_min==0)){
				SS_Decoder_Set_Number(&min_unit,0);
				SS_Decoder_Set_Number(&min_tenth,0);
				timer_hr_change();
			}

			}
	}
}


void timer_hr_change(void){
	   timer_hr--;
	   if(timer_hr>=10){
	   		SS_Decoder_Set_Number(&hr_unit,timer_hr%10);
	   		SS_Decoder_Set_Number(&hr_tenth,timer_hr/10);
	   }

		else if(timer_hr<10){
			if(timer_hr>=0){
			SS_Decoder_Set_Number(&hr_unit,timer_hr);
			SS_Decoder_Set_Number(&hr_tenth,0);
			}
		else if(timer_hr==-1){
            timer_hr=0;

            Buzzer_On();

            while(KPD_status()!=switch_mode);

            Buzzer_Off();

            LED_OFF(&Timer_LED);

            if(clock_hr>=10){
	   		SS_Decoder_Set_Number(&hr_unit,clock_hr%10);
	   		SS_Decoder_Set_Number(&hr_tenth,clock_hr/10);
            }
            else if(clock_hr<10){
    			SS_Decoder_Set_Number(&hr_unit,clock_hr);
    			SS_Decoder_Set_Number(&hr_tenth,0);
            }

            if(clock_min>=10){
	   		SS_Decoder_Set_Number(&min_unit,clock_min%10);
	   		SS_Decoder_Set_Number(&min_tenth,clock_min/10);
            }
            else if(clock_min<10){
    			SS_Decoder_Set_Number(&min_unit,clock_min);
    			SS_Decoder_Set_Number(&min_tenth,0);
            }

            Mode=Clock_mode;

		  }
	  }
}

void restart_oven_timer(void){

while(KPD_status()!=switch_mode);

if(Mode==Clock_mode){

	Mode=Restart_oven_timer_mode;

	SS_Decoder_Set_Number(&hr_tenth,0);
	SS_Decoder_Set_Number(&hr_unit,0);
	SS_Decoder_Set_Number(&min_tenth,0);
	SS_Decoder_Set_Number(&min_unit,0);

	set_oven_timer();

    if(timer_min==0){

    	LED_OFF(&Timer_LED);

    	if(clock_hr>=10){
           SS_Decoder_Set_Number(&hr_unit,clock_hr%10);
    	   SS_Decoder_Set_Number(&hr_tenth,clock_hr/10);
    	 }
    	else if(clock_hr<10){
    	   SS_Decoder_Set_Number(&hr_unit,clock_hr);
    	   SS_Decoder_Set_Number(&hr_tenth,0);
    	 }

        if(clock_min>=10){
           SS_Decoder_Set_Number(&min_unit,clock_min%10);
    	   SS_Decoder_Set_Number(&min_tenth,clock_min/10);
    	   }
    	   else if(clock_min<10){
    	   SS_Decoder_Set_Number(&min_unit,clock_min);
    	   SS_Decoder_Set_Number(&min_tenth,0);
    	   }
        Mode=Clock_mode;

        return;
    }

	Overflow_restart_count(TIMER1_ID);

	Mode=Oven_timer_mode;
 }


else if(Mode==Oven_timer_mode){
    	timer_hr=0;
    	timer_min=0;

	    LED_OFF(&Timer_LED);

    	if(clock_hr>=10){
           SS_Decoder_Set_Number(&hr_unit,clock_hr%10);
    	   SS_Decoder_Set_Number(&hr_tenth,clock_hr/10);
    	 }
    	else if(clock_hr<10){
    	   SS_Decoder_Set_Number(&hr_unit,clock_hr);
    	   SS_Decoder_Set_Number(&hr_tenth,0);
    	 }

        if(clock_min>=10){
           SS_Decoder_Set_Number(&min_unit,clock_min%10);
    	   SS_Decoder_Set_Number(&min_tenth,clock_min/10);
    	   }
    	   else if(clock_min<10){
    	   SS_Decoder_Set_Number(&min_unit,clock_min);
    	   SS_Decoder_Set_Number(&min_tenth,0);
    	   }
        Mode=Clock_mode;
 }

}

void Blink_SS_LED(void){

	static U16 count=0;
	static U8 hr_blink=0;
	static U8 min_blink=0;

if((SS_blink_hr_flag!=No_Blink)||(SS_blink_min_flag!=No_Blink)||(LED_blink_flag!=No_Blink)){

		count++;

if(count==blink_desired_count){

	if(LED_blink_flag==Blink_LED){
       LED_Toggle(&Timer_LED);
    }


	if(SS_blink_hr_flag==Blink_hr){

		if(hr_blink==0){
		   SS_OFF(&hr_tenth);
		   hr_blink=1;
		   }
		   else if(hr_blink==1){
		   SS_ON(&hr_tenth);
		   hr_blink=0;
		   }

      }

	if(SS_blink_min_flag==Blink_min){


		if(min_blink==0){
		   SS_OFF(&min_tenth);
		   min_blink=1;
		   }
		 else if(min_blink==1){
		   SS_ON(&min_tenth);
		   min_blink=0;
		   }
	}


			Overflow_restart_count(TIMER0_ID);
			count=0;

 }
}

}


