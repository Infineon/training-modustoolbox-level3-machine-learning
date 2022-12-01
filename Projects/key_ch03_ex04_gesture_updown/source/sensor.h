/******************************************************************************
* File Name:   sensor.h
*
* Description: This file contains the function prototypes and constants used
*   in sensor.c.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef SENSOR_H
#define SENSOR_H

#include "cy_result.h"

/******************************************************************************
 * Constants
 *****************************************************************************/
#define SENSOR_DATA_WIDTH 2
#define SENSOR_NUM_AXIS   6
#define SENSOR_SAMPLE_SIZE (SENSOR_DATA_WIDTH * SENSOR_NUM_AXIS)
#define SENSOR_BATCH_SIZE  128u

/*******************************************************************************
* Functions
*******************************************************************************/
cy_rslt_t sensor_init(void);
cy_rslt_t sensor_get_data(void* sensor_data);

#endif /* SENSOR_H */