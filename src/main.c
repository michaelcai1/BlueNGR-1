
/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : Micro/Hello_World/Micro_Hello_World_main.c
* Author             : RF Application Team
* Version            : V1.0.0
* Date               : September-2015
* Description        : Code demostrating Hello World with BlueNRG-1
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portable.h"

#include "uart_dma.h"
#include "BlueNRG1.h"
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

static void led1_Task(void* pvParameters)
{
    uint8_t count = 0;
    const portTickType xDelay = pdMS_TO_TICKS(1000); 

    while(1)
    {
        printf("task1...\r\n");
        if(count++ % 2 ==0)
        {
            SdkEvalLedOff(LED1);
        }
        else
        {
            SdkEvalLedOn(LED1);
        }
        vTaskDelay( xDelay ); 
    }
}

static void led2_Task(void* pvParameters)
{
    uint8_t count = 0;
    const portTickType xDelay = pdMS_TO_TICKS(1000); 
    while(1)
    {
        printf("task2...\r\n");
        if(count++ % 2 ==0)
        {
            SdkEvalLedOn(LED2);
        }
        else
        {
            SdkEvalLedOff(LED2);
        }
        vTaskDelay( xDelay );
    }
}

int main(void)
{
    xTaskHandle vTaskhandle1, vTaskhandle2;
    /* System initialization function */
    SystemInit();
  
    /* Identify BlueNRG1 platform */
    SdkEvalIdentification();
    //Clock_Init();
    SdkEvalLedInit(LED1);
    SdkEvalLedInit(LED2);
    SdkEvalLedInit(LED3);
    SdkEvalLedOn(LED3);
    /* UART initialization */
    SdkEvalComUartInit(UART_BAUDRATE);
    //uart_dma_init();


   xTaskCreate( (pdTASK_CODE)led1_Task \
   	            , "LED1 TEST" \
   	            , 150 \
   	            , NULL \
   	            , 1 \
   	            , &vTaskhandle1 );
  xTaskCreate( (pdTASK_CODE)led2_Task \
   	            , "LED2 TEST" \
   	            , 150 \
   	            , NULL \
   	            , 1 \
   	            , &vTaskhandle2 );
  vTaskStartScheduler();

    for(;;);
}

void vApplicationStackOverflowHook( void )
{
	/* This will be called if a task overflows its stack.  pxCurrentTCB
	can be inspected to see which is the offending task. */
	for( ;; );
}

void vApplicationMallocFailedHook(void)
{
	for( ;; );
}

