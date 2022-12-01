/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 6 MCU: Machine Learning
*              Gesture Detection Example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/

#include <gesture.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"


/*******************************************************************************
* Constants
*******************************************************************************/
#define TASK_STACK_SIZE 4096
#define TASK_PRIORITY   2

/*******************************************************************************
* Global Variables
********************************************************************************/
/* This enables RTOS aware debugging */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It does...
*    1. Initializes the BSP.
*    2. Prints welcome message
*    3. Initializes the task to run the inference engine
*    4. Starts the scheduler
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "Gesture Detection Code Example"
           "****************** \r\n\n");

    /* Create one task, processes all sensor data and feeds it to the inference engine */
    xTaskCreate(gesture_task, "Gesture task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    for (;;)
    {
    }
}

/*******************************************************************************
* Function Name: vApplicationDaemonTaskStartupHook
********************************************************************************
* Summary:
*   Executes after the scheduler starts.
*
*******************************************************************************/
void vApplicationDaemonTaskStartupHook(void)
{
    /* Initializes the inference engine */
    cy_rslt_t result;

    result = gesture_init();
    if(CY_RSLT_SUCCESS != result)
    {
        /* Reset the system on sensor fail */
        NVIC_SystemReset();
    }
}


/* [] END OF FILE */
