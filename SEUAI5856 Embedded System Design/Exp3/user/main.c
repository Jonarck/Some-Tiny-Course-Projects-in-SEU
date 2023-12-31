#include "lm4f232h5qd.h"
#include <stdbool.h>
#include <stdint.h>
#include <Stdlib.h>
#include <String.h>
#include "driverlib/fpu.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "hw_watchdog.h"

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#define SYS_CLOCK_KHZ 80000

#define KEY1 ROM_GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_0)

#define LED7 0x10 //0001 0000
#define LED4 0x20 //0010 0000
#define LED5 0x40 //0100 0000
#define LED6 0x80 //1000 0000

#define SYS_CLOCK_KHZ 80000

void Delay(int time){
	while(time>0){
		time = time - 1;
	}
}

//Inite clock
void sysclock_cfg();

//Inite Basic Hardware
void key_1_inite(void){//Used to 
	SYSCTL_RCGCGPIO_R |= 0xC0;//PORT G(PG)
	GPIO_PORTG_DIR_R |= 0x00;//DIR: Input
	GPIO_PORTG_AFSEL_R |= 0x00;//Only GPIO
	GPIO_PORTG_DEN_R |= 0xFF;//Use Its Function
	GPIO_PORTG_PUR_R = 0x01;//WEAK-UP
}

void LEDs_inite(void){//Used to show the phenomenon
	SYSCTL_RCGCGPIO_R |= 0x80;//PORT H	
	GPIO_PORTH_DIR_R |= 0xFF;//DIR: Output
	GPIO_PORTH_AFSEL_R |= 0x00;//Only GPIO
	GPIO_PORTH_DEN_R |= 0xFF;//Use Its Function
}



//Define interrupt function & Design the interrupt phenomenon
void IntDefaultHandler_WDT(){
	while(1){

		GPIO_PORTH_DATA_R = LED6;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED5;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED6;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED5;
		Delay(10000000);

	}
}

//Inite WatchDog
void WDT0_inite(void){
	SYSCTL_RCGCWD_R |= 0x01;//Chip Select to Enable the WDT0

	WATCHDOG0_LOCK_R = 0x1ACCE551;//WDT locked to avoid effect from other hardware

	WATCHDOG0_LOAD_R = 0x1FFFFFFF;//Load the required timer load value for the WDTLOAD register
	
	WATCHDOG0_CTL_R = 0x00000003;//Set the INTTYPE position bit and INTEN bit of the WDTCTL register to set interrupt type and enable it. 
	
	NVIC_EN0_R = 0x00040000;//Interrupt enable (NVIC)
}

//Feet dog
void Feet_dog(){
	WATCHDOG0_ICR_R = 0123;//eliminate the interrupt
	WATCHDOG0_LOAD_R = 0x1FFFFFFF;//Load the required timer load value for the WDTLOAD register
}




int main(void)
{
	sysclock_cfg();
	key_1_inite();
	LEDs_inite();
	

	
	WDT0_inite();//Fistly,Enable the WDT

	while(1){
		


		GPIO_PORTH_DATA_R = LED4;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED5;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED6;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED7;
		Delay(10000000);
		GPIO_PORTH_DATA_R = 0;
	
	
		if(KEY1 != 1){//Press key1 to feet dog
			//feet dog
			WATCHDOG0_ICR_R = 0x0123;//eliminate the interrupt
			WATCHDOG0_LOAD_R = 0x0FFFFFFF;//Load the required timer load value for the WDTLOAD register
		GPIO_PORTH_DATA_R = LED4;
		Delay(10000000);
		GPIO_PORTH_DATA_R = LED7;
		Delay(10000000);
		}
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