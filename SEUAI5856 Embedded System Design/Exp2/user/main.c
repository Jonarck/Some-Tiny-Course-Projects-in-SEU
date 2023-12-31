#include "lm4f232h5qd.h"
#include <stdbool.h>
#include <stdint.h>
#include <Stdlib.h>
#include <String.h>
#include "driverlib/fpu.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"


#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#define KEY1 ROM_GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_0)


#define LED4 0x10 //0001 0000
#define LED5 0x20 //0010 0000
#define LED6 0x40 //0100 0000
#define LED7 0x80 //1000 0000

#define SYS_CLOCK_KHZ 80000

void key_1_inite(void){
	SYSCTL_RCGCGPIO_R |= 0x40;//PORT G(PG)
	GPIO_PORTG_DIR_R |= 0x00;//DIR: Input
	GPIO_PORTG_AFSEL_R |= 0x00;//Only GPIO
	GPIO_PORTG_DEN_R |= 0xFF;//Use Its Function
	GPIO_PORTG_PUR_R |= 0x01;//WEAK-UP
//	GPIO_PORTH_DATA_R = 0x20;//1 1 1 1 0 0 0 0
}
void LEDs_inite(void){
	SYSCTL_RCGCGPIO_R |= 0x80;//PORT H	
	GPIO_PORTH_DIR_R |= 0xFF;//DIR: Output
	GPIO_PORTH_AFSEL_R |= 0x00;//Only GPIO
	GPIO_PORTH_DEN_R |= 0xFF;//Use Its Function
}
void Delay(int time);
void sysclock_cfg();


int main(void)
{
	sysclock_cfg();
	key_1_inite();
	LEDs_inite();
	
	while(1){

		if(KEY1 == 0){
			GPIO_PORTH_DATA_R = 0x00;
			Delay(10000000);
		}
		else{
				GPIO_PORTH_DATA_R = LED4;
				Delay(10000000);
				GPIO_PORTH_DATA_R = LED6;
				Delay(10000000);
				GPIO_PORTH_DATA_R = LED5;
				Delay(10000000);	
				GPIO_PORTH_DATA_R = LED7;
				Delay(10000000);
		
		}
			
	}

}
void Delay(int time){
	while(time>0){
		time = time -1;
	}
}
void sysclock_cfg(){
	uint32_t ui32Delay, ui32RCC, ui32RCC2;
	ui32RCC = SYSCTL_RCC_R;
	ui32RCC2 = SYSCTL_RCC2_R;
	ui32RCC |= SYSCTL_RCC_BYPASS;
	ui32RCC &= ~(SYSCTL_RCC_USESYSDIV);
	ui32RCC2 |= SYSCTL_RCC2_BYPASS2;
	SYSCTL_RCC_R = ui32RCC;
	SYSCTL_RCC2_R = ui32RCC2;
	SYSCTL_RCC_R &= ~(0x01);
	for(ui32Delay = 524288; ui32Delay > 0; ui32Delay--) {}
	ui32RCC = SYSCTL_RCC_R;
	ui32RCC2 = SYSCTL_RCC2_R;
	ui32RCC &= ~(0x000007c0 | 0x00000030); 
	ui32RCC |= 0x00000440; 
	SYSCTL_RCC_R = ui32RCC; 
	SYSCTL_RCC2_R &= ~(0x00000070); 
	SYSCTL_RCC2_R |= (0x80000000); 
	for(ui32Delay = 32768; ui32Delay > 0; ui32Delay--) {}
	SYSCTL_MISC_R |= 0x00000040; 
	SYSCTL_RCC2_R &= ~(0x00002000); 
	SYSCTL_RCC_R &= ~(0x00002000); 
	ui32RCC = SYSCTL_RCC_R;
	ui32RCC &= ~(0x07800000 | 0x00400000);
	ui32RCC |= 0x07800000;
	ui32RCC2 = SYSCTL_RCC2_R;
	ui32RCC2 &= ~(0x1f800000 | 0x00400000); 
	ui32RCC2 |= (0x41000000); 
	for(ui32Delay = 32768; ui32Delay > 0; ui32Delay--) {
	if(SYSCTL_RIS_R & 0x00000040) {
	break; } }
	ui32RCC &= ~(0x00000800);
	ui32RCC2 &= ~(0x00000800);
	SYSCTL_RCC_R = ui32RCC;
	SYSCTL_RCC2_R = ui32RCC2;
	
	for(ui32Delay = 0; ui32Delay < 10000; )
	{
		ui32Delay++;
	} 
}