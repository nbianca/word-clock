#include "matrix.h"

void reset_rows()
{
    ROW_00_REG &= ~(1 << ROW_00_BIT);
    ROW_01_REG &= ~(1 << ROW_01_BIT);
    ROW_02_REG &= ~(1 << ROW_02_BIT);
    ROW_03_REG &= ~(1 << ROW_03_BIT);
    ROW_04_REG &= ~(1 << ROW_04_BIT);
    ROW_05_REG &= ~(1 << ROW_05_BIT);
    ROW_06_REG &= ~(1 << ROW_06_BIT);
    ROW_07_REG &= ~(1 << ROW_07_BIT);
    ROW_08_REG &= ~(1 << ROW_08_BIT);
    ROW_09_REG &= ~(1 << ROW_09_BIT);
    ROW_10_REG &= ~(1 << ROW_10_BIT);
}

void set_row(int idx)
{
    switch (idx) {
        case  0: ROW_00_REG |= (1 << ROW_00_BIT); break;
        case  1: ROW_01_REG |= (1 << ROW_01_BIT); break;
        case  2: ROW_02_REG |= (1 << ROW_02_BIT); break;
        case  3: ROW_03_REG |= (1 << ROW_03_BIT); break;
        case  4: ROW_04_REG |= (1 << ROW_04_BIT); break;
        case  5: ROW_05_REG |= (1 << ROW_05_BIT); break;
        case  6: ROW_06_REG |= (1 << ROW_06_BIT); break;
        case  7: ROW_07_REG |= (1 << ROW_07_BIT); break;
        case  8: ROW_08_REG |= (1 << ROW_08_BIT); break;
        case  9: ROW_09_REG |= (1 << ROW_09_BIT); break;
        case 10: ROW_10_REG |= (1 << ROW_10_BIT); break;
    }
}

void reset_cols()
{
    COL_00_REG |= 1 << COL_00_BIT;
    COL_01_REG |= 1 << COL_01_BIT;
    COL_02_REG |= 1 << COL_02_BIT;
    COL_03_REG |= 1 << COL_03_BIT;
    COL_04_REG |= 1 << COL_04_BIT;
    COL_05_REG |= 1 << COL_05_BIT;
    COL_06_REG |= 1 << COL_06_BIT;
    COL_07_REG |= 1 << COL_07_BIT;
    COL_08_REG |= 1 << COL_08_BIT;
    COL_09_REG |= 1 << COL_09_BIT;
    COL_10_REG |= 1 << COL_10_BIT;
}

void set_col(int idx)
{
    switch (idx) {
        case  0: COL_00_REG &= ~(1 << COL_00_BIT); break;
        case  1: COL_01_REG &= ~(1 << COL_01_BIT); break;
        case  2: COL_02_REG &= ~(1 << COL_02_BIT); break;
        case  3: COL_03_REG &= ~(1 << COL_03_BIT); break;
        case  4: COL_04_REG &= ~(1 << COL_04_BIT); break;
        case  5: COL_05_REG &= ~(1 << COL_05_BIT); break;
        case  6: COL_06_REG &= ~(1 << COL_06_BIT); break;
        case  7: COL_07_REG &= ~(1 << COL_07_BIT); break;
        case  8: COL_08_REG &= ~(1 << COL_08_BIT); break;
        case  9: COL_09_REG &= ~(1 << COL_09_BIT); break;
        case 10: COL_10_REG &= ~(1 << COL_10_BIT); break;
    }
}

void draw_matrix(const uint16_t mask[DISPLAY_HEIGHT])
{
    for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
        reset_rows(), reset_cols();
        set_row(i);
        for (int j = 0; j < DISPLAY_WIDTH; ++j) {
            if (mask[i] & (1 << (15 - j))) {
                set_col(j);
            }
        }
        _delay_ms(1);
    }
}

void test_matrix()
{
    for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
        for (int j = 0; j < DISPLAY_WIDTH; ++j) {
            reset_rows(), reset_cols();
            set_row(i), set_col(j);
            _delay_ms(100);
        }
    }
}

void draw_mask(uint16_t mask[DISPLAY_HEIGHT], int mask_x, int mask_y, const uint16_t *charmap, int charmap_x, int charmap_y)
{
    for (int j = 0; j < charmap_y; ++j) {
        for (int i = 0; i < charmap_x; ++i) {
            if (charmap[j * charmap_x + i]) {
                mask[mask_y + j] |= 1 << (15 - (mask_x + i));
            }
        }
    }
}
