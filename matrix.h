#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <avr/io.h>
#include <util/delay.h>

/* Matrix dimensions. */
#define DISPLAY_WIDTH 11
#define DISPLAY_HEIGHT 11

/* Rows definitions. */
#define ROW_00_REG PORTA
#define ROW_01_REG PORTA
#define ROW_02_REG PORTA
#define ROW_03_REG PORTA
#define ROW_04_REG PORTA
#define ROW_05_REG PORTA
#define ROW_06_REG PORTA
#define ROW_07_REG PORTA
#define ROW_08_REG PORTB
#define ROW_09_REG PORTB
#define ROW_10_REG PORTB

#define ROW_00_BIT PA7
#define ROW_01_BIT PA6
#define ROW_02_BIT PA5
#define ROW_03_BIT PA4
#define ROW_04_BIT PA3
#define ROW_05_BIT PA2
#define ROW_06_BIT PA1
#define ROW_07_BIT PA0
#define ROW_08_BIT PB7
#define ROW_09_BIT PB6
#define ROW_10_BIT PB5

/* Columns definitions. */
#define COL_00_REG PORTB
#define COL_01_REG PORTB
#define COL_02_REG PORTB
#define COL_03_REG PORTB
#define COL_04_REG PORTB
#define COL_05_REG PORTD
#define COL_06_REG PORTD
#define COL_07_REG PORTD
#define COL_08_REG PORTD
#define COL_09_REG PORTD
#define COL_10_REG PORTD

#define COL_00_BIT PB4
#define COL_01_BIT PB3
#define COL_02_BIT PB2
#define COL_03_BIT PB1
#define COL_04_BIT PB0
#define COL_05_BIT PD7
#define COL_06_BIT PD6
#define COL_07_BIT PD5
#define COL_08_BIT PD4
#define COL_09_BIT PD3
#define COL_10_BIT PD2

/**
 * Disables all rows.
 */
void reset_rows();

/**
 * Enables a row.
 *
 * @param idx Row index.
 */
void set_row(int idx);

/**
 * Disables all columns.
 */
void reset_cols();

/**
 * Sets all columns.
 *
 * @param idx Column index.
 */
void set_col(int idx);

/**
 * Draws the matrix from a mask.
 *
 * @param mask Matrix's mask
 */
void draw_matrix(const uint16_t mask[DISPLAY_HEIGHT]);

/**
 * Lights up all LEDs.
 */
void test_matrix();

/**
 * Draws a charmap in another mask.
 *
 * @param mask      Matrix mask.
 * @param mask_x    Column index in mask.
 * @param mask_y    Row index in mask.
 * @param charmap   Charmap mask.
 * @param charmap_x Charmap's width.
 * @param charmap_y Charmap's height.
 */
void draw_mask(uint16_t mask[DISPLAY_HEIGHT], int mask_x, int mask_y, const uint16_t *charmap, int charmap_x, int charmap_y);

#endif // _MATRIX_H_
