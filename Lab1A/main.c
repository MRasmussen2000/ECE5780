#include "stm32l476xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#define STACK_SIZE 32

//GPIO A5 LED
//GPIO C13 Switch

void prvSetupHardware(void);
void vLEDStatus( void );
void vButtonStatus( void );
static unsigned int LEDStatus;

void prvSetupHardware(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOC->MODER &= 0xF3FFFFFF;
	GPIOA->MODER &= 0xFFFFF3FF;
	GPIOA->MODER |= 0x400;
}


void vLEDSt( void )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. */

    for( ;; )
    {
        /* Task code goes here. */
			if(LEDStatus == 1){
				GPIOA->ODR |= 1<<5;
			}
			else{
				GPIOA->ODR &= ~(1<<5);
			}
		}
}

void vButSt( void )
{

    for( ;; )
    {
        /* Task code goes here. */
			unsigned int idr = GPIOC->IDR;
			if(idr == 0){
				while(idr == 0){
					idr = GPIOC->IDR;
				}
				if(LEDStatus == 1)
					LEDStatus = 0;
				else
					LEDStatus = 1;
			}
		}	
}
		


int main( void )
{
	/* Perform any hardware setup necessary. */
	prvSetupHardware();
	
	/* --- APPLICATION TASKS CAN BE CREATED HERE --- */
	BaseType_t vLED1;
	BaseType_t vButton1;
	LEDStatus = 1;

	/* Create the task, storing the handle. */
	vLED1 = xTaskCreate(
									vLEDSt,       /* Function that implements the task. */
									"LED is either on or off",          /* Text name for the task. */
									STACK_SIZE,      /* Stack size in words, not bytes. */
									NULL,    /* Parameter passed into the task. */
									tskIDLE_PRIORITY,/* Priority at which the task is created. */
									NULL );      /* Used to pass out the created task's handle. */
	vButton1 = xTaskCreate(
								vButSt,       /* Function that implements the task. */
								"Button is clicked or not",          /* Text name for the task. */
								STACK_SIZE,      /* Stack size in words, not bytes. */
								NULL,    /* Parameter passed into the task. */
								tskIDLE_PRIORITY,/* Priority at which the task is created. */
								NULL );      /* Used to pass out the created task's handle. */

	if( vLED1 == pdPASS && vButton1 == pdPASS)
	{
			/* The task was created.  Use the task's handle to delete the task. */
			//vTaskDelete( xHandle );
			
	}
	vTaskStartScheduler();
	
	/* Start the created tasks running. */
	
	
	/* Execution will only reach here if there was insufficient heap
	to start the scheduler. */
	for( ;; );
	return 0;
}

