#include "rtc.h"

#include <avr/io.h>
#include <i2chw/i2cmaster.h>

// statically allocated structure for time value
struct tm _tm;

uint8_t dec2bcd(uint8_t d) { return ((d / 10 * 16) + (d % 10)); }

uint8_t bcd2dec(uint8_t b) { return ((b / 16 * 10) + (b % 16)); }

uint8_t ds3231_read_byte(uint8_t offset)
{
    i2c_start((DS3231_ADDR << 1) | I2C_WRITE);
    i2c_write(offset);
    i2c_stop();

    i2c_start((DS3231_ADDR << 1) | I2C_READ);
    uint8_t ret = i2c_readNak();
    i2c_stop();

    return ret;
}

void ds3231_write_byte(uint8_t b, uint8_t offset)
{
    i2c_start((DS3231_ADDR << 1) | I2C_WRITE);
    i2c_write(offset);
    i2c_write(b);
    i2c_stop();
}

void ds3231_init(void)
{
    // Attempt autodetection:
    // 1) Read and save temperature register
    // 2) Write a value to temperature register
    // 3) Read back the value
    //   equal to the one written: DS1307, write back saved value and return
    //   different from written:   DS3231

    uint8_t temp1 = ds3231_read_byte(0x11);
    uint8_t temp2 = ds3231_read_byte(0x12);

    ds3231_write_byte(0xee, 0x11);
    ds3231_write_byte(0xdd, 0x12);

    if (ds3231_read_byte(0x11) == 0xee && ds3231_read_byte(0x12) == 0xdd) {
        // s_is_ds1307 = true;
        // restore values
        ds3231_write_byte(temp1, 0x11);
        ds3231_write_byte(temp2, 0x12);
    } else {
        // s_is_ds3231 = true;
    }
}

void ds3231_set_time(struct tm* tm_)
{
    i2c_start((DS3231_ADDR << 1) | I2C_WRITE);
    i2c_write(0x0);

    uint8_t century;
    if (tm_->year > 2000) {
        century = 0x80;
        tm_->year = tm_->year - 2000;
    } else {
        century = 0;
        tm_->year = tm_->year - 1900;
    }

    // clock halt bit is 7th bit of seconds: this is always cleared to start the clock
    i2c_write(dec2bcd(tm_->sec)); // seconds
    i2c_write(dec2bcd(tm_->min)); // minutes
    i2c_write(dec2bcd(tm_->hour)); // hours
    i2c_write(dec2bcd(tm_->wday)); // day of week
    i2c_write(dec2bcd(tm_->mday)); // day
    i2c_write(dec2bcd(tm_->mon) + century); // month
    i2c_write(dec2bcd(tm_->year)); // year

    i2c_stop();
}

struct tm* ds3231_get_time(void)
{
    uint8_t buff[9];
    uint8_t century = 0;

    // read 7 bytes starting from register 0
    // sec, min, hour, day-of-week, date, month, year
    i2c_start((DS3231_ADDR << 1) | I2C_WRITE);
    i2c_write(0x0);
    i2c_stop();

    i2c_start((DS3231_ADDR << 1) | I2C_READ);
    for (uint8_t i = 0; i < 6; i++)
        buff[i] = i2c_readAck();
    buff[6] = i2c_readNak();
    i2c_stop();

    // Clear clock halt bit from read data
    // This starts the clock for a DS1307, and has no effect for a DS3231
    buff[0] &= ~(_BV(CH_BIT)); // clear bit

    _tm.sec = bcd2dec(buff[0]);
    _tm.min = bcd2dec(buff[1]);
    _tm.hour = bcd2dec(buff[2]);
    _tm.mday = bcd2dec(buff[4]);
    _tm.mon = bcd2dec(buff[5] & 0x1F); // returns 1-12
    century = (buff[5] & 0x80) >> 7;
    _tm.year = century == 1 ? 2000 + bcd2dec(buff[6]) : 1900
            + bcd2dec(buff[6]); // year 0-99
    _tm.wday = bcd2dec(buff[3]); // returns 1-7

    if (_tm.hour == 0) {
        _tm.twelveHour = 0;
        _tm.am = 1;
    } else if (_tm.hour < 12) {
        _tm.twelveHour = _tm.hour;
        _tm.am = 1;
    } else {
        _tm.twelveHour = _tm.hour - 12;
        _tm.am = 0;
    }

    return &_tm;
}

float ds3231_get_temp(int8_t* i, uint8_t* f)
{
    uint8_t msb, lsb;

    *i = 0;
    *f = 0;

    i2c_start((DS3231_ADDR << 1) | I2C_WRITE);
    i2c_write(0x11);
    i2c_stop();

    i2c_start((DS3231_ADDR << 1) | I2C_READ);
    msb = i2c_readAck(); // integer part (in twos complement)
    lsb = i2c_readNak(); // fraction part
    i2c_stop();

    // integer part in entire byte
    *i = msb;
    // fractional part in top two bits (increments of 0.25)
    *f = (lsb >> 6) * 25;

    // float value can be read like so:
    return ((((short)msb << 8) | (short)lsb) >> 6) / 4.0f;
}
