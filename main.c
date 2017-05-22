#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>
#include <i2chw/i2cmaster.h>

#include "buzzer.h"
#include "matrix.h"
#include "rtc.h"
#include "usart.h"

/**
 * Useful phrases to be shown.
 */
#define hour_ONE                    mask[9]  |= 0xE000
#define hour_TWO                    mask[7]  |= 0xE0
#define hour_THREE                  mask[9]  |= 0x3E0
#define hour_FOUR                   mask[10] |= 0xF000
#define hour_FIVE                   mask[7]  |= 0xF000
#define hour_SIX                    mask[9]  |= 0x1C00
#define hour_SEVEN                  mask[4]  |= 0x7C00
#define hour_EIGHT                  mask[8]  |= 0x3E0
#define hour_NINE                   mask[7]  |= 0xF00
#define hour_TEN                    mask[6]  |= 0xE0
#define hour_ELEVEN                 mask[8]  |= 0xFC00

#define phrase_A0                   mask[0]  |= 0x80
#define phrase_A                    mask[2]  |= 0x200
#define phrase_DAY                  mask[5]  |= 0x1C0
#define phrase_FIVE                 mask[2]  |= 0x1E0
#define phrase_HALF                 mask[0]  |= 0x1E0
#define phrase_HAVE                 mask[0]  |= 0x1E00
#define phrase_ITS                  mask[0]  |= 0xE000
#define phrase_MIDNIGHT             mask[6]  |= 0xFF00
#define phrase_NICE                 mask[5]  |= 0x3C0
#define phrase_NOON                 mask[4]  |= 0x1E0
#define phrase_OCLOCK               mask[10] |= 0x7E0
#define phrase_PAST                 mask[3]  |= 0x3C0
#define phrase_QUARTER              mask[1]  |= 0xFE0
#define phrase_TEN                  mask[1]  |= 0x7000
#define phrase_TIL                  mask[3]  |= 0x1C00
#define phrase_TWENTY               mask[2]  |= 0xFC00
#define phrase_WAY                  mask[3]  |= 0xE000

/**
 * Charmap of all digits using a grid of 4x5.
 */
static const uint16_t DIGITS_4x5[][20] = {
    {
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
    },
    {
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
    },
    {
        1, 1, 1, 1,
        0, 0, 0, 1,
        1, 1, 1, 1,
        1, 0, 0, 0,
        1, 1, 1, 1,
    },
    {
        1, 1, 1, 1,
        0, 0, 0, 1,
        1, 1, 1, 1,
        0, 0, 0, 1,
        1, 1, 1, 1,
    },
    {
        1, 0, 0, 0,
        1, 0, 0, 0,
        1, 1, 1, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
    },
    {
        1, 1, 1, 1,
        1, 0, 0, 0,
        1, 1, 1, 1,
        0, 0, 0, 1,
        1, 1, 1, 1,
    },
    {
        1, 1, 1, 1,
        1, 0, 0, 0,
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
    },
    {
        1, 1, 1, 1,
        0, 0, 0, 1,
        0, 0, 1, 0,
        0, 0, 1, 0,
        0, 0, 1, 0,
    },
    {
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
    },
    {
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 1, 1, 1,
        0, 0, 0, 1,
        1, 1, 1, 1,
    }

};

/**
 * Initializes the hardware.
 */
void init()
{
    /* Initialize USART for debugginig. */
    USART0_init(12);
    USART0_print("USART0 initialized.\n");

    /* Initializes I2C. */
    i2c_init();
    USART0_print("I2C initialized.\n");

    /* Initializes DS3231. */
    ds3231_init();
    USART0_print("RTC DS3231 initialized.\n");

    /* Initialize buzzer. */
    DDRC |= (1 << PC7);
    PORTC &= ~(1 << PC7);
    USART0_print("Buzzer initialized.\n");

    /* Initialize matrix. */
    DDRA |= (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4) | (1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0);
    DDRB |= (1 << PB7) | (1 << PB6) | (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2);
    USART0_print("Matrix initialized.\n");
}

/**
 * Gets the mask for a specific time.
 *
 * @param t    Time.
 * @param mask Returned mask.
 */
