#include <MKL25Z4.h>
#include <stdio.h>
#include <stdlib.h>

#define LED1_SHIFT (1) //yellow LED portA
#define LED2_SHIFT (2)	//green LED portA
#define LED3_SHIFT (4)	//blue LED portD
#define LED4_SHIFT (12) //red LED portA
#define SW1_SHIFT (5)		//ETS switch portA
#define SW2_SHIFT (13)  //FLS switch portA
#define SW3_SHIFT (16) //overflow switch portA

#define MASK(x) (1UL << (x))
void Basic_Light_Switching_Example(void);



void Basic_Light_Switching_Example(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTA->PCR[LED1_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED1_SHIFT] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED2_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED2_SHIFT] |= PORT_PCR_MUX(1);
	PORTD->PCR[LED3_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED3_SHIFT] |= PORT_PCR_MUX(1);
	PORTA->PCR[LED4_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED4_SHIFT] |= PORT_PCR_MUX(1);
	PORTA->PCR[SW1_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SW1_SHIFT] |= PORT_PCR_MUX(1);
	PORTA->PCR[SW2_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SW2_SHIFT] |= PORT_PCR_MUX(1);
	PORTA->PCR[SW3_SHIFT]  &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SW3_SHIFT] |= PORT_PCR_MUX(1);
	
	PTA->PDDR |= MASK(LED1_SHIFT) | MASK(LED2_SHIFT) | MASK(LED4_SHIFT);
	PTD->PDDR |= MASK(LED3_SHIFT);
	PTA->PDDR &= ~MASK(SW1_SHIFT);
	PTA->PDDR &= ~MASK(SW2_SHIFT);
	PTA->PDDR &= ~MASK(SW3_SHIFT);
	
	//PTA->PSOR = MASK(LED1_SHIFT);
	//PTA->PCOR = MASK(LED2_SHIFT);

}
int main(void){
	char location = 'a';
	Basic_Light_Switching_Example();
	
	while(1){
		switch(location){
			case 'E':
				PTA->PCOR = MASK(LED1_SHIFT); //yellow led off
				PTA->PCOR = MASK(LED2_SHIFT); // green led off
				PTD->PSOR = MASK(LED3_SHIFT); // blue led on
				PTA->PCOR = MASK(LED4_SHIFT); // red LED off
				//if ETS Switch is not pressed, go to Water in tank, else stay at empty
				if(PTA->PDIR &MASK(SW1_SHIFT)){
					location = 'W';
				} else{
				}
				break;
				
			case 'W':
				PTA->PCOR = MASK(LED1_SHIFT); //yellow led off
				PTA->PSOR = MASK(LED2_SHIFT); //green led on
				PTD->PSOR = MASK(LED3_SHIFT); //blue led on
				PTA->PCOR = MASK(LED4_SHIFT); // red LED off
				//if ETS switch is pressed, go back to tank empty
				if(PTA->PDIR &MASK(SW1_SHIFT)){
				} else{
					location = 'E';
				}
				//if FLS switch is pressed, go to tank full
				if(PTA->PDIR &MASK(SW2_SHIFT)){
				} else{
					location = 'F';
				}
				break;
			
			case 'F':
				PTA->PSOR = MASK(LED1_SHIFT);//yellow led on
				PTA->PCOR = MASK(LED2_SHIFT);//green led off
				PTD->PSOR = MASK(LED3_SHIFT);//blue led on
				PTA->PCOR = MASK(LED4_SHIFT); // red LED off
				//if FLS switch is not pressed, go back to water in tank
				if(PTA->PDIR &MASK(SW2_SHIFT)){
					location = 'W';
				} else{
				}
				//if overflow switch is pressed, go to overflow
				if(PTA->PDIR &MASK(SW3_SHIFT)){
				} else{
					location = 'O';
				}
				break;
				
			case 'O':
				PTA->PCOR = MASK(LED1_SHIFT);//yellow led off
				PTA->PCOR = MASK(LED2_SHIFT);//green led off
				PTD->PCOR = MASK(LED3_SHIFT);//blue led off
				PTA->PSOR = MASK(LED4_SHIFT); // red LED on
				//if overflow switch is not pressed, go back to tank full
				if(PTA->PDIR & MASK(SW3_SHIFT)){
					location = 'F';
				} else{
				}
				break;
			
			default:
				PTA->PCOR = MASK(LED1_SHIFT);//yellow led off
				PTA->PCOR = MASK(LED2_SHIFT);//green led off
				PTD->PCOR = MASK(LED3_SHIFT);//blue led off
				PTA->PCOR = MASK(LED4_SHIFT); // red LED off
				//when ETS is pressed, go to tank empty
				if(PTA->PDIR & MASK(SW1_SHIFT)){
				} else{
					location = 'E';
				}	
				break;
		}	
	}
}
