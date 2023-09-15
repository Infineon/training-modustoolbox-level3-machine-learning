/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Neural Network Profiler Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "elapsed_timer.h"
#include "mtb_ml_stream.h"
#include "ml_local_regression.h"

#include MTB_ML_INCLUDE_MODEL_FILE(MODEL_NAME)

/*******************************************************************************
* Macros
********************************************************************************/
#define USE_STREAM_DATA             0u
#define USE_LOCAL_DATA              1u

/* Choose the source of regression data. Options:
 * - USE_STREAM_DATA
 * - USE_LOCAL_DATA */ 
#define REGRESSION_DATA_SOURCE      USE_STREAM_DATA

/* Choose which profiling to enable. Options: 
 *  MTB_ML_PROFILE_DISABLE
 *  MTB_ML_PROFILE_ENABLE_MODEL
 *  MTB_ML_PROFILE_ENABLE_LAYER
 *  MTB_ML_PROFILE_ENABLE_MODEL_PER_FRAME
 *  MTB_ML_PROFILE_ENABLE_LAYER_PER_FRAME
 *  MTB_ML_LOG_ENABLE_MODEL_LOG */
#define PROFILE_CONFIGURATION       MTB_ML_PROFILE_ENABLE_MODEL

/*******************************************************************************
* Function Prototypes
********************************************************************************/

/*******************************************************************************
* Global Variables
********************************************************************************/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It does...
*    1. Initializes the BSP.
*    2. Prints welcome message
*    3. Initialize the regression unit - stream or local
*    4. Run the regression
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

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
#if REGRESSION_DATA_SOURCE == USE_LOCAL_DATA
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
#else
    /* Set the baudrate provided by the ML-Middleware (based on host OS) */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, UART_DEFAULT_STREAM_BAUD_RATE);
#endif

    /* Initialize the elapsed timer */
    elapsed_timer_init();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "Neural Network Profiler "
           "****************** \r\n\n");

    mtb_ml_model_bin_t model_bin = {MTB_ML_MODEL_BIN_DATA(MODEL_NAME)};

#if REGRESSION_DATA_SOURCE == USE_STREAM_DATA
    mtb_ml_stream_interface_t interface = {CY_ML_INTERFACE_UART, &cy_retarget_io_uart_obj};

    result = mtb_ml_stream_init(&interface, PROFILE_CONFIGURATION, &model_bin);
#else
    result = ml_local_regression_init(PROFILE_CONFIGURATION, &model_bin);
#endif    

    if(result != CY_RSLT_SUCCESS)
    {
        printf("ERROR: initialization of the ML profiler failed!\r\n");
        CY_HALT();
    }

    for (;;)
    {
#if REGRESSION_DATA_SOURCE == USE_STREAM_DATA
        result = mtb_ml_stream_task();
#else
        result = ml_local_regression_task();
#endif

        if (result == CY_RSLT_SUCCESS)
        {
            printf("\n\rProfiling completed!\n\r");
        }
        else
        {
            printf("\n\rProfiling task failed!\n\r");
        }

#if REGRESSION_DATA_SOURCE == USE_LOCAL_DATA
        /* Only run the local regression once */
        elapsed_timer_pause();
        cyhal_syspm_sleep();
#endif

        printf("Restarting...\r\n");
    }
}

/* [] END OF FILE */
