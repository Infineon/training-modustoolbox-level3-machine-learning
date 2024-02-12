/******************************************************************************
* File Name:   ml_local_regression.c
*
* Description: This file contains the implementation of local validation of the
*   machine learning model.
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
#include "ml_local_regression.h"
#include "mtb_ml_utils.h"
#include "mtb_ml_dataset.h"

#include "cyhal.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Include regression files */
#include MTB_ML_INCLUDE_MODEL_X_DATA_FILE(MODEL_NAME)
#include MTB_ML_INCLUDE_MODEL_Y_DATA_FILE(MODEL_NAME)

/*******************************************************************************
* Typedefs
*******************************************************************************/

/*******************************************************************************
* Constants
*******************************************************************************/
#define SUCCESS_RATE       98.0f

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* NN Model Object */
static mtb_ml_model_t *model_obj;

/* Output/result buffers for the inference engine */
static MTB_ML_DATA_T *result_buffer;

/* Model Output Size */
static int model_output_size;

/*******************************************************************************
* Local Functions
*******************************************************************************/

/*******************************************************************************
* Function Name: nn_profiler_init
********************************************************************************
* Summary:
*   Initialize the Neural Network based on the given model and setup to start
*   regression of the model and profiling configuration.
*
* Parameters:
*   profile_cfg: profiling configuration
*   model_bin: pointer to the model data
*
* Return:
*   The status of the initialization.
*******************************************************************************/
cy_rslt_t ml_local_regression_init(mtb_ml_profile_config_t profile_cfg,
                                   mtb_ml_model_bin_t *model_bin)
{
    cy_rslt_t result;

    /* Initialize the neural network */
    result = mtb_ml_model_init(model_bin,
                               NULL,
                               &model_obj);
    if (result != CY_RSLT_SUCCESS)
    {
        printf("MTB ML initialization failure: %lu\r\n", (unsigned long) result);
        return result;
    }

    mtb_ml_model_profile_config(model_obj, profile_cfg);

    mtb_ml_model_get_output(model_obj, &result_buffer, &model_output_size);

    /* Print information about the model */
    mtb_ml_utils_print_model_info(model_obj);
    
    return CY_RSLT_SUCCESS;
}

/*******************************************************************************
* Function Name: ml_local_regression_task
********************************************************************************
* Summary:
*   Run the Neural Network Inference Engine based on the regression data.
*
* Return:
*   The status of the task execution.
*******************************************************************************/
cy_rslt_t ml_local_regression_task(void)
{
    /* Regression pointers */
    MTB_ML_DATA_T  *input_reference;
    MTB_ML_DATA_T  *output_reference;
       
    uint32_t     num_loop;
    uint32_t     input_size;
    uint32_t     correct_result = 0;
    bool         test_result;
    uint32_t     total_count = 0;

    /* Parse input data information: 
     * - Data type (TFLM only)
     * - Number of samples
     * - Frame size
     * - Q Factor (IFX only)
     */
    mtb_ml_x_file_header_t *x_file_header = (mtb_ml_x_file_header_t *) MTB_ML_MODEL_X_DATA_BIN(MODEL_NAME);

    /* Point to regression data */
    input_reference  = (MTB_ML_DATA_T *) (((uint32_t) x_file_header) + sizeof(*x_file_header));
    output_reference = (MTB_ML_DATA_T *) MTB_ML_MODEL_Y_DATA_BIN(MODEL_NAME);

    /* Get the number of loops for this regression */
    num_loop = x_file_header->num_of_samples;

    /* Get the number of inputs of the NN */
    input_size = x_file_header->frame_size;

    /* If using Fixed-point and IFX engine, get and set the Q fraction bits */
#if !COMPONENT_ML_FLOAT32
    #ifdef COMPONENT_ML_IFX
        uint32_t q_format = x_file_header->q_factor;
        mtb_ml_model_set_input_q_fraction_bits(model_obj, q_format);
    #endif

#endif /* #if !COMPONENT_ML_FLOAT32 */

    if (input_size != mtb_ml_model_get_input_size(model_obj))
    {
        printf("Input buffer size error, input size=%d, model input size=%d, aborting...\r\n", 
                (int) input_size, (int) mtb_ml_model_get_input_size(model_obj));
        return MTB_ML_RESULT_MISMATCH_DATA_TYPE;
    }

    /* The following loop runs for number of examples used in regression */
    for (int j = 0; j < num_loop; j++)
    {
        mtb_ml_model_run(model_obj, input_reference);

        /* Check if the results are accurate enough */
        if (mtb_ml_utils_find_max(result_buffer, model_output_size) == 
            mtb_ml_utils_find_max(output_reference, model_output_size))
        {
            correct_result++;
        }

        /* Increment buffers */
        input_reference  += input_size;
        output_reference += model_output_size;

        total_count++;
    }

    /* Print PASS or FAIL with Accuracy percentage 
     * Only for regression ... */
    {
        float success_rate;
        char pass[] = "PASS";
        char fail[] = "FAIL";

        /* Check if total count is different than ZERO */
        if (total_count == 0)
        {
            success_rate = 0;
        }
        else
        {
            success_rate = correct_result * 100.0f / total_count;
        }
        

        test_result = (success_rate >= SUCCESS_RATE);

        mtb_ml_model_profile_log(model_obj);
        
        printf("\r\n***************************************************\r\n");
        if (test_result == true)
        {
            printf("%s with accuracy percentage =%3.2f, total_cnt=%d", pass, success_rate, (int) total_count);
        }
        else
        {
            printf("%s with accuracy percentage =%3.2f, total_cnt=%d", fail, success_rate, (int) total_count);
        }
        printf("\r\n***************************************************\r\n");
    }

    return CY_RSLT_SUCCESS;
}

