/******************************************************************************
* File Name:   control.c
*
* Description: This file contains the implementation to print the results from 
*              the inference engine.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#include <gesture.h>
#include "control.h"
#include "mtb_ml_utils.h"
#include "gesture_names.h"

#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
* Constants
*******************************************************************************/
#define MIN_CONFIDENCE 0.60

/*******************************************************************************
* Function Name: control
********************************************************************************
* Summary:
*   A function used to print the results from the inference engine, such as the
*   the class and the confidence of each class.
*
* Parameters:
*     result_buffer: The result buffer from the inference engine
*     model_output_size: Output size from the inference engine
*
*
*******************************************************************************/
void control(MTB_ML_DATA_T* result_buffer, int model_output_size)
{
    /* Get the class with the highest confidence */
    int class_index = mtb_ml_utils_find_max(result_buffer, model_output_size);

#if !COMPONENT_ML_FLOAT32
    /* Convert 16bit fixed-point output to floating-point for visualization */
    float *nn_float_buffer = (float *) malloc(model_output_size * sizeof(float));
    mtb_ml_utils_model_dequantize(magic_wand_obj, nn_float_buffer);
#else
    float *nn_float_buffer = result_buffer;
#endif

    /* Clear the screen */
    printf("\x1b[2J\x1b[;H");

    /* Prints the confidence level of each class */
    printf("| Gesture         | Confidence\r\n");
    printf("--------------------------------\r\n");
    printf("| %s:", gesture_one);
    printf("%s %%%-3d\r\n", dash_ges_one, (int)(nn_float_buffer[0]*100 + 0.5));
    printf("--------------------------------\r\n");
    printf("| %s:", gesture_two);
    printf("%s %%%-3d\r\n", dash_ges_two, (int)(nn_float_buffer[1]*100 + 0.5));
    printf("--------------------------------\r\n");
    printf("| %s:", gesture_three);
    printf("%s %%%-3d\r\n", dash_ges_three, (int)(nn_float_buffer[2]*100 + 0.5));
    printf("--------------------------------\r\n");
    printf("| %s:", gesture_four);
    printf("%s %%%-3d\r\n", dash_ges_four, (int)(nn_float_buffer[3]*100 + 0.5));
    printf("--------------------------------\r\n");
    printf("| Detection:        ");

    /* Check the confidence for the selected class */
    if(MIN_CONFIDENCE < nn_float_buffer[class_index])
    {
        /* Switch statement for the selected class */
        switch (class_index)
        {
            case 0:
                printf("%s\r\n", gesture_one);
                break;
            case 1:
                printf("%s\r\n", gesture_two);
                break;
            case 2:
                printf("%s\r\n", gesture_three);
                break;
            case 3:
                printf("%s\r\n", gesture_four);
                break;
        }
    }
    /* If the confidence is not high, no gesture detected */
    else
    {
        printf("%s\r\n", gesture_four);
    }
    free(nn_float_buffer);
}