void get_mask(struct tm *t, uint16_t mask[DISPLAY_HEIGHT])
{
    int hour = t->hour, min = t->min;

    /* It is no longer X past Y, it's (60 - X) til (Y + 1). */
    if (min > 30)
        ++hour;

    /* Always show IT'S. */
    phrase_ITS;

    /* Show MINUTE. */
    if ((min > 57) && (min < 3))
        { /* no phrase for minutes */ }
    else if ((min > 2) && (min < 8))
        phrase_FIVE, phrase_PAST;
    else if ((min > 7) && (min < 13))
        phrase_TEN, phrase_PAST;
    else if ((min > 12) && (min < 18))
        phrase_A0, phrase_QUARTER, phrase_PAST;
    else if ((min > 17) && (min < 23))
        phrase_TWENTY, phrase_PAST;
    else if ((min > 22) && (min < 28))
        phrase_TWENTY, phrase_FIVE, phrase_PAST;
    else if ((min > 27) && (min < 33))
        phrase_HALF, phrase_PAST;
    else if ((min > 32) && (min < 38))
        phrase_TWENTY, phrase_FIVE, phrase_TIL;
    else if ((min > 37) && (min < 43))
        phrase_TWENTY, phrase_TIL;
    else if ((min > 42) && (min < 48))
        phrase_A0, phrase_QUARTER, phrase_TIL;
    else if ((min > 47) && (min < 53))
        phrase_TEN, phrase_TIL;
    else if ((min > 52) && (min < 58))
        phrase_FIVE, phrase_TIL;

    /* Show HOUR. */
    switch (hour) {
        case 0:     phrase_MIDNIGHT; break;
        case 1:
        case 13:    hour_ONE;        break;
        case 2:
        case 14:    hour_TWO;        break;
        case 3:
        case 15:    hour_THREE;      break;
        case 4:
        case 16:    hour_FOUR;       break;
        case 5:
        case 17:    hour_FIVE;       break;
        case 6:
        case 18:    hour_SIX;        break;
        case 7:
        case 19:    hour_SEVEN;      break;
        case 8:
        case 20:    hour_EIGHT;      break;
        case 9:
        case 21:    hour_NINE;       break;
        case 10:
        case 22:    hour_TEN;        break;
        case 11:
        case 23:    hour_ELEVEN;     break;
        case 12:    phrase_NOON;     break;
    }

    /* Always show O'CLOCK. */
    phrase_OCLOCK;
}

void loop()
{
    /*
     * Read time and temperature from DS3231.
     */
    struct tm* tm = ds3231_get_time();
    int8_t t;
    uint8_t f;
    double temp = ds3231_get_temp(&t, &f);

    /*
     * Display time using USART0 for debugging purposes.
     */
    char buff[16];
    sprintf(buff, "%d:%d:%d\n", tm->hour, tm->min, tm->sec);
    USART0_print(buff);

    /*
     * Display temperature using USART0 for debugging purposes.
     */
    sprintf(buff, "%d %u %f\n", t, f, temp);
    USART0_print(buff);

    /*
     * Update the LED matrix display.
     */
    static long last_time = 0;
    static int mode = 1;
    long diff = (tm->hour * 60 * 60 + tm->min * 60 + tm->sec) - last_time;
    if ((diff > 10) || (diff < -10)) {
        /* The mode changes every 10 seconds. */
        USART0_print("New mode!\n");
        mode = (mode + 1) % 4;
        last_time = tm->hour * 60 * 60 + tm->min * 60 + tm->sec;
    }

    uint16_t mask[DISPLAY_HEIGHT];
    memset(mask, 0, sizeof(mask));
    if (mode == 0) {
        /* The fancy word clock mode. */
        get_mask(tm, mask);
    } else if (mode == 1) {
        /* +-------+-------+
         * |       |       |
         * |    H O U R    |
         * |       |       |
         * +-------+-------+
         * |       |       |
         * | M I N U T E S |
         * |       |       |
         * +-------+-------+
         */
        draw_mask(mask, 1, 0, DIGITS_4x5[tm->hour / 10], 4, 5);
        draw_mask(mask, 6, 0, DIGITS_4x5[tm->hour % 10], 4, 5);
        draw_mask(mask, 1, 6, DIGITS_4x5[tm->min  / 10], 4, 5);
        draw_mask(mask, 6, 6, DIGITS_4x5[tm->min  % 10], 4, 5);
    } else if (mode == 2) {
        /* +-------+-------+
         * |       |       |
         * |     D A Y     |
         * |       |       |
         * +-------+-------+
         * |       |       |
         * |   M O N T H   |
         * |       |       |
         * +-------+-------+
         */
        draw_mask(mask, 1, 0, DIGITS_4x5[tm->mday / 10], 4, 5);
        draw_mask(mask, 6, 0, DIGITS_4x5[tm->mday % 10], 4, 5);
        draw_mask(mask, 1, 6, DIGITS_4x5[tm->mon  / 10], 4, 5);
        draw_mask(mask, 6, 6, DIGITS_4x5[tm->mon  % 10], 4, 5);
    } else if (mode == 3) {
        /* +-------+-------+
         * |       |       |
         * |  TEMPERATURE  |
         * |       |       |
         * +-------+-------+
         * |       |       |
         * |       |       |
         * |       |       |
         * +-------+-------+
         */
        draw_mask(mask, 1, 0, DIGITS_4x5[(int)temp / 10], 4, 5);
        draw_mask(mask, 6, 0, DIGITS_4x5[(int)temp % 10], 4, 5);
    }
    draw_matrix(mask);

    /*
     * Beep every hour.
     *
     * This may be a little annoying.
     */
}

/**
 * Entry point of this program.
 *
 * @return Never returns.
 */
int main(void)
{
    init();
    while (1)
        loop();
    return 0;
}
