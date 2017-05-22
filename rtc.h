#ifndef _RTC_H_
#define _RTC_H_

#include <stdbool.h>
#include <stdint.h>

#define DS3231_ADDR 0x68 // I2C address
#define CH_BIT 7 // clock halt bit

struct tm {
    int sec; // 0 to 59
    int min; // 0 to 59
    int hour; // 0 to 23
    int mday; // 1 to 31
    int mon; // 1 to 12
    int year; // year-99
    int wday; // 1-7

    // 12-hour clock data
    bool am; // true for AM, false for PM
    int twelveHour; // 12 hour clock time
};

/**
 * Converts from decimal to BCD.
 *
 * @param  d Decimal number.
 * @return   BCD number.
 */
uint8_t dec2bcd(uint8_t d);

/**
 * Converts from BCD to decimal.
 *
 * @param  b BCD number.
 * @return   Decimal number.
 */
uint8_t bcd2dec(uint8_t b);

/**
 * Reads a byte from a specific register of DS2321.
 *
 * @param  offset Register address.
 * @return        Register value.
 */
uint8_t ds3231_read_byte(uint8_t offset);

/**
 * Writes a byte to a specific register of DS2321.
 *
 * @param b      Byte to be written.
 * @param offset Register address.
 */
void ds3231_write_byte(uint8_t b, uint8_t offset);

/**
 * Initializes the DS2321.
 */
void ds3231_init(void);

/**
 * Sets the time of DS2321.
 *
 * @param tm_ The new time.
 */
void ds3231_set_time(struct tm* tm_);

/**
 * Gets the time from DS2321.
 *
 * @return The current time.
 */
struct tm* ds3231_get_time(void);

/**
 * Gets the temperature from DS2321.
 *
 * @param  i Integer part.
 * @param  f Fractional part in increments of .25.
 * @return   The temperature.
 */
float ds3231_get_temp(int8_t* i, uint8_t* f);

#endif // _RTC_H_
