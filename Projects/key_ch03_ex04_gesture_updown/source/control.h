/******************************************************************************
* File Name:   control.h
*
* Description: This file contains the function prototypes and constants used
*   in control.c.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef CONTROL_H
#define CONTROL_H

#include "mtb_ml_common.h"

/*******************************************************************************
* Functions
*******************************************************************************/
void control(MTB_ML_DATA_T* result_buffer, int model_output_size);

#endif /* CONTROL_H */
