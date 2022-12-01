// This file is generated. Do not edit.
// Generated on: 08.09.2022 02:33:42

#ifndef TEST_MODEL_GEN_H
#define TEST_MODEL_GEN_H

#include "tensorflow/lite/c/common.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define TEST_MODEL_MODEL_CONST_DATA_SIZE 14094
#define TEST_MODEL_MODEL_INIT_DATA_SIZE 372
#define TEST_MODEL_MODEL_UNINIT_DATA_SIZE 1608


// Sets up the model with init and prepare steps.
TfLiteStatus TEST_MODEL_init();
// Returns the input tensor with the given index.
TfLiteTensor *TEST_MODEL_input(int index);
// Returns the output tensor with the given index.
TfLiteTensor *TEST_MODEL_output(int index);
// Runs inference for the model.
TfLiteStatus TEST_MODEL_invoke();

// Returns the number of input tensors.
size_t TEST_MODEL_inputs();

// Returns the number of output tensors.
size_t TEST_MODEL_outputs();

// Return the buffer pointer of input tensor
void *TEST_MODEL_input_ptr(int index);

// Return the buffer size of input tensor
size_t TEST_MODEL_input_size(int index);

// Return the dimention size of input tensor
int TEST_MODEL_input_dims_len(int index);

// Return the dimention buffer pointer of input tensor
int *TEST_MODEL_input_dims(int index);

// Return the buffer pointer of output tensor
void *TEST_MODEL_output_ptr(int index);

// Return the buffer size of output tensor
size_t TEST_MODEL_output_size(int index);

// Return the dimention size of output tensor
int TEST_MODEL_output_dims_len(int index);

// Return the dimention buffer pointer of output tensor
int *TEST_MODEL_output_dims(int index);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif
