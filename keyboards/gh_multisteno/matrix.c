#include "matrix.h"
#include "gpio.h"
#include "wait.h"
#include <hal.h>

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static void init_rows(void) {
    for (uint8_t rowInd = 0; rowInd < MATRIX_ROWS; rowInd++) {
        palSetLineMode(row_pins[rowInd], PAL_MODE_OUTPUT_PUSHPULL);
        palSetLine(col_pins[rowInd]);
    }
}

static void init_cols(void) {
    for (uint8_t colInd = 0; colInd < MATRIX_COLS; colInd++) {
      palSetLineMode(col_pins[colInd], PAL_MODE_INPUT_PULLUP);
    }  
}

void matrix_init_custom(void) {
    init_rows();
    init_cols();
}

bool read_cols(uint8_t rowInd, matrix_row_t current_matrix[]) {
    bool changed = false;
    
    for (uint8_t colInd = 0; colInd < MATRIX_COLS; colInd++) {
        matrix_row_t existingState = current_matrix[rowInd] & (1U << colInd);
        matrix_row_t newState = (!palReadLine(col_pins[colInd])) << colInd;
        changed |= existingState ^ newState;

        current_matrix[rowInd] = (current_matrix[rowInd] & ~(matrix_row_t)(1U << colInd)) | newState;
    }

    return changed;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    for (uint8_t rowInd = 0; rowInd < MATRIX_ROWS; rowInd++) {
        palClearLine(row_pins[rowInd]);

        wait_us(5);

        changed |= read_cols(rowInd, current_matrix);
        
        palSetLine(row_pins[rowInd]);

        wait_us(5);
    }

    return changed;
}