/******************************************************************************
* File Name:   gesture.h
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
#ifndef GESTURE_H
#define GESTURE_H

#include "cyabs_rtos.h"
#include "mtb_ml_model.h"

/******************************************************************************
 * Constants
 *****************************************************************************/
/* Define if should run inference or print data to the terminal */
/* 0u - run inference engine (default) */
/* 1u - print sensor data to the terminal */
#define GESTURE_DATA_COLLECTION_MODE     0u

/*******************************************************************************
* Global Variables
********************************************************************************/
extern mtb_ml_model_t *magic_wand_obj;

/*******************************************************************************
* Functions
*******************************************************************************/
cy_rslt_t gesture_init(void);
void gesture_task(void *arg);


#endif /* GESTURE_H */
