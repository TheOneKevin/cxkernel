#include "system/time.h"

#define CURRENT_YEAR        2017                            // Change this each year!

int century_register = 0x00;                                // Set by ACPI table parsing code if possible

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int  year;

void out_byte (uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t in_byte (uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

enum
{
    cmos_address = 0x70,
    cmos_data    = 0x71
};

int get_update_in_progress_flag ()
{
    out_byte (cmos_address, 0x0A);
    return(in_byte (cmos_data) & 0x80);
}

unsigned char get_RTC_register (int reg)
{
    out_byte (cmos_address, reg);
    return in_byte (cmos_data);
}

void read_rtc ()
{
    unsigned char century = 0;
    unsigned char last_second;
    unsigned char last_minute;
    unsigned char last_hour;
    unsigned char last_day;
    unsigned char last_month;
    unsigned char last_year;
    unsigned char last_century;
    unsigned char registerB;

    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (get_update_in_progress_flag ()); // Make sure an update isn't in progress
    second = get_RTC_register (0x00);
    minute = get_RTC_register (0x02);
    hour = get_RTC_register (0x04);
    day = get_RTC_register (0x07);
    month = get_RTC_register (0x08);
    year = get_RTC_register (0x09);

    if (century_register != 0)
    {
        century = get_RTC_register (century_register);
    }

    do
    {
        last_second = second;
        last_minute = minute;
        last_hour = hour;
        last_day = day;
        last_month = month;
        last_year = year;
        last_century = century;

        while (get_update_in_progress_flag ())
            ;                                                // Make sure an update isn't in progress
        second = get_RTC_register (0x00);
        minute = get_RTC_register (0x02);
        hour = get_RTC_register (0x04);
        day = get_RTC_register (0x07);
        month = get_RTC_register (0x08);
        year = get_RTC_register (0x09);
        if (century_register != 0)
        {
            century = get_RTC_register (century_register);
        }
    } while ((last_second != second) || (last_minute != minute) || (last_hour != hour) ||
             (last_day != day) || (last_month != month) || (last_year != year) || (last_century != century));

    registerB = get_RTC_register (0x0B);

    // Convert BCD to binary values if necessary

    if (!(registerB & 0x04))
    {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
        if (century_register != 0)
        {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    // Convert 12 hour clock to 24 hour clock if necessary

    if (!(registerB & 0x02) && (hour & 0x80))
    {
        hour = ((hour & 0x7F) + 12) % 24;
    }

    // Calculate the full (4-digit) year

    if (century_register != 0)
    {
        year += century * 100;
    }
    else
    {
        year += (CURRENT_YEAR / 100) * 100;
        if (year < CURRENT_YEAR)
            year += 100;
    }
}

uint64_t getEpoch()
{
    read_rtc();
    uint32_t tm_year = year - 1970; //Should we really be governed by the Unix epoch? Why are most hobbyist OS designs based off of Unix? Where did our creativity go? Is that really the best design? *goes on rant*
    uint32_t tm_day = day;
    for(int i = 1; i < month; i++)
    {
        if((i == 1) || (i == 3) || (i == 5) || (i == 7) || (i == 8) || (i == 10) || (i == 12))
            tm_day += 31;
        else if((i == 4) || (i == 6) || (i == 9) || (i == 11))
            tm_day += 30;
        else if(i == 2)
            tm_day += 28;
    }

    if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        tm_day ++;

    uint64_t epoch = second + minute * 60 + hour * 3600 + tm_day * 86400 + (tm_year - 70) * 31536000 + ((tm_year - 69) / 4) * 86400 - ((tm_year - 1) / 100) * 86400 + ((tm_year + 299) / 400) * 86400;

    return epoch;
}
