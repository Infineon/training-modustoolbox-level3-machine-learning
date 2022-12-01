/******************************************************************************
* File Name:   processing.h
*
* Description: This file contains the function prototypes and constants used
*   in processing.c.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef PROCESSING_H
#define PROCESSING_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
#define MAX_STATES 10

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct iir_filter_struct_t
{
    const float* b;
    const float* a;
    float x_states[MAX_STATES];
    float y_states[MAX_STATES];
    int n_order;
}iir_filter_struct;

/* Coefficients for 3rd order butter-worth filter */
#define IIR_FILTER_BUTTER_WORTH_COEFF_B { 0.01809893f, 0.0542968f , 0.0542968f , 0.01809893f }
#define IIR_FILTER_BUTTER_WORTH_COEFF_A { 1.0f        , -1.76004188f,  1.18289326f, -0.27805992f }

/*******************************************************************************
* Functions
*******************************************************************************/
int iir_filter_init(iir_filter_struct* st, const float* b, const float* a, int n_order);
void iir_filter(iir_filter_struct *st, float *buf, int length, uint16_t cur_dimension, uint16_t total_dimensions);
void normalization_min_max(float *buf, uint16_t length, uint16_t dimension,float sensor_min, float sensor_max);
void cast_int16_to_float(int16_t *int_buf, float *float_buf, uint16_t length);
void column_swap(float *buf, uint16_t length, uint16_t dimension, uint16_t column_one, uint16_t column_two);
void column_inverse(float *buf, uint16_t length, uint16_t dimension, uint16_t column);

#endif /* PROCESSING_H */
